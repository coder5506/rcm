// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

// API for communication with e-Paper display on DGT Centaur board

#ifndef RCM_EPD2IN9D_H
#define RCM_EPD2IN9D_H

#include <stdint.h>

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 296

// Shutdown display
void epd2in9d_close(void);

// Initialize display
int epd2in9d_open(void);

// Put display to sleep
void epd2in9d_sleep(void);

// Wake display from sleep
void epd2in9d_init(void);

// Partially update display.  That is, instruct the e-Paper to make the minimal
// changes necessary to display the new image.
void epd2in9d_update(const uint8_t *data);

// Fully refresh display.  This is slower and draws more power than partial
// updates, but should be done occassionally to cleanup e-Paper artifacts.
void epd2in9d_display(const uint8_t *data);

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
