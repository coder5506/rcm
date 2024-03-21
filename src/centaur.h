// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef CENTAUR_H
#define CENTAUR_H

#include "board.h"

#include <vector>

struct Context;
struct View;

struct Centaur {
    Game *game;
    View *screen_view;
    std::vector<Action> actions;
};

extern Centaur centaur;

void centaur_close();
int centaur_open();

void centaur_render();
void centaur_set_game(Game* game);

int centaur_batterylevel();
int centaur_charging();
void centaur_led(thc::Square square);
void centaur_led_from_to(thc::Square from, thc::Square to);

// Read current state of board fields
// MSB: H1=63 G1 F1 ... A1, H2 G2 ... A2, ..., H8 G8 ... A8=0
uint64_t centaur_getstate();

void centaur_clear_actions();
int centaur_update_actions();
void centaur_purge_actions();

bool
centaur_read_move(
    Game&                     game,
    std::uint64_t             boardstate,
    std::vector<thc::Move>&   candidates,
    std::optional<thc::Move>& takeback);

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
