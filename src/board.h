// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef BOARD_H
#define BOARD_H

#include "boardserial.h"
#include "chess/chess.h"

#include <vector>

class Board {
    BoardSerial boardserial;

public:
    static const Bitmap STARTING_POSITION = 0xffff00000000ffff;

    bool reversed{false};

    // Return battery and charging status
    int batterylevel();
    int charging();

    // Read current state of board fields
    // MSB: H1=63 G1 F1 ... A1, H2 G2 ... A2, ..., H8 G8 ... A8=0
    Bitmap getstate();

    // Return number of actions read
    int read_actions(ActionHistory& actions);

    int leds_off();
    int led_flash();
    int led(thc::Square square);
    int led_array(const std::vector<thc::Square>& squares);
    int led_from_to(thc::Square from, thc::Square to);
};

inline thc::Square rotate_square(thc::Square square) {
    // square  == 8 * (7 - row) + col
    //         == 56 - 8 * row + col
    //         == 56 - (8 * row - col)
    // rotated == 8 * row + (7 - col)
    //         == 7 + (8 * row - col)
    // square + rotated == 63
    // et voila
    return static_cast<thc::Square>(63 - square);
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
