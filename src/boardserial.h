// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

// API for serial communication with DGT Centaur board

#ifndef RCM_BOARDSERIAL_H
#define RCM_BOARDSERIAL_H

#include <stdint.h>

enum Button {
    BUTTON_NONE = 0,
    BUTTON_BACK = 1 << 0,
    BUTTON_DOWN = 1 << 1,
    BUTTON_PLAY = 1 << 2,
    BUTTON_UP   = 1 << 3,
    BUTTON_TICK = 1 << 4,
    BUTTON_HELP = 1 << 6, // no, I don't know where 5 went
    BUTTON_MASK = 0x5F,
};

enum Sound {
    SOUND_GENERAL    = 0,
    SOUND_FACTORY    = 1,
    SOUND_POWER_OFF  = 2,
    SOUND_POWER_ON   = 3,
    SOUND_WRONG      = 4,
    SOUND_WRONG_MOVE = 5,
    SOUND_NONE       = 6,
};

// Shutdown serial connection to board
void boardserial_close(void);

// Initialize serial connection to board
int boardserial_open(void);

// Return battery and charging status
int boardserial_chargingstate(void);

// Read current state of board fields.  Returns bitmap where set bit indicates
// presence of piece.
// MSB: H1=63 G1 F1 ... A1, H2 G2 ... A2, ..., H8 G8 ... A8=0
uint64_t boardserial_boardstate(void);

// Read field events from board
int boardserial_readdata(uint8_t *buf, int len);

// Read button events from board
void boardserial_buttons(enum Button *press, enum Button *release);

int boardserial_leds_off(void);
int boardserial_led_flash(void);
int boardserial_led(int square);
int boardserial_led_array(const int *squares, int num_squares);
int boardserial_led_from_to(int from, int to);

int boardserial_play_sound(enum Sound sound);

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
