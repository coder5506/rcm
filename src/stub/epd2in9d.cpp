// Copyright (C) 2024 Eric Sessoms
// See license at end of file

// Stubs out communication with e-Paper display on DGT Centaur board.  Used for
// development and testing on Linux systems where board is not available.

#include "../epd2in9d.h"
#include "../utility/sleep.h"

#include <cstdio>

void epd2in9d_close(void) {
    printf("epd2in9d_close()\n");
}

int epd2in9d_open(void) {
    printf("epd2in9d_open() => 0\n");
    return 0;  // Success
}

void epd2in9d_sleep(void) {
    printf("epd2in9d_sleep()\n");
    sleep_ms(2000);  // Simulate delay
}

void epd2in9d_init(void) {
    printf("epd2in9d_init()\n");
}

void epd2in9d_update(const uint8_t *data) {
    printf("epd2in9d_update(%p)\n", (void*)data);
    sleep_ms(100);
}

void epd2in9d_display(const uint8_t *data) {
    printf("epd2in9d_display(%p)\n", (void*)data);
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
