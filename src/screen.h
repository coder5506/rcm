// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef SCREEN_H
#define SCREEN_H

#include "utility/model.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <pthread.h>

struct Context;
struct View;

struct Screen {
    struct Model    model;
    struct Image   *image[2];
    struct Context *context;
    pthread_cond_t  cond;
    pthread_mutex_t mutex;
    pthread_t       thread;
    bool            shutdown;
};

extern struct Screen screen;

// Shutdown display
void screen_close(void);

// Initialize display
int screen_open(void);

// Render UI to display
void screen_render(struct View *view);

// Get PNG image of display
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
