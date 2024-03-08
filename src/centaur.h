// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef CENTAUR_H
#define CENTAUR_H

#include "board.h"

struct CentaurMod;
struct Context;
struct View;

#define MAX_ACTIONS 32

struct Centaur {
    struct Game  *game;
    struct View  *screen_view;
    struct Action actions[MAX_ACTIONS];
    int           num_actions;

    // Current gameplay module
    struct CentaurMod *mod;
};

// There can be only one
extern struct Centaur centaur;

// Shutdown both field array and screen
void centaur_close(void);

// Initialize both field array and screen
int centaur_open(void);

// Put display to sleep
void centaur_sleep(void);

// Wake display from sleep
void centaur_wake(void);

enum Color centaur_player_color(void);
void centaur_set_player_color(enum Color color);

// Read current state of board fields
// MSB: H1=63 G1 F1 ... A1, H2 G2 ... A2, ..., H8 G8 ... A8=0
uint64_t centaur_getstate(void);

// Return battery and charging status
int centaur_batterylevel(void);
int centaur_charging(void);

// Clear display
void centaur_clear(void);

// Render UI to display
void centaur_render(void);

// Wait for pieces to be put in starting position
void centaur_sync(void);

// Play a game!
void centaur_main(void);

// Utility
void sleep_ms(int milliseconds);

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
