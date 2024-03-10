// Copyright (C) 2024 Eric Sessoms
// See license at end of file

// This file stubs out communication with the e-Paper display on a DGT Centaur
// board.  It is used for development and testing on Linux systems where the
// board is not available.

#include "../epd2in9d.h"

// Utility
void sleep_ms(int milliseconds);

void epd2in9d_close(void) {
}

int epd2in9d_open(void) {
    return 0;
}

void epd2in9d_sleep(void) {
    sleep_ms(2000);
}

void epd2in9d_wake(void) {
}

void epd2in9d_clear(void) {
    sleep_ms(3500);
}

void epd2in9d_update(const uint8_t *data) {
    (void)data;
    sleep_ms(100);
}

void epd2in9d_display(const uint8_t *data) {
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
