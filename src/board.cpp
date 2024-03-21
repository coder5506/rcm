// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "board.h"
#include "boardserial.h"
#include "thc/thc.h"

#include <cassert>
#include <cstdio>
#include <cstring>

#include <time.h>

const std::uint64_t STARTING_POSITION = 0xffff00000000ffff;

bool board_reversed = false;

// Shutdown connection to board
void board_close() {
    boardserial_close();
}

// Initialize connection to board
int board_open() {
    return boardserial_open();
}

static std::uint64_t reverse_bits(std::uint64_t value) {
    std::uint64_t reversed = 0;
    for (int i = 0; i < 64; ++i) {
        reversed = (reversed << 1) | (value & 1);
        value >>= 1;
    }
    return reversed;
}

// Read current state of board fields
std::uint64_t board_getstate() {
    const std::uint64_t boardstate = boardserial_boardstate();
    if (board_reversed) {
        return reverse_bits(boardstate);
    }
    return boardstate;
}

// Return battery status
int board_batterylevel() {
    const int charging = boardserial_chargingstate();
    if (charging == -1) {
        return -1;
    }
    return charging & 31;
}

// Return charging status
int board_charging() {
    const int charging = boardserial_chargingstate();
    if (charging == -1) {
        return -1;
    }
    return (charging >> 5 & 7) == 1;
}

int board_leds_off() {
    return boardserial_leds_off();
}

int board_led_flash() {
    return boardserial_led_flash();
}

int board_led(thc::Square square) {
    if (board_reversed) {
        square = rotate_square(square);
    }
    return boardserial_led(square);
}

int board_led_array(const std::vector<thc::Square>& squares) {
    int rotated_squares[squares.size()];
    for (auto i = 0; i != squares.size(); ++i) {
        if (board_reversed) {
            rotated_squares[i] = rotate_square(squares[i]);
        } else {
            rotated_squares[i] = squares[i];
        }
    }
    return boardserial_led_array(rotated_squares, squares.size());
}

int board_led_from_to(thc::Square from, thc::Square to) {
    if (board_reversed) {
        from = rotate_square(from);
        to   = rotate_square(to);
    }
    return boardserial_led_from_to(from, to);
}

int board_read_actions(std::vector<Action>& actions) {
    unsigned char buf[256];
    const auto num_read = boardserial_readdata(buf, sizeof buf);
    if (num_read <= 6) {
        // Failed to read data packet
        return 0;
    }

    auto n = 0;
    for (auto i = 5; i < num_read - 1;) {
        switch (buf[i++]) {
        case 64:
            if (0 <= buf[i] && buf[i] < 64) {
                thc::Square lift = static_cast<thc::Square>(buf[i++]);
                if (board_reversed) {
                    lift = rotate_square(lift);
                }
                actions.push_back(Action{lift, thc::SQUARE_INVALID});
                ++n;
            }
            break;
        case 65:
            if (0 <= buf[i] && buf[i] < 64) {
                thc::Square place = static_cast<thc::Square>(buf[i++]);
                if (board_reversed) {
                    place = rotate_square(place);
                }
                actions.push_back(Action{thc::SQUARE_INVALID, place});
                ++n;
            }
            break;
        default:
            break;
        }
    }

    return n;
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
