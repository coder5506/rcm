// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "../model.h"

#include <stdbool.h>
#include <stdint.h>

#include <time.h>

struct Action;
struct Move;
struct Position;

// Seven Tag Roster (STR)
struct Roster {
    const char *event;   // "?"
    const char *site;    // "?"
    const char *date;    // "YYYY.MM.DD"
    const char *round;   // "-"
    const char *white;   // "?"
    const char *black;   // "?"
    const char *result;  // "*"
};

// Game is graph of positions joined by moves
struct Game {
    struct Model  model;
    struct Roster roster;
    struct List  *history;  // Path from start to current position
    time_t        started;  // Timestamp, serves as ID
};

bool game_valid(const struct Game *game);

struct Position *game_position(struct Game *game, int index);
const struct Position *game_start(const struct Game *game);
struct Position *game_current(struct Game *game);
struct Position *game_previous(struct Game *game);

bool game_started(const struct Game *game);
void game_set_start(struct Game *game, const struct Position *start);

struct Game *game_from_position(const struct Position *start);
struct Game *game_from_fen(const char *fen);

// Not a dup, does not share history
struct Game *game_fork(const struct Game *game);

struct List *game_legal_moves(const struct Game *game);

int game_takeback(struct Game *game);

int game_apply_move(struct Game *game, const struct Move *move);
int game_apply_takeback(struct Game *game, const struct Move *takeback);

int game_apply_move_name(struct Game *game, const char *name);
int game_move_san(struct Game *game, const char *san);

int
game_complete_move(
    struct Game       *game,
    const struct Move *takeback,
    const struct Move *move);

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
