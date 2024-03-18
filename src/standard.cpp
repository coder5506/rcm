// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "standard.h"
#include "board.h"
#include "centaur.h"
#include "chess/chess.h"
#include "db.h"
#include "utility/list.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <sys/select.h>

#include <jansson.h>

// Either player can be computer or human, no restrictions
enum PlayerType {
    COMPUTER,
    HUMAN,
};

struct Player {
    enum PlayerType type;
    union {
        // Engine settings for computer players
        struct {
            const char *engine;  // e.g. "stockfish"
            int         elo;     // Desired playing strength 1400..2800
            // N.B., 2800 is about as good as Stockfish can do on a Pi Zero
        } computer;
        // Coaching settings for human players
        struct {
            // centipawns, zero to disable
            int error;        // Alert moves that actively worsen your position
            int opportunity;  // Alert moves that fail to improve your position
        } human;
    };
};

struct StandardGame {
    struct Player white;
    struct Player black;
};

static struct StandardGame standard = {
    .white = {
        .type = COMPUTER,
        .computer = {
            .engine = "stockfish",
            .elo    = 1400,
        },
    },
    .black = {
        .type = HUMAN,
        .human = {
            .error       = 0,
            .opportunity = 0,
        },
    },
};

static json_t *player_to_json(const struct Player *player) {
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

static char *settings_to_json(void) {
    json_t *settings = json_object();
    json_object_set_new(settings, "module", json_string("standard"));
    json_object_set_new(settings, "white", player_to_json(&standard.white));
    json_object_set_new(settings, "black", player_to_json(&standard.black));

    char *json = json_dumps(settings, 0);
    json_decref(settings);

    return json;
}

static int player_from_json(struct Player *player, json_t *data) {
    const char *type = json_string_value(json_object_get(data, "player"));
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

int settings_from_json(const char *json) {
    json_t *data = json_loads(json, 0, NULL);
    if (!data) {
        return 1;
    }

    const char *module = json_string_value(json_object_get(data, "module"));
    if (strcmp(module, "standard") != 0) {
        json_decref(data);
        return 1;
    }

    json_t *white = json_object_get(data, "white");
    json_t *black = json_object_get(data, "black");
    if (!white || !black) {
        json_decref(data);
        return 1;
    }

    int err = player_from_json(&standard.white, white);
    if (!err) {
        err = player_from_json(&standard.black, black);
    }

    json_decref(data);
    return err;
}

// Ensure game and current settings are persisted to database
static void game_changed(struct Game *game, void *data) {
    (void)data;
    if (!game->started) {
        return;
    }

    if (standard.white.type == COMPUTER) {
        game_set_tag(game, "White", (char*)standard.white.computer.engine);
    } else {
        game_set_tag(game, "White", (void*)"Human");
    }

    if (standard.black.type == COMPUTER) {
        game_set_tag(game, "Black", (char*)standard.black.computer.engine);
    } else {
        game_set_tag(game, "Black", (void*)"Human");
    }

    game->settings = settings_to_json();
    db_save_game(game);
}

static void standard_stop(void) {
    if (centaur.game) {
        MODEL_UNOBSERVE(centaur.game, game_changed, NULL);
    }
}

static void standard_set_game(struct Game *game) {
    if (centaur.game) {
        MODEL_UNOBSERVE(centaur.game, game_changed, NULL);
    }
    centaur_set_game(game);
    MODEL_OBSERVE(centaur.game, game_changed, NULL);
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

static void standard_start(void) {
    struct Game *game = db_load_latest();
    if (game && game->settings) {
        settings_from_json(game->settings);
    }
    if (!game) {
        game = game_from_fen(NULL);
    }

    // Switch pieces around if human is playing black against computer
    board_reversed = standard.black.type == HUMAN && standard.white.type == COMPUTER;

    standard_set_game(game);
    centaur_render();

    struct Position *current = game_current(centaur.game);
    while (1) {
        const uint64_t boardstate = centaur_getstate();
        if (boardstate == current->bitmap) {
            // Resume last game
            centaur.game->started = time(NULL);
            break;
        }
        if (boardstate == STARTING_POSITION) {
            // Start new game
            break;
        }

        // Discard actions generated during setup
        (void)centaur_update_actions();
        if (poll_for_keypress(500) > 0) {
            break;
        }
    }

    centaur_purge_actions();
}

// Gameplay loop: Read and interpret user actions to update game state
static void standard_run(void) {
    struct List *candidates  = list_new();
    struct Move *takeback    = NULL;

    const struct Position *current = game_current(centaur.game);
    const struct Player   *player  = current->turn == 'w' ? &standard.white : &standard.black;

    // If camputer has first move, see what it wants to do
    current = game_current(centaur.game);
    player  = current->turn == 'w' ? &standard.white : &standard.black;
    if (player->type == COMPUTER) {
        // In case human played for computer and something is left in the queue
        (void)engine_move(centaur.game, player->computer.engine);
        // Ask for new move
        engine_play(centaur.game, player->computer.engine, player->computer.elo);
    }

    while (1) {
        current = game_current(centaur.game);
        player  = current->turn == 'w' ? &standard.white : &standard.black;

        // Check if computer has move to play
        struct Move *move = NULL;
        if (player->type == COMPUTER) {
            move = engine_move(centaur.game, player->computer.engine);
        }
        if (move) {
            // Prompt user to move piece
            centaur_led_from_to(move->from, move->to);
        }

        uint64_t boardstate = 0;
        bool maybe_valid = false;
        int err = 0;

        // Check if there are user actions to process
        if (centaur_update_actions() == 0) {
            // No actions, nothing's changed, there's nothing to do
            goto next;
        }
        boardstate = centaur_getstate();

        // Detect start of new game
        if (boardstate == STARTING_POSITION) {
            centaur_clear_actions();
            if (centaur.game->started) {
                // Replace in-progress game with new game
                standard_set_game(game_from_fen(NULL));
            }
            goto next;
        }

        // Interpret player actions
        list_clear(candidates);
        takeback = NULL;
        maybe_valid =
            centaur_read_move(candidates, &takeback, centaur.game, boardstate);
        if (!maybe_valid) {
            goto next;
        }

        // Execute player actions
        // TODO promotions menu
        move = (struct Move*)list_first(candidates);
        err = 0;
        if (takeback && move) {
            err = game_complete_move(centaur.game, takeback, move);
            centaur_led(move->to);
        } else  if (takeback) {
            err = game_apply_takeback(centaur.game, takeback);
            centaur_led(takeback->from);
        } else if (move) {
            err = game_apply_move(centaur.game, move);
            centaur_led(move->to);
        }

        // This really shouldn't happen.  If we read a move, we should be able
        // to apply it.
        assert(err == 0);

        // If is now computer's turn, ask for its move
        current = game_current(centaur.game);
        player  = current->turn == 'w' ? &standard.white : &standard.black;
        printf("turn %c player %d\n", current->turn, player->type);
        if (player->type == COMPUTER) {
            // In case human played for computer and something is left in the queue
            (void)engine_move(centaur.game, player->computer.engine);
            // Ask for new move
            printf("requesting engine move\n");
            engine_play(centaur.game, player->computer.engine, player->computer.elo);
        }

    next:
        // Looping 4x/second seems(?) sufficient for smooth play
        if (poll_for_keypress(250) > 0) {
            break;
        }
    }
}

void standard_main(void) {
    standard_start();  // Wait for pieces to be set up
    standard_run();    // Play a game
    standard_stop();   // Clean up
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
