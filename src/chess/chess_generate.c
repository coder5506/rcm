// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_generate.h"
#include "chess.h"
#include "../list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Moves
//

static void
add_promotion(struct List *list, int from, int to, enum Piece promotion) {
    assert(0 <= from && from < 120);
    assert(0 <= to   && to   < 120);
    assert(to != from);

    const enum Square from_square = board_index[from];
    const enum Square to_square   = board_index[to];

    // Validate squares
    assert(square_valid(from_square));
    assert(square_valid(to_square));

    // Promote to knight, bishop, rook, or queen
    assert(strchr(" NBRQnbrq", promotion));

    // Promote on last rank only
    assert(promotion == ' ' || is_last_rank(to_square, piece_color(promotion)));

    // Add move to list
    list_push(list, move_new(from_square, to_square, promotion));
}

static void add_move(struct List *list, int from, int to) {
    add_promotion(list, from, to, ' ');
}

static void
add_pawn_moves(struct List *list, int from, int to, enum Color turn) {
    if (!is_last_rank(board_index[to], turn)) {
        add_move(list, from, to);
        return;
    }

    const char *promotions = turn == 'w' ? "NBRQ" : "nbrq";
    for (; *promotions; ++promotions) {
        add_promotion(list, from, to, *promotions);
    }
}

//
// Pawns
//

static void
pawn_attacks(struct List *list, const struct Mailbox *mailbox, enum Color turn)
{
    const enum Piece pawn      = turn == 'w' ? 'P' : 'p';
    const enum Color other     = color_other(turn);
    const int        direction = pawn == 'P' ? -10 : 10;

    const char *cells = mailbox->cells;
    for (enum Square sq = A7; sq <= H2; ++sq) {
        const int from = mailbox_index[sq];
        if (cells[from] != (char)pawn) {
            continue;
        }

        const int advance = from + direction;

        const enum Piece target_1 = cells[advance - 1];
        if (target_1 == ' ' || piece_color(target_1) == other) {
            add_move(list, from, advance - 1);
        }

        const enum Piece target_2 = cells[advance + 1];
        if (target_2 == ' ' || piece_color(target_2) == other) {
            add_move(list, from, advance + 1);
        }
    }
}

static void
pawn_moves(struct List *list, const struct Position *position) {
    const enum Piece pawn      = position->turn == 'w' ? 'P' : 'p';
    const enum Color other     = color_other(position->turn);
    const int        direction = pawn == 'P' ? -10 : 10;

    const char *cells = position->mailbox.cells;
    for (enum Square sq = A7; sq <= H2; ++sq) {
        const int from = mailbox_index[sq];
        if (cells[from] != (char)pawn) {
            continue;
        }

        const int advance = from + direction;

        // Advance
        if (cells[advance] == ' ') {
            add_pawn_moves(list, from, advance, position->turn);
            if (is_starting_rank(board_index[from], position->turn)) {
                if (cells[advance + direction] == ' ') {
                    add_move(list, from, advance + direction);
                }
            }
        }

        // Capture
        if (piece_color(cells[advance - 1]) == other) {
            add_pawn_moves(list, from, advance - 1, position->turn);
        }
        if (piece_color(cells[advance + 1]) == other) {
            add_pawn_moves(list, from, advance + 1, position->turn);
        }

        // En passant
        if (position->en_passant != INVALID_SQUARE) {
            if (board_index[advance - 1] == position->en_passant) {
                add_pawn_moves(list, from, advance - 1, position->turn);
            }
            if (board_index[advance + 1] == position->en_passant) {
                add_pawn_moves(list, from, advance + 1, position->turn);
            }
        }
    }
}

//
// Pieces
//

// How pieces move in mailbox representation
static const int *move_offsets(enum Piece piece) {
    static const int offsets[6][9] = {
        {   0,   0,   0,   0,  0,  0,  0,  0, 0, }, // Pawn
        { -21, -19, -12,  -8,  8, 12, 19, 21, 0, }, // Knight
        { -11,  -9,   9,  11,  0,  0,  0,  0, 0, }, // Bishop
        { -10,  -1,   1,  10,  0,  0,  0,  0, 0, }, // Rook
        { -11, -10,  -9,  -1,  1,  9, 10, 11, 0, }, // Queen
        { -11, -10,  -9,  -1,  1,  9, 10, 11, 0, }, // King
    };

    switch (piece) {
    case 'P': case 'p':
        return offsets[0];
    case 'N': case 'n':
        return offsets[1];
    case 'B': case 'b':
        return offsets[2];
    case 'R': case 'r':
        return offsets[3];
    case 'Q': case 'q':
        return offsets[4];
    case 'K': case 'k':
        return offsets[5];
    default:
        return NULL;
    }
}

// Sliding pieces move along a vector
static bool is_sliding(enum Piece piece) {
    assert(piece_valid(piece));
    switch (piece) {
    case 'B': case 'R': case 'Q':
    case 'b': case 'r': case 'q':
        return true;
    default:
        return false;
    }
}

static void
piece_attacks(struct List *list, const struct Mailbox *mailbox, enum Color turn)
{
    const enum Color other = color_other(turn);

    const char *cells = mailbox->cells;
    for (enum Square sq = A8; sq <= H1; ++sq) {
        const int        from  = mailbox_index[sq];
        const enum Piece piece = cells[from];
        if (piece_color(piece) != turn) {
            continue;
        }

        const int *offsets = move_offsets(piece);
        const bool sliding = is_sliding(piece);

        // Iterate over possible vectors
        for (; *offsets; ++offsets) {
            // Iterate steps along vector
            for (int to = from + *offsets; ; to += *offsets) {
                const enum Piece target = cells[to];
                if (target == INVALID_PIECE) {
                    // Sentinel
                    break;
                }
                if (target != ' ') {
                    if (piece_color(target) == other) {
                        // Capture
                        add_move(list, from, to);
                    }
                    // Occupied
                    break;
                }
                // Move
                add_move(list, from, to);
                if (!sliding) {
                    break;
                }
            }
        }
    }
}

static void
piece_moves(struct List *list, const struct Position *position) {
    piece_attacks(list, &position->mailbox, position->turn);
}

//
// Attacks
//

static void
attacks_list(struct List *list, const struct Mailbox *mailbox, enum Color turn)
{
    pawn_attacks(list, mailbox, turn);
    piece_attacks(list, mailbox, turn);
}

static void
attacked_squares(
    struct Board         *board,
    const struct Mailbox *mailbox,
    enum Color            turn)
{
    // INVALID_PIECE is not attacked
    board_set_all(board, INVALID_PIECE);

    struct List *list = list_new();
    attacks_list(list, mailbox, turn);

    for (struct List *it = list->next; it != list; it = it->next) {
        const struct Move *move = it->data;
        board->squares[move->to] = mailbox->cells[mailbox_index[move->to]];
    }
}

//
// Castling
//

void generate_castle_moves(struct List *list, const struct Position *position) {
    struct Board board;
    attacked_squares(&board, &position->mailbox, color_other(position->turn));

    const char *attacks = board.squares;
    const char *cells   = position->mailbox.cells;

    if (position->turn == 'w' && attacks[E1] != 'K') {
        const int e1 = mailbox_index[E1];
        if (position->castle & K_CASTLE) {
            const int f1 = mailbox_index[F1];
            const int g1 = mailbox_index[G1];
            // i.e., square is empty and not attacked
            if (cells[f1] == ' ' && attacks[F1] != ' ' &&
                cells[g1] == ' ' && attacks[G1] != ' ')
            {
                add_move(list, e1, g1);
            }
        }
        if (position->castle & Q_CASTLE) {
            const int d1 = mailbox_index[D1];
            const int c1 = mailbox_index[C1];
            const int b1 = mailbox_index[B1];
            if (cells[d1] == ' ' && attacks[D1] != ' ' &&
                cells[c1] == ' ' && attacks[C1] != ' ' &&
                cells[b1] == ' ')
            {
                add_move(list, e1, c1);
            }
        }
        return;
    }

    if (position->turn == 'b' && attacks[E8] != 'k') {
        const int e8 = mailbox_index[E8];
        if (position->castle & k_CASTLE) {
            const int f8 = mailbox_index[F8];
            const int g8 = mailbox_index[G8];
            if (cells[f8] == ' ' && attacks[F8] != ' ' &&
                cells[g8] == ' ' && attacks[G8] != ' ')
            {
                add_move(list, e8, g8);
            }
        }
        if (position->castle & q_CASTLE) {
            const int d8 = mailbox_index[D8];
            const int c8 = mailbox_index[C8];
            const int b8 = mailbox_index[B8];
            if (cells[d8] == ' ' && attacks[D8] != ' ' &&
                cells[c8] == ' ' && attacks[C8] != ' ' &&
                cells[b8] == ' ')
            {
                add_move(list, e8, c8);
            }
        }
    }
}

//
// Evaluation
//

// All possible moves, legal or otherwise
static void
candidate_moves(struct List *list, const struct Position *position) {
    pawn_moves(list, position);
    piece_moves(list, position);
    generate_castle_moves(list, position);
}

static bool in_check(const struct Mailbox *mailbox, enum Color turn) {
    // Find all squares attacked by opponent
    struct Board attacks;
    attacked_squares(&attacks, mailbox, color_other(turn));

    // Is our king under attack?
    const enum Piece king = turn == 'w' ? 'K' : 'k';
    for (enum Square sq = A8; sq <= H1; ++sq) {
        if (attacks.squares[sq] == (char)king) {
            return true;
        }
    }
    return false;
}

bool position_legal(const struct Position *position) {
    return !in_check(&position->mailbox, color_other(position->turn));
}

void generate_legal_moves(struct List *list, const struct Position *before) {
    struct List *candidates = list_new();
    candidate_moves(candidates, before);

    for (struct List *it = candidates->next; it != candidates; it = it->next) {
        struct Move     *move  = it->data;
        struct Position *after = position_apply_move(before, move);
        if (position_legal(after)) {
            move->before = before;
            move->after  = after;
            list_push(list, move);
        }
    }
}

struct List *position_legal_moves(const struct Position *before) {
    if (!before->legal_moves) {
        struct Position *mutable = (struct Position *)before;
        mutable->legal_moves = list_new();
        generate_legal_moves(mutable->legal_moves, before);
    }
    return before->legal_moves;
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
