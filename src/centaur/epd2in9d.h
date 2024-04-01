// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

// API for communication with e-Paper display on DGT Centaur board

#ifndef EPD2IN9D_H
#define EPD2IN9D_H

#include <cstdint>

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 296

// For RAII
class Gpio {
public:
    ~Gpio();
    Gpio();
};

class Epd2in9d {
    Gpio gpio;
    bool lut_ready{false};

public:
    // Shutdown display
    ~Epd2in9d() noexcept;

    // Connect to display
    Epd2in9d();

    // Put display to sleep
    void sleep();

    // Initialize display
    void init();

    // Fully refresh display.  This is slower and draws more power than partial
    // updates, but should be done occassionally to cleanup e-Paper artifacts.
    void display(const std::uint8_t* data);

    // Partially update display.  That is, instruct the e-Paper to make the
    // minimal changes necessary to display the new image.
    void update(const std::uint8_t* data);

private:
    void init_lut();
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
