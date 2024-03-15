// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef RCM_CHESS_BOARD_H
#define RCM_CHESS_BOARD_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Both colors and pieces are really just characters.  There's no reason for
// anything fancier.  We define these enums as documentation.

enum Color {
    WHITE = 'w',
    BLACK = 'b',
    INVALID_COLOR = -1,
};

enum Piece {
    EMPTY         = ' ',
    WHITE_PAWN    = 'P',
    WHITE_KNIGHT  = 'N',
    WHITE_BISHOP  = 'B',
    WHITE_ROOK    = 'R',
    WHITE_QUEEN   = 'Q',
    WHITE_KING    = 'K',
    BLACK_PAWN    = 'p',
    BLACK_KNIGHT  = 'n',
    BLACK_BISHOP  = 'b',
    BLACK_ROOK    = 'r',
    BLACK_QUEEN   = 'q',
    BLACK_KING    = 'k',
    INVALID_PIECE = -1,
};

static inline bool
color_valid(enum Color color) { return color == 'w' || color == 'b'; }

enum Color color_other(enum Color color);

bool piece_valid(enum Piece piece);
enum Color piece_color(enum Piece piece);

// We enumerate squares to align with the FEN standard.  Conveniently, this
// also matches the field indices used by the Centaur board.
enum Square {
    A8 = 0,
        B8, C8, D8, E8, F8, G8, H8,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A1, B1, C1, D1, E1, F1, G1, H1,
    INVALID_SQUARE = -1,
};

static inline bool
square_valid(enum Square square) { return A8 <= square && square <= H1; }

char square_file(enum Square square);
char square_rank(enum Square square);
enum Square square(char file, char rank);

const char *square_name(enum Square square);
enum Square square_named(const char *name);

char starting_rank(enum Color color);
char last_rank(enum Color color);

bool is_starting_rank(enum Square square, enum Color color);
bool is_last_rank(enum Square square, enum Color color);

// A board is an 8x8 grid of pieces, indexed by square.
struct Board {
    char squares[64];
};

// A mailbox is a 12-row x 10-column grid embedding a board surrounded by
// sentinels.  This representation is convenient for move generation.
struct Mailbox {
    // N.B., a board has "squares", a mailbox has "cells"
    //
    // It is helpful to use "enum Square" for board indices and "int" for
    // mailbox indices, to minimize confusion.
    char cells[120];
};

// We use lookup tables to translate between the two.
extern const int         mailbox_index[64]; // Map board squares to mailbox cells
extern const enum Square board_index[120];  // Map mailbox cells to board squares

void board_print(const struct Board *board);
void board_set_all(struct Board *board, enum Piece piece);
void board_empty(struct Board *board);

void mailbox_print(const struct Mailbox *mailbox);
void mailbox_invalid(struct Mailbox *mailbox);
void mailbox_empty(struct Mailbox *mailbox);
void mailbox_copy(struct Mailbox *dst, const struct Mailbox *src);
bool mailbox_equal(const struct Mailbox *a, const struct Mailbox *b);

void board_from_mailbox(struct Board *board, const struct Mailbox *mailbox);
void mailbox_from_board(struct Mailbox *mailbox, const struct Board *board);

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
