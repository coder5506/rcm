// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "standard.h"
#include "board.h"
#include "centaur.h"
#include "chess/chess.h"
#include "db.h"

#include <algorithm>
#include <cassert>
#include <cstring>

#include <sys/select.h>

#include <jansson.h>

using namespace std;
using namespace thc;

StandardGame::StandardGame() {
    white = {
        .type = HUMAN,
        .human = {
            .error       = 0,
            .opportunity = 0,
        },
    };
    black = {
        .type = COMPUTER,
        .computer = {
            .engine = "stockfish",
            .elo    = 1350,
        },
    };
}

static json_t* player_to_json(const Player& player) {
    json_t *data = json_object();
    json_object_set_new(data, "player",
        json_string(player.type == COMPUTER ? "computer" : "human"));

    switch (player.type) {
    case COMPUTER:
        json_object_set_new(data, "engine", json_string(player.computer.engine));
        json_object_set_new(data, "elo", json_integer(player.computer.elo));
        break;
    case HUMAN:
        json_object_set_new(data, "error", json_integer(player.human.error));
        json_object_set_new(data, "opportunity", json_integer(player.human.opportunity));
        break;
    }

    return data;
}

char* StandardGame::settings_to_json() {
    json_t *settings = json_object();
    json_object_set_new(settings, "module", json_string("standard"));
    json_object_set_new(settings, "white", player_to_json(white));
    json_object_set_new(settings, "black", player_to_json(black));

    char *json = json_dumps(settings, 0);
    json_decref(settings);

    return json;
}

static int player_from_json(Player& player, json_t* data) {
    const char* type = json_string_value(json_object_get(data, "player"));
    if (!type) {
        return 1;
    }

    if (strcmp(type, "computer") == 0) {
        player.type = COMPUTER;

        const char* engine = json_string_value(json_object_get(data, "engine"));
        if (!engine) {
            engine = "stockfish";
        }
        player.computer.engine = engine;

        int elo = json_integer_value(json_object_get(data, "elo"));
        player.computer.elo = max(1350, min(2850, elo));

        return 0;
    }

    if (strcmp(type, "human") == 0) {
        player.type = HUMAN;

        int error = json_integer_value(json_object_get(data, "error"));
        player.human.error = max(0, error);

        int opportunity = json_integer_value(json_object_get(data, "opportunity"));
        player.human.opportunity = max(0, opportunity);

        return 0;
    }

    return 1;
}

int StandardGame::settings_from_json(const char* json) {
    json_t *data = json_loads(json, 0, NULL);
    if (!data) {
        return 1;
    }

    const char *module = json_string_value(json_object_get(data, "module"));
    if (strcmp(module, "standard") != 0) {
        json_decref(data);
        return 1;
    }

    json_t *white_settings = json_object_get(data, "white");
    json_t *black_settings = json_object_get(data, "black");
    if (!white_settings || !black_settings) {
        json_decref(data);
        return 1;
    }

    int err = player_from_json(white, white_settings);
    if (!err) {
        err = player_from_json(black, black_settings);
    }

    json_decref(data);
    return err;
}

// Ensure game and current settings are persisted to database
void StandardGame::on_changed(Game& game) {
    if (!game.started) {
        return;
    }

    if (white.type == COMPUTER) {
        // TODO Add ELO to computer name
        game.tag("White") = white.computer.engine;
    } else {
        game.tag("White") = "Human";
    }

    if (black.type == COMPUTER) {
        game.tag("Black") = black.computer.engine;
    } else {
        game.tag("Black") = "Human";
    }

    game.settings = settings_to_json();
    db.save_game(game);
}

StandardGame::~StandardGame() {
    if (centaur.game) {
        centaur.game->unobserve(this);
    }
}

void StandardGame::set_game(unique_ptr<Game> game) {
    // The current game is managed by the global `centaur` object.  This hook is
    // only to manage subscribing to and unsubscribing from game events.
    if (centaur.game) {
        centaur.game->unobserve(this);
    }
    centaur.set_game(move(game));
    centaur.game->observe(this);
}

// When running in the console, we can hit "Enter" to exit cleanly.  Otherwise
// this is just a sleep.
static int poll_for_keypress(int timeout_ms) {
    const struct timespec timeout = {
        .tv_sec  =  timeout_ms / 1000,
        .tv_nsec = (timeout_ms % 1000) * 1000000,
    };

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);

    // Yields 1 if there's input, 0 otherwise
    auto result = pselect(1, &readfds, NULL, NULL, &timeout, NULL);
    if (result) {
        getchar();
    }
    return result;
}

// We can only read moves from known positions.  Here we wait for the pieces to
// be setup in a known position, which can be either the last position seen on
// the board (to resume the prior game) or the start position (for a new game).
void StandardGame::start() {
    // Load latest game and settings from database.
    auto game = db.load_latest();
    if (static_cast<bool>(game) && !game->settings.empty()) {
        settings_from_json(game->settings.data());
    }
    if (!game) {
        game = make_unique<Game>();
    }

    // Switch pieces around if human is playing black against computer.
    centaur.reversed(black.type == HUMAN && white.type == COMPUTER);

    // Display last position from previous game to help user get setup.
    set_game(move(game));
    centaur.render();

    while (!poll_for_keypress(500)) {
        // Discard any actions generated during setup
        centaur.purge_actions();

        const auto boardstate = centaur.getstate();
        if (boardstate == centaur.game->bitmap()) {
            // Recognize last board position and resume prior game.
            centaur.game->started = time(NULL);
            break;
        }
        else if (boardstate == Board::STARTING_POSITION) {
            // Recognize starting position for new game.
            centaur.game->fen("");
            centaur.render();
            break;
        }
    }
}

// Gameplay loop: read and interpret player actions to update game state
void StandardGame::run() {
    Engine engine{"stockfish"};

    auto player = centaur.game->WhiteToPlay() ? &white : &black;

    // This check is necessary b/c in the main loop (below) we read the
    // computer's move at the top but request it at the bottom.  When computer
    // moves first, we need this extra request to kick things off.
    if (player->type == COMPUTER) {
        // If camputer has first move, see what it wants to do.
        engine.play(*centaur.game, player->computer.elo);
    }

    while (!poll_for_keypress(200)) {
        player = centaur.game->WhiteToPlay() ? &white : &black;

        // Does computer have move to play?
        if (auto move = player->type == COMPUTER ? engine.move() : nullopt) {
            // Prompt user to move piece.
            centaur.led_from_to(move->src, move->dst);
        }

        // Are there player actions to interpret?
        if (centaur.update_actions() == 0) {
            // No actions, nothing's changed, there's nothing to do.
            continue;

            // N.B., `update_actions` returns the number of outstanding actions,
            // not just new actions.  So we are not shorting the loop if there
            // is work to be done.
        }

        // We don't ever want to read actions newer than the known board state.
        // So it's `update_actions` first, then `getstate`.
        const auto boardstate = centaur.getstate();

        // Starting a new game?
        if (boardstate == Board::STARTING_POSITION) {
            centaur.purge_actions();
            if (centaur.game->started) {
                // Replace in-progress game with new game.
                set_game(make_unique<Game>());
            }
            continue;
        }

        // Interpret player actions.
        vector<Move>   candidates;
        optional<Move> takeback;
        if (!centaur.read_move(boardstate, candidates, takeback)) {
            // No move or takeback, so nothing more to do right now.
            continue;
        }

        optional<Move> move;
        if (!candidates.empty()) {
            // TODO Show promotions menu to select from multiple candidates.
            move = candidates.front();
        }

        // Execute player actions
        if (takeback && move) {
            centaur.game->revise_move(*takeback, *move);
            centaur.led(move->dst);
        }
        else if (takeback) {
            centaur.game->play_takeback(*takeback);
            centaur.led(takeback->src);
        }
        else if (move) {
            centaur.game->play_move(*move);
            centaur.led(move->dst);
        }

        // We completed a move or takeback, so it is now the other player's
        // turn.  If it's the computer's turn, ask it what it wants to do.
        auto new_player = centaur.game->WhiteToPlay() ? &white : &black;
        if (new_player != player && new_player->type == COMPUTER) {
            engine.play(*centaur.game, new_player->computer.elo);
        }
    }
}

// Execute "standard game" module
void StandardGame::main() {
    start();  // Wait for pieces to be set up
    run();    // Play a game
}

// This file is part of the Raccoon's Centaur Mods (RCM).
//
// RCM is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RCM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
