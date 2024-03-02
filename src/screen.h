// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef SCREEN_H
#define SCREEN_H

#include "model.h"

#include <stddef.h>
#include <stdint.h>

struct Context;
struct View;

struct Screen {
    struct Model    model;
    struct Image   *image[2];
    struct Context *context;
};

extern struct Screen screen;

// Shutdown display
void screen_close(void);

// Initialize display
int screen_open(void);

// Put display to sleep
void screen_sleep(void);

// Wake display from sleep
void screen_wake(void);

// Return screen's current graphics context
struct Context *screen_context(void);

// Clear display
void screen_clear(void);

// Render UI to display
void screen_render(struct View *view);

int screen_png(uint8_t **png, size_t *size);

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
