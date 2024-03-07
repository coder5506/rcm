// Copyright (C) 2024 Eric Sessoms
// See license at end of file

// API for serial communication with field array on DGT Centaur board

#ifndef BOARDSERIAL_H
#define BOARDSERIAL_H

#include <stdint.h>

// Shutdown serial connection to board
void boardserial_close(void);

// Initialize serial connection to board
int boardserial_open(void);

// Read field events from board
int boardserial_readdata(uint8_t *buf, int len);

// Read current state of board fields.  Returns bitmap where set bit
// indicates presence of piece.
// MSB: H1=63 G1 F1 ... A1, H2 G2 ... A2, ..., H8 G8 ... A8=0
uint64_t boardserial_boardstate(void);

// Return battery and charging status
int boardserial_chargingstate(void);

int boardserial_leds_off(void);
int boardserial_led_flash(void);
int boardserial_led(int square);
int boardserial_led_array(const int *squares, int num_squares);
int boardserial_led_from_to(int from, int to);

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
