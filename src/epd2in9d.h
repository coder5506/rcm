// Copyright (c) 2024 Eric Sessoms
// See license at end of file

// Defines API for communication with e-Paper display on DGT Centaur board

#ifndef EPD2IN9D_H
#define EPD2IN9D_H

// Display dimensions
extern const int SCREEN_WIDTH;  // 128
extern const int SCREEN_HEIGHT; // 296

// Shutdown display
void epd2in9d_close(void);

// Initialize display
int epd2in9d_open(void);

// Put display to sleep
void epd2in9d_sleep(void);

// Wake display from sleep
void epd2in9d_wake(void);

// Clear display
void epd2in9d_clear(void);

// Partially update display.  That is, instruct the e-Paper to make the minimal
// changes necessary to display the new image.
void epd2in9d_update(const unsigned char *data);

// Fully refresh display.  This is slower and draws more power than partial
// updates, but should be done occassionally to cleanup e-Paper artifacts.
void epd2in9d_display(const unsigned char *data);

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
