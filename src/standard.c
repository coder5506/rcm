// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "standard.h"
#include "board.h"
#include "centaur.h"
#include "chess/chess.h"
#include "db.h"
#include "utility/list.h"

#include <sys/select.h>

enum PlayerType {
    COMPUTER,
    HUMAN,
};

struct Player {
    enum PlayerType type;
    union {
        struct {
            const char *engine;
            int         elo;
        } computer;
    };
};

struct StandardGame {
    struct Player white;
    struct Player black;
};

static struct StandardGame standard = {
    .white = {.type = COMPUTER, .computer = {.engine = "stockfish", .elo = 1400}},
    .black = {.type = HUMAN},
};

static void game_changed(struct Game *game, void *data) {
    (void)data;
    if (game->started) {
        db_save_game(game);
    }
}

static void standard_stop(void) {
    MODEL_UNOBSERVE(centaur.game, game_changed, NULL);
}

static void standard_start(void) {
    board_reversed = standard.black.type == HUMAN && standard.white.type == COMPUTER;
    centaur_render();
    // TODO
    // - load latest game
    // - wait for:
    //   - board to match game state
    //   - board to match starting position
    //   - keypress for menu

    MODEL_OBSERVE(centaur.game, game_changed, NULL);
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
    // Declared outside loop to avoid redundant allocations
    struct List *candidates = list_new();
    struct Move *takeback   = NULL;

    while (true) {
        list_clear(candidates);
        takeback = NULL;

        // Update history before reading boardstate.  We don't want actions
        // that are newer than the boardstate, and we can always get the latest
        // on the next iteration.
        if (centaur_update_actions() == 0) {
            goto next;
        }
        const uint64_t boardstate = centaur_getstate();

        // Put pieces in starting position for a new game.
        if (boardstate == STARTING_POSITION) {
            centaur_clear_actions();
            if (game_started(centaur.game)) {
                game_set_start(centaur.game, NULL);
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
