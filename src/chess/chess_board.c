// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_board.h"
#include "chess.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

//
// Pieces
//

char color_other(char color) {
    assert(color_valid(color));
    return color == 'w' ? 'b' : 'w';
}

char piece_color(char piece) {
    switch (piece) {
    case 'P': case 'N': case 'B': case 'R': case 'Q': case 'K':
        return 'w';
    case 'p': case 'n': case 'b': case 'r': case 'q': case 'k':
        return 'b';
    default:
        return -1;
    }
}

bool piece_valid(char piece) {
    return color_valid(piece_color(piece));
}

//
// Squares
//

char square_file(int square) {
    assert(square_valid(square));
    const char file = 'a' + (square % 8);
    assert('a' <= file && file <= 'h');
    return file;
}

char square_rank(int square) {
    assert(square_valid(square));
    const char rank = '8' - (square / 8);
    assert('1' <= rank && rank <= '8');
    return rank;
}

int square(char file, char rank) {
    assert('a' <= file && file <= 'h');
    assert('1' <= rank && rank <= '8');

    const int sq = ('8' - rank) * 8 + (file - 'a');
    assert(square_valid(sq));
    assert(square_file(sq) == file);
    assert(square_rank(sq) == rank);

    return sq;
}

const char *square_name(int square) {
    static const char *names[] = {
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    };

    assert(square_valid(square));
    return names[square];
}

int square_named(const char *name) {
    assert(name);
    return square(name[0], name[1]);
}

char starting_rank(char color) {
    assert(color_valid(color));
    return color == 'w' ? '2' : '7';
}

char last_rank(char color) {
    assert(color_valid(color));
    return color == 'w' ? '8' : '1';
}

bool is_starting_rank(int square, char color) {
    return square_rank(square) == starting_rank(color);
}

bool is_last_rank(int square, char color) {
    return square_rank(square) == last_rank(color);
}

//
// Boards
//

// Map board squares to mailbox cells
const int mailbox_index[64] = {
    21, 22, 23, 24, 25, 26, 27, 28,
    31, 32, 33, 34, 35, 36, 37, 38,
    41, 42, 43, 44, 45, 46, 47, 48,
    51, 52, 53, 54, 55, 56, 57, 58,
    61, 62, 63, 64, 65, 66, 67, 68,
    71, 72, 73, 74, 75, 76, 77, 78,
    81, 82, 83, 84, 85, 86, 87, 88,
    91, 92, 93, 94, 95, 96, 97, 98,
};

// Map mailbox cells to board squares
//
// Only one sentinel is needed on the left and right edges because
// columns "wrap".  Move offsets are always scalar values.
const int board_index[120] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, A8, B8, C8, D8, E8, F8, G8, H8, -1,
    -1, A7, B7, C7, D7, E7, F7, G7, H7, -1,
    -1, A6, B6, C6, D6, E6, F6, G6, H6, -1,
    -1, A5, B5, C5, D5, E5, F5, G5, H5, -1,
    -1, A4, B4, C4, D4, E4, F4, G4, H4, -1,
    -1, A3, B3, C3, D3, E3, F3, G3, H3, -1,
    -1, A2, B2, C2, D2, E2, F2, G2, H2, -1,
    -1, A1, B1, C1, D1, E1, F1, G1, H1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

void board_print(const struct Board *board) {
    assert(board);
    int sq = A8;
    for (char r = '8'; r >= '1'; --r) {
        for (char f = 'a'; f <= 'h'; ++f) {
            printf(" %c", board->squares[sq++]);
        }
        printf("\n");
    }
}

void board_set_all(struct Board *board, char piece) {
    assert(board);
    memset(board, piece, sizeof *board);
}

void board_empty(struct Board *board) {
    board_set_all(board, ' ');
}

void mailbox_print(const struct Mailbox *mailbox) {
    assert(mailbox);
    int sq = A8;
    for (char r = '8'; r >= '1'; --r) {
        for (char f = 'a'; f <= 'h'; ++f) {
            printf(" %c", mailbox->cells[mailbox_index[sq++]]);
        }
        printf("\n");
    }
}

void mailbox_invalid(struct Mailbox *mailbox) {
    assert(mailbox);
    memset(mailbox, -1, sizeof *mailbox);
}

void mailbox_empty(struct Mailbox *mailbox) {
    mailbox_invalid(mailbox);
    for (int sq = A8; sq <= H1; ++sq) {
        mailbox->cells[mailbox_index[sq]] = ' ';
    }
}

void mailbox_copy(struct Mailbox *dst, const struct Mailbox *src) {
    assert(dst && src);
    memcpy(dst, src, sizeof *dst);
}

bool mailbox_equal(const struct Mailbox *a, const struct Mailbox *b) {
    assert(a && b);
    return memcmp(a, b, sizeof *a) == 0;
}


void board_from_mailbox(struct Board *board, const struct Mailbox *mailbox) {
    assert(board && mailbox);
    for (int sq = A8; sq <= H1; ++sq) {
        board->squares[sq] = mailbox->cells[mailbox_index[sq]];
    }
}

void mailbox_from_board(struct Mailbox *mailbox, const struct Board *board) {
    assert(mailbox && board);
    mailbox_invalid(mailbox);
    for (int sq = A8; sq <= H1; ++sq) {
        mailbox->cells[mailbox_index[sq]] = board->squares[sq];
    }
}

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
