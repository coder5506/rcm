// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "chess_position.h"

struct List;

// Game is graph of positions joined by moves
struct Game {
    struct List *history; // Path from start to current position
};

struct Position *game_position(struct Game *game, int index);
struct Position *game_start(struct Game *game);
struct Position *game_current(struct Game *game);
struct Position *game_previous(struct Game *game);

void game_from_position(struct Game *game, const struct Position *start);
void game_from_fen(struct Game *game, const char *fen);

struct List *game_legal_moves(const struct Game *game);

int game_apply_move(struct Game *game, const struct Move *move);
int game_apply_takeback(struct Game *game, const struct Move *takeback);

int game_apply_move_name(struct Game *game, const char *name);
int game_apply_move_san(struct Game *game, const char *san);

bool game_read_move(
    struct List   *candidates,
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
