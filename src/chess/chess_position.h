// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef CHESS_POSITION_H
#define CHESS_POSITION_H

#include "chess_move.h"

#include <stdint.h>

// Positions are the nodes of the game graph.  As such, they are listable and
// hold a list of their out-egdes (i.e., moves_played).
//
// As moves are not shared, positions are generally responsible for freeing
// their moves.  The positions themselves are owned by the game.

struct Move;

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
    struct Move      legal_moves;
    struct Mailbox   mailbox;
    uint64_t         bitmap;
    uint64_t         white_bitmap;
    enum Color       turn;
    enum Castle      castle;
    enum Square      en_passant;
    int              halfmove;
    int              fullmove;
};

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

// True if boardstate might represent a transition into this position
bool position_incomplete(struct Position *position, uint64_t boardstate);

// Action can be either lift or place
struct Action {
    enum Square lift;
    enum Square place;
};

#define EMPTY_ACTION (struct Action){.lift = -1, .place = -1}

bool position_read_move(
    struct Move     *candidates,
    struct Position *position,
    uint64_t         boardstate,
    struct Action   *actions,
    int              num_actions);

//
// List utilities
//

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
