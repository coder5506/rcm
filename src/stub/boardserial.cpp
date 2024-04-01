// Copyright (C) 2024 Eric Sessoms
// See license at end of file

// Stubs out serial communication with DGT Centaur board.  Used for development
// and testing on Linux systems where board is not available.

#include "boardserial.h"
#include <cstdio>

using namespace std;

// Default: no pieces on board
static Bitmap boardstate = 0llu;

BoardSerial::~BoardSerial() {
    printf("boardserial_close()\n");
}

BoardSerial::BoardSerial() {
    printf("boardserial_open() => 0\n");
}

int BoardSerial::chargingstate() {
    printf("boardserial_chargingstate() => 20\n");
    return 20;  // Fully charged, not charging
}

Bitmap BoardSerial::boardstate() {
    printf("boardserial_boardstate() => %016lx\n", ::boardstate);
    return ::boardstate;
}

int BoardSerial::readdata(uint8_t* buf, int len) {
    printf("boardserial_readdata(%p, %d) => 6\n", (void*)buf, len);
    return 6;  // Idle
}

void BoardSerial::buttons(Buttons& press, Buttons& release) {
    printf("boardserial_buttons()\n");
    press   = Buttons();
    release = Buttons();
}

int BoardSerial::leds_off() {
    printf("boardserial_leds_off() => 0\n");
    return 0;  // Success
}

int BoardSerial::led_flash() {
    printf("boardserial_led_flash() => 0\n");
    return 0;
}

int BoardSerial::led(int square) {
    printf("boardserial_led(%d) => 0\n", square);
    return 0;
}

int BoardSerial::led_array(const int* squares, int num_squares) {
    printf("boardserial_led_array(%p, %d) => 0\n", (void*)squares, num_squares);
    return 0;
}

int BoardSerial::led_from_to(int from, int to) {
    printf("boardserial_led_from_to(%d, %d) => 0\n", from, to);
    return 0;
}

int BoardSerial::play_sound(Sound sound) {
    printf("boardserial_play_sound(%d) => 0\n", int(sound));
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
