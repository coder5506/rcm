// Copyright (c) 2024 Eric Sessoms
// See license at end of file

#include "centaur.h"
#include "httpd.h"

#include <stdio.h>

int main(void) {
    if (centaur_open() != 0) {
        return -1;
    }
    if (httpd_start() != 0) {
        centaur_close();
        return -1;
    }

    // centaur_wake();
    // centaur_clear();
    // sleep_ms(3000);

    centaur_render();

    getchar();

    httpd_stop();
    centaur_close();

    // struct Position position;
    // from_fen(&position, STARTING_FEN);
    // struct Component *board_ui = board_component_alloc(&position);
    // centaur_set_ui(board_ui);
    // centaur_render();
    // sleep_ms(2000);
    // const uint64_t boardstate = centaur_getstate();
    // centaur_printstate(boardstate);
    // struct Game *game = game_new(NULL);
    // while (1) {
    //     struct Move *move = NULL;
    //     board_readmove(game, &move);
    //     if (move) {
    //         printf("MOVED %d - %d\n", move->from, move->to);
    //         move_free(move);
    //         move = NULL;
    //     }
    //     sleep_ms(1000);
    // }
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
