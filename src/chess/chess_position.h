// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef RCM_CHESS_POSITION_H
#define RCM_CHESS_POSITION_H

#include "chess_board.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct List;
struct Move;

// Flags
enum Castle {
    K_CASTLE = 1 << 3, // White kingside
    Q_CASTLE = 1 << 2, // White queenside
    k_CASTLE = 1 << 1, // Black kingside
    q_CASTLE = 1 << 0, // Black queenside
};

struct Position {
    struct Mailbox mailbox;
    enum Color     turn;
    enum Castle    castle;
    enum Square    en_passant;
    int            halfmove;
    int            fullmove;
    uint64_t       bitmap;
    uint64_t       white_bitmap;
    struct List   *moves_played;
    struct List   *legal_moves;
};

bool position_valid(const struct Position *position);

struct Position *position_new(void);
struct Position *position_dup(const struct Position *position);
struct Position *position_from_fen(const char *fen);

enum Piece position_piece(const struct Position *position, enum Square square);
bool position_equal(const struct Position *a, const struct Position *b);

static inline void position_print(const struct Position *position) {
    mailbox_print(&position->mailbox);
}

struct Position*
position_apply_move(const struct Position *before, const struct Move *move);

// True if boardstate might represent a transition into this position
bool position_incomplete(const struct Position *position, uint64_t boardstate);

// Action can be either lift or place
struct Action {
    enum Square lift;
    enum Square place;
};

#define EMPTY_ACTION (struct Action){.lift = -1, .place = -1}

bool position_read_move(
    struct List           *candidates,
    const struct Position *before,
    uint64_t               boardstate,
    struct Action         *actions,
    int                    num_actions);

#ifdef __cplusplus
}
#endif

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
