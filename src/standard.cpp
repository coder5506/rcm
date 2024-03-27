// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "standard.h"
#include "board.h"
#include "centaur.h"
#include "chess/chess.h"
#include "db.h"

#include <cassert>
#include <cstdio>
#include <cstring>

#include <sys/select.h>

#include <jansson.h>

StandardGame::StandardGame() {
    white = {
        .type = COMPUTER,
        .computer = {
            .engine = "stockfish",
            .elo    = 1400,
        },
    };
    black = {
        .type = HUMAN,
        .human = {
            .error       = 0,
            .opportunity = 0,
        },
    };
}

static json_t* player_to_json(const Player* player) {
    json_t *data = json_object();
    json_object_set_new(data, "player",
        json_string(player->type == COMPUTER ? "computer" : "human"));

    switch (player->type) {
    case COMPUTER:
        json_object_set_new(data, "engine", json_string(player->computer.engine));
        json_object_set_new(data, "elo", json_integer(player->computer.elo));
        break;
    case HUMAN:
        json_object_set_new(data, "error", json_integer(player->human.error));
        json_object_set_new(data, "opportunity", json_integer(player->human.opportunity));
        break;
    }

    return data;
}

char* StandardGame::settings_to_json() {
    json_t *settings = json_object();
    json_object_set_new(settings, "module", json_string("standard"));
    json_object_set_new(settings, "white", player_to_json(&white));
    json_object_set_new(settings, "black", player_to_json(&black));

    char *json = json_dumps(settings, 0);
    json_decref(settings);

    return json;
}

static int player_from_json(Player* player, json_t* data) {
    const char* type = json_string_value(json_object_get(data, "player"));
    if (!type) {
        return 1;
    }

    if (strcmp(type, "computer") == 0) {
        player->type = COMPUTER;

        const char *engine = json_string_value(json_object_get(data, "engine"));
        if (!engine) {
            engine = "stockfish";
        }
        player->computer.engine = engine;

        int elo = json_integer_value(json_object_get(data, "elo"));
        if (elo < 1400 || 2800 < elo) {
            elo = 2000;  // for no other reason than this is the ELO basis
        }
        player->computer.elo = elo;

        return 0;
    }

    if (strcmp(type, "human") == 0) {
        player->type = HUMAN;

        int error = json_integer_value(json_object_get(data, "error"));
        player->human.error = error < 0 ? 0 : error;

        int opportunity = json_integer_value(json_object_get(data, "opportunity"));
        player->human.opportunity = opportunity < 0 ? 0 : opportunity;

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

    int err = player_from_json(&white, white_settings);
    if (!err) {
        err = player_from_json(&black, black_settings);
    }

    json_decref(data);
    return err;
}

// Ensure game and current settings are persisted to database
void StandardGame::model_changed(Game& game) {
    if (!game.started) {
        return;
    }

    if (white.type == COMPUTER) {
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

void StandardGame::set_game(std::unique_ptr<Game> game) {
    if (centaur.game) {
        centaur.game->unobserve(this);
    }
    centaur.set_game(std::move(game));
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

    return pselect(1, &readfds, NULL, NULL, &timeout, NULL);
}

void StandardGame::start() {
    auto game = db.load_latest();
    if (static_cast<bool>(game) && !game->settings.empty()) {
        settings_from_json(game->settings.data());
    }
    if (!game) {
        game = std::make_unique<Game>();
    }

    // Switch pieces around if human is playing black against computer
    centaur.reversed(black.type == HUMAN && white.type == COMPUTER);

    set_game(std::move(game));
    centaur.render();

    for (;;) {
        const auto boardstate = centaur.getstate();
        if (boardstate == centaur.game->bitmap()) {
            // Resume last game
            centaur.game->started = time(NULL);
            break;
        }
        if (boardstate == Board::STARTING_POSITION) {
            // Start new game
            break;
        }

        // Discard actions generated during setup
        (void)centaur.update_actions();
        if (poll_for_keypress(500) > 0) {
            break;
        }
    }

    centaur.purge_actions();
}

// Gameplay loop: Read and interpret user actions to update game state
void StandardGame::run() {
    std::vector<thc::Move>   candidates;
    std::optional<thc::Move> takeback;

    auto player = centaur.game->WhiteToPlay() ? &white : &black;

    // If camputer has first move, see what it wants to do
    if (player->type == COMPUTER) {
        // In case human played for computer and something is left in the queue
        (void)engine_move(*centaur.game, player->computer.engine);
        // Ask for new move
        engine_play(*centaur.game, player->computer.engine, player->computer.elo);
    }

    for (;;) {
        player = centaur.game->WhiteToPlay() ? &white : &black;

        // Check if computer has move to play
        std::optional<thc::Move> move;
        if (player->type == COMPUTER) {
            move = engine_move(*centaur.game, player->computer.engine);
        }
        if (move) {
            // Prompt user to move piece
            centaur.led_from_to(move->src, move->dst);
        }

        Bitmap boardstate = 0;
        auto maybe_valid = false;

        // Check if there are user actions to process
        if (centaur.update_actions() == 0) {
            // No actions, nothing's changed, there's nothing to do
            goto next;
        }
        boardstate = centaur.getstate();

        // Detect start of new game
        if (boardstate == Board::STARTING_POSITION) {
            centaur.actions.clear();
            if (centaur.game->started) {
                // Replace in-progress game with new game
                set_game(std::make_unique<Game>());
            }
            goto next;
        }

        // Interpret player actions
        maybe_valid =
            centaur.read_move(boardstate, candidates, takeback);
        if (!maybe_valid) {
            goto next;
        }

        // Execute player actions
        // TODO promotions menu
        if (!candidates.empty()) {
            move = candidates.front();
        }
        // if (takeback && move) {
        //     err = game_complete_move(centaur.game, takeback, move);
        //     centaur_led(move->dst);
        // } else  if (takeback) {
        if (takeback) {
            centaur.game->play_takeback(*takeback);
            centaur.led(takeback->src);
        } else if (move) {
            centaur.game->play_move(*move);
            centaur.led(move->dst);
        }

        // If is now computer's turn, ask for its move
        player = centaur.game->WhiteToPlay() ? &white : &black;
        if (player->type == COMPUTER) {
            // In case human played for computer and something is left in the queue
            (void)engine_move(*centaur.game, player->computer.engine);
            // Ask for new move
            engine_play(*centaur.game, player->computer.engine, player->computer.elo);
        }

    next:
        // Looping 4x/second seems(?) sufficient for smooth play
        if (poll_for_keypress(250) > 0) {
            break;
        }
    }
}

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
