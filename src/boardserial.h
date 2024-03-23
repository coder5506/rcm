// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

// API for serial communication with DGT Centaur board

#ifndef BOARDSERIAL_H
#define BOARDSERIAL_H

#include <array>
#include <bitset>
#include <cstdint>

using Bitmap = std::uint64_t;

enum class Button {
    BACK = 0,
    DOWN = 1,
    PLAY = 2,
    UP   = 3,
    TICK = 4,
    HELP = 6,  // no, I don't know where 5 went
};

class Buttons {
    std::bitset<7> buttons;
public:
    Buttons(unsigned long value = 0) noexcept : buttons(value) {}
};

enum class Sound {
    GENERAL    = 0,
    FACTORY    = 1,
    POWER_OFF  = 2,
    POWER_ON   = 3,
    WRONG      = 4,
    WRONG_MOVE = 5,
    NONE       = 6,
};

class BoardSerial {
    int fd{-1};
    std::array<std::uint8_t, 2> addr{0, 0};

public:
    // Shutdown serial connection to board
    ~BoardSerial() noexcept;

    // Initialize serial connection to board
    BoardSerial();

    // Return battery and charging status
    int chargingstate();

    // Read current state of board fields.  Returns bitmap where set bit
    // indicates presence of piece.
    // MSB: H1=63 G1 F1 ... A1, H2 G2 ... A2, ..., H8 G8 ... A8=0
    Bitmap boardstate();

    // Read field events from board
    int readdata(std::uint8_t* buf, int len);

    // Read button events from board
    void buttons(Buttons& press, Buttons& release);

    int leds_off();
    int led_flash();
    int led(int square);
    int led_array(const int* squares, int num_squares);
    int led_from_to(int from, int to);

    int play_sound(Sound sound);

private:
    void read_address();

    void build_packet(std::uint8_t* buf, int addr_pos, int len);
    int write_board(std::uint8_t* buf, int addr_pos, int len);
};

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
