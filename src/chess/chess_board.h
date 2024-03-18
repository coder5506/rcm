// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef RCM_CHESS_BOARD_H
#define RCM_CHESS_BOARD_H

static inline bool
color_valid(char color) { return color == 'w' || color == 'b'; }

char color_other(char color);

bool piece_valid(char piece);
char piece_color(char piece);

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
square_valid(int square) { return A8 <= square && square <= H1; }

char square_file(int square);
char square_rank(int square);
int square(char file, char rank);

const char *square_name(int square);
int square_named(const char *name);

char starting_rank(char color);
char last_rank(char color);

bool is_starting_rank(int square, char color);
bool is_last_rank(int square, char color);

// A board is an 8x8 grid of pieces, indexed by square.
struct Board {
    char squares[64];
};

// A mailbox is a 12-row x 10-column grid embedding a board surrounded by
// sentinels.  This representation is convenient for move generation.
struct Mailbox {
    // N.B., a board has "squares", a mailbox has "cells"
    //
    // It is helpful to use "int" for board indices and "int" for
    // mailbox indices, to minimize confusion.
    char cells[120];
};

// We use lookup tables to translate between the two.
extern const int         mailbox_index[64]; // Map board squares to mailbox cells
extern const int board_index[120];  // Map mailbox cells to board squares

void board_print(const struct Board *board);
void board_set_all(struct Board *board, char piece);
void board_empty(struct Board *board);

void mailbox_print(const struct Mailbox *mailbox);
void mailbox_invalid(struct Mailbox *mailbox);
void mailbox_empty(struct Mailbox *mailbox);
void mailbox_copy(struct Mailbox *dst, const struct Mailbox *src);
bool mailbox_equal(const struct Mailbox *a, const struct Mailbox *b);

void board_from_mailbox(struct Board *board, const struct Mailbox *mailbox);
void mailbox_from_board(struct Mailbox *mailbox, const struct Board *board);

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
