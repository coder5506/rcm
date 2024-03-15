// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "standard.h"
#include "board.h"
#include "centaur.h"
#include "chess/chess.h"
#include "db.h"
#include "utility/list.h"
#include "utility/utility.h"

#include <string.h>

#include <sys/select.h>

#include <jansson.h>

enum PlayerType {
    COMPUTER,
    HUMAN,
};

struct Player {
    enum PlayerType type;
    union {
        struct {
            const char *engine;  // e.g. "stockfish"
            int         elo;     // 1400 .. 3000
        } computer;
        struct {
            int blunder;    // centipawns, zero to disable
            int imprecise;  // ditto
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
            .elo    = 1600,
        },
    },
    .black = {
        .type = HUMAN,
        .human = {
            .blunder   = 0,
            .imprecise = 0,
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
        json_object_set_new(data, "blunder", json_integer(player->human.blunder));
        json_object_set_new(data, "imprecise", json_integer(player->human.imprecise));
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
        if (elo < 1400 || 3000 < elo) {
            elo = 2000;
        }
        player->computer.elo = elo;

        return 0;
    }

    if (strcmp(type, "human") == 0) {
        player->type = HUMAN;

        int blunder = json_integer_value(json_object_get(data, "blunder"));
        player->human.blunder = blunder < 0 ? 0 : blunder;

        int imprecise = json_integer_value(json_object_get(data, "imprecise"));
        player->human.imprecise = imprecise < 0 ? 0 : imprecise;

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
        goto error;
    }

    json_t *white = json_object_get(data, "white");
    json_t *black = json_object_get(data, "black");
    if (!white || !black) {
        goto error;
    }

    int err = player_from_json(&standard.white, white);
    if (!err) {
        err = player_from_json(&standard.black, black);
    }

    if (!err) {
        json_decref(data);
        return 0;
    }

error:
    json_decref(data);
    return 1;
}

static void game_changed(struct Game *game, void *data) {
    (void)data;
    if (!game->started) {
        return;
    }

    if (standard.white.type == COMPUTER) {
        game_set_tag(game, "White", (char*)standard.white.computer.engine);
    } else {
        game_set_tag(game, "White", "Human");
    }

    if (standard.black.type == COMPUTER) {
        game_set_tag(game, "Black", (char*)standard.black.computer.engine);
    } else {
        game_set_tag(game, "Black", "Human");
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
        sleep_ms(500);
    }

    centaur_purge_actions();
}

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

static void standard_run(void) {
    struct List *candidates = list_new();
    struct Move *takeback   = NULL;

    while (1) {
        list_clear(candidates);
        takeback = NULL;

        if (centaur_update_actions() == 0) {
            // No actions, nothing's changed, there's nothing to do
            goto next;
        }
        const uint64_t boardstate = centaur_getstate();

        // Detect start of new game
        if (boardstate == STARTING_POSITION) {
            centaur_clear_actions();
            if (centaur.game->started) {
                standard_set_game(game_from_fen(NULL));
            }
            goto next;
        }

        bool maybe_valid =
            centaur_read_move(candidates, &takeback, centaur.game, boardstate);
        if (!maybe_valid) {
            goto next;
        }

        // TODO promotions menu
        struct Move *move = list_first(candidates);
        if (takeback && move) {
            game_complete_move(centaur.game, takeback, move);
            centaur_led(move->to);
        } else  if (takeback) {
            game_apply_takeback(centaur.game, takeback);
            centaur_led(takeback->from);
        } else if (move) {
            game_apply_move(centaur.game, move);
            centaur_led(move->to);
        }

    next:
        // Looping 4x/second seems sufficient for smooth play
        if (poll_for_keypress(250) > 0) {
            break;
        }
    }
}

void standard_main(void) {
    standard_start();
    standard_run();
    standard_stop();
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
