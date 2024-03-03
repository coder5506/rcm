// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "chess_position.h"
#include "../list.h"

// Game is graph of positions joined by moves
struct Game {
    struct Node history; // Path from start to current position
    struct Node moves;
};

void game_destroy(struct Game *game);
void game_from_position(struct Game *game, const struct Position *start);
void game_from_fen(struct Game *game, const char *fen);

int game_move(struct Game *game, const struct Move *move);
int game_takeback(struct Game *game, const struct Move *takeback);

bool game_read_move(
    struct Node   *candidates,
    struct Move  **takeback,
    struct Game   *game,
    uint64_t       boardstate,
    struct Action *actions,
    int            num_actions);

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
