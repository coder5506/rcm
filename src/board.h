// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef BOARD_H
#define BOARD_H

#include "chess/chess.h"

// 0xffff00000000ffff
extern const uint64_t STARTING_POSITION;

extern bool board_reversed;

// Shutdown connection to board
void board_close(void);

// Initialize connection to board
int board_open(void);

// Read current state of board fields
// MSB: H1=63 G1 F1 ... A1, H2 G2 ... A2, ..., H8 G8 ... A8=0
uint64_t board_getstate(void);

// Return battery and charging status
int board_batterylevel(void);
int board_charging(void);

int board_leds_off(void);
int board_led_flash(void);
int board_led(enum Square square);
int board_led_array(const enum Square *squares, int num_squares);
int board_led_from_to(enum Square from, enum Square to);

// Return number of actions read
int board_read_actions(struct Action *actions, int max_actions);

static inline enum Square rotate_square(enum Square square) {
    // square  == 8 * (7 - row) + col
    //         == 56 - 8 * row + col
    //         == 56 - (8 * row - col)
    // rotated == 8 * row + (7 - col)
    //         == 7 + (8 * row - col)
    // square + rotated == 63
    // et voila
    return 63 - square;
}

#endif

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
