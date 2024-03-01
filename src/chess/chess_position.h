// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef CHESS_POSITION_H
#define CHESS_POSITION_H

#include "chess_board.h"
#include "../list.h"

#include <stddef.h>
#include <stdint.h>

// Before we can define positions, we need to put them in context.  A game
// is a directed graph of positions joined by moves.  This allows for both
// variations (a position may be continued by multiple moves) and
// transpositions (a position may be reached by multiple move orders).
//
// Representing the graph externally proves a bit cumbersome, so both moves
// and positions embed the graph.  Positions (i.e., nodes) have move lists
// and moves (i.e., edges) have resulting positions.
//
// As nodes are shared, moves do not "own" their positions.  All positions
// are owned only by the game.  On the other hand, edges are not shared, so
// positions do own their moves.

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

// Flags
enum Castle {
    K_CASTLE = 1 << 3, // White kingside
    Q_CASTLE = 1 << 2, // White queenside
    k_CASTLE = 1 << 1, // Black kingside
    q_CASTLE = 1 << 0, // Black queenside
};

struct Position {
    struct Position *next;
    struct Position *prev;
    struct Move      moves_played;
    struct Mailbox   mailbox;
    uint64_t         bitmap;
    uint64_t         white_bitmap;
    enum Color       turn;
    enum Castle      castle;
    enum Square      en_passant;
    int              halfmove;
    int              fullmove;
};

void move_free(struct Move *move);

struct Move *move_alloc(void);
struct Move *move_new(enum Square from, enum Square to, enum Piece promotion);
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
struct Move *move_from_san(const struct Position *position, const char *san);

void position_free(struct Position *position);
struct Position *position_alloc(void);
struct Position *position_from_fen(const char *fen);
struct Position *position_copy(const struct Position *position);

enum Piece position_piece(const struct Position *position, enum Square square);
void position_update_bitmap(struct Position *position);

bool position_equal(const struct Position *a, const struct Position *b);

static inline void position_print(const struct Position *position) {
    mailbox_print(&position->mailbox);
}

//
// List wrappers
//

static inline void
movelist_remove(struct Move *move) { list_remove((struct Node*)move); }

static inline void movelist_push(struct Move *list, struct Move *move) {
    list_push((struct Node*)list, (struct Node*)move);
}

static inline struct Move *movelist_pop(struct Move *list) {
    return (struct Move*)list_pop((struct Node*)list);
}

static inline struct Move *movelist_shift(struct Move *list) {
    return (struct Move*)list_shift((struct Node*)list);
}

static inline bool movelist_empty(const struct Move *move) {
    return list_empty((struct Node*)move);
}

static inline int
movelist_length(const struct Move *move) {
    return list_length((struct Node*)move);
}

static inline void movelist_free(struct Move *list) {
    list_free((struct Node*)list, (void(*)(struct Node*))move_free);
}


static inline void
positionlist_push(struct Position *list, struct Position *position) {
    list_push((struct Node*)list, (struct Node*)position);
}

static inline struct Position *positionlist_pop(struct Position *list) {
    return (struct Position*)list_pop((struct Node*)list);
}

static inline struct Position *positionlist_shift(struct Position *list) {
    return (struct Position*)list_shift((struct Node*)list);
}

static inline void
positionlist_clear(struct Position *list) { list_clear((struct Node*)list); }

static inline bool positionlist_empty(const struct Position *position) {
    return list_empty((struct Node*)position);
}

static inline struct Position*
positionlist_find(const struct Position *list, const struct Position *position) {
    return (struct Position*)list_find((struct Node*)list, (struct Node*)position);
}

static inline void positionlist_free(struct Position *list) {
    list_free((struct Node*)list, (void(*)(struct Node*))position_free);
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
