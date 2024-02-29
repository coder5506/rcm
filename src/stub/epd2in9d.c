// Copyright (c) 2024 Eric Sessoms
// See license at end of file

// This file stubs out communication with the e-Paper display on a
// DGT Centaur board.  It is used for development and testing on Linux
// systems where the board is not available.

#include "../epd2in9d.h"

const int SCREEN_WIDTH  = 128;
const int SCREEN_HEIGHT = 296;

void epd2in9d_close(void) {
}

int epd2in9d_open(void) {
    return 0;
}

void epd2in9d_sleep(void) {
}

void epd2in9d_wake(void) {
}

void epd2in9d_clear(void) {
}

void epd2in9d_update(const unsigned char *data) {
    (void)data;
}

void epd2in9d_display(const unsigned char *data) {
    (void)data;
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
