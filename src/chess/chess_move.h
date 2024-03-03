// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include "chess_board.h"
#include "../list.h"

// Moves are the edges of the game graph.  As such, they are listable and can
// point to both their parent position and their resulting position (they are
// directed).
//
// Note that moves are not shared, but positions are.  This means that moves
// are generally not responsible for freeing their positions, but there are
// convenience functions to do so.

struct Position;

// Represent moves in terms of named squares on an 8x8 board
struct Move {
    struct Move     *next;
    struct Move     *prev;
    struct Position *before;
    struct Position *after;
    enum Square      from;
    enum Square      to;
    enum Piece       promotion;
};

void move_free(struct Move *move);

// Also frees after position
void move_free_after(struct Move *move);

struct Move *move_alloc(void);
struct Move *move_new(enum Square from, enum Square to, enum Piece promotion);

struct Move *move_dup(const struct Move *move);
struct Move *move_copy(const struct Move *move);
struct Move *move_copy_after(const struct Move *move);

bool move_validate(enum Square from, enum Square to, enum Piece promotion);
bool move_valid(const struct Move *move);

bool move_equal(const struct Move *a, const struct Move *b);

struct Move *movelist_find_equal(const struct Move *list, const struct Move *move);

// Pure Coordinate Notation
int move_name(char *buf, int len, const struct Move *move);
const char *move_name_static(const struct Move *move);
struct Move *move_named(const char *name);

// Standard Algebraic Notation
int move_san(char *buf, int len, const struct Move *move);
const char *move_san_static(const struct Move *move);
struct Move *move_from_san(struct Position *before, const char *san);

//
// List utilites
//

static inline void movelist_remove(struct Move *move) {
    LIST_REMOVE(move);
}

static inline void movelist_push(struct Move *list, struct Move *move) {
    LIST_PUSH(list, move);
}

static inline struct Move *movelist_pop(struct Move *list) {
    return (struct Move*)LIST_POP(list);
}

static inline struct Move *movelist_shift(struct Move *list) {
    return (struct Move*)LIST_SHIFT(list);
}

static inline bool movelist_empty(const struct Move *list) {
    return LIST_EMPTY(list);
}

static inline int movelist_length(const struct Move *list) {
    return LIST_LENGTH(list);
}

static inline void movelist_free(struct Move *list) {
    LIST_CLEAR(list);
}

static inline void movelist_free_after(struct Move *list) {
    LIST_CLEAR(list);
}

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
