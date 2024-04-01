// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "board.h"
#include "boardserial.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <ctime>

using namespace std;
using namespace thc;

// Return battery status
int Board::batterylevel() {
    const auto charging = boardserial.chargingstate();
    if (charging == -1) {
        return -1;
    }
    return charging & 31;
}

// Return charging status
int Board::charging() {
    const auto charging = boardserial.chargingstate();
    if (charging == -1) {
        return -1;
    }
    return (charging >> 5 & 7) == 1;
}

static Bitmap reverse_bits(Bitmap value) {
    Bitmap reversed = 0;
    for (auto i = 0; i < 64; ++i) {
        reversed = (reversed << 1) | (value & 1);
        value >>= 1;
    }
    return reversed;
}

// Read current state of board fields
Bitmap Board::getstate() {
    const auto boardstate = boardserial.boardstate();
    return reversed ? reverse_bits(boardstate) : boardstate;
}

int Board::read_actions(ActionList& actions) {
    uint8_t buf[256];
    const auto num_read = boardserial.readdata(buf, sizeof buf);
    if (num_read <= 6) {
        // Failed to read data packet
        return 0;
    }

    auto n = 0;
    for (auto i = 5; i < num_read - 1;) {
        switch (buf[i++]) {
        case 64:  // Lift
            if (0 <= buf[i] && buf[i] < 64) {
                auto lift = static_cast<Square>(buf[i++]);
                if (reversed) {
                    lift = rotate_square(lift);
                }
                actions.push_back(Action{lift, SQUARE_INVALID});
                ++n;
            }
            break;
        case 65:  // Place
            if (0 <= buf[i] && buf[i] < 64) {
                auto place = static_cast<Square>(buf[i++]);
                if (reversed) {
                    place = rotate_square(place);
                }
                actions.push_back(Action{SQUARE_INVALID, place});
                ++n;
            }
            break;
        default:
            break;
        }
    }

    return n;
}

int Board::leds_off() {
    return boardserial.leds_off();
}

int Board::led_flash() {
    return boardserial.led_flash();
}

int Board::led(Square square) {
    if (reversed) {
        square = rotate_square(square);
    }
    return boardserial.led(square);
}

int Board::led_array(const vector<Square>& squares) {
    int rotated_squares[squares.size()];
    for (auto i = 0; i != squares.size(); ++i) {
        if (reversed) {
            rotated_squares[i] = rotate_square(squares[i]);
        } else {
            rotated_squares[i] = squares[i];
        }
    }
    return boardserial.led_array(rotated_squares, squares.size());
}

int Board::led_from_to(Square from, Square to) {
    if (reversed) {
        from = rotate_square(from);
        to   = rotate_square(to);
    }
    return boardserial.led_from_to(from, to);
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
