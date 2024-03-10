// Copyright (C) 2024 Eric Sessoms
// See license at end of file

// This file stubs out serial communication with the field array on a
// DGT Centaur board.  It is used for development and testing on Linux
// systems where the board is not available.

#include "../boardserial.h"

void boardserial_close(void) {
}

int boardserial_open(void) {
    return 0; // Success
}

int boardserial_chargingstate(void) {
    return 20; // Fully charged, not charging
}

uint64_t boardserial_boardstate(void) {
    return 0; // No pieces on board
}

int boardserial_readdata(uint8_t *buf, int len) {
    (void)buf;
    (void)len;
    return 6; // Idle
}

void boardserial_buttons(enum Button *press, enum Button *release) {
    if (press) {
        *press = BUTTON_NONE;
    }
    if (release) {
        *release = BUTTON_NONE;
    }
}

int boardserial_leds_off(void) {
    return 0; // Success
}

int boardserial_led_flash(void) {
    return 0;
}

int boardserial_led(int square) {
    (void)square;
    return 0;
}

int boardserial_led_array(const int *squares, int num_squares) {
    (void)squares;
    (void)num_squares;
    return 0;
}

int boardserial_led_from_to(int from, int to) {
    (void)from;
    (void)to;
    return 0;
}

int boardserial_play_sound(enum Sound sound) {
    (void)sound;
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
