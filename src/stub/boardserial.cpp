// Copyright (C) 2024 Eric Sessoms
// See license at end of file

// Stubs out serial communication with DGT Centaur board.  Used for development
// and testing on Linux systems where board is not available.

#include "../boardserial.h"
#include <stdio.h>

// Default: no pieces on board
static uint64_t boardstate = 0llu;

// Hook for testing and debugging
void boardserial_set_boardstate(uint64_t bitmap) {
    printf("boardserial_set_boardstate(%016lx)\n", bitmap);
    boardstate = bitmap;
}

void boardserial_close(void) {
    printf("boardserial_close()\n");
}

int boardserial_open(void) {
    printf("boardserial_open() => 0\n");
    return 0;  // Success
}

int boardserial_chargingstate(void) {
    printf("boardserial_chargingstate() => 20\n");
    return 20;  // Fully charged, not charging
}

uint64_t boardserial_boardstate(void) {
    printf("boardserial_boardstate() => %016lx\n", boardstate);
    return boardstate;
}

int boardserial_readdata(uint8_t *buf, int len) {
    printf("boardserial_readdata(%p, %d) => 6\n", (void*)buf, len);
    return 6;  // Idle
}

void boardserial_buttons(enum Button *press, enum Button *release) {
    printf("boardserial_buttons(%p, %p)\n", (void*)press, (void*)release);
    if (press) {
        *press = BUTTON_NONE;
    }
    if (release) {
        *release = BUTTON_NONE;
    }
}

int boardserial_leds_off(void) {
    printf("boardserial_leds_off() => 0\n");
    return 0;  // Success
}

int boardserial_led_flash(void) {
    printf("boardserial_led_flash() => 0\n");
    return 0;
}

int boardserial_led(int square) {
    printf("boardserial_led(%d) => 0\n", square);
    return 0;
}

int boardserial_led_array(const int *squares, int num_squares) {
    printf("boardserial_led_array(%p, %d) => 0\n", (void*)squares, num_squares);
    return 0;
}

int boardserial_led_from_to(int from, int to) {
    printf("boardserial_led_from_to(%d, %d) => 0\n", from, to);
    return 0;
}

int boardserial_play_sound(enum Sound sound) {
    printf("boardserial_play_sound(%d) => 0\n", sound);
    return 0;
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
