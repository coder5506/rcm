// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include "chess_board.h"

struct List;
struct Position;

// Represent moves in terms of named squares on an 8x8 board
struct Move {
    enum Square from;
    enum Square to;
    enum Piece  promotion;
    const struct Position *before;
    const struct Position *after;
};

bool move_validate(enum Square from, enum Square to, enum Piece promotion);
bool move_valid(const struct Move *move);
struct Move *move_new(enum Square from, enum Square to, enum Piece promotion);

bool move_equal(const struct Move *a, const struct Move *b);
struct Move *movelist_find_equal(const struct List *list, const struct Move *move);

// Pure Coordinate Notation
int move_name(char *buf, int len, const struct Move *move);
const char *move_name_static(const struct Move *move);
struct Move *move_named(const char *name);

// Standard Algebraic Notation
int move_san(char *buf, int len, const struct Move *move);
const char *move_san_static(const struct Move *move);
struct Move *move_from_san(struct Position *before, const char *san);

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
