// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_move.h"
#include "chess.h"
#include "../list.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gc/gc.h>

bool move_validate(enum Square from, enum Square to, enum Piece promotion) {
    if (from == to || !square_valid(from) || !square_valid(to)) {
        return false;
    }
    if (!promotion || !strchr(" NBRQnbrq", promotion)) {
        return false;
    }
    return promotion == ' ' || is_last_rank(to, piece_color(promotion));
}

bool move_valid(const struct Move *move) {
    assert(move);
    return move_validate(move->from, move->to, move->promotion);
}

struct Move *move_new(enum Square from, enum Square to, enum Piece promotion) {
    assert(move_validate(from, to, promotion));
    struct Move *move = GC_MALLOC(sizeof *move);
    *move = (struct Move){
        .from      = from,
        .to        = to,
        .promotion = promotion,
        .before    = NULL,
        .after     = NULL,
    };
    assert(move_valid(move));
    return move;
}

bool move_equal(const struct Move *a, const struct Move *b) {
    assert(move_valid(a));
    assert(move_valid(b));
    return a->from == b->from && a->to == b->to && a->promotion == b->promotion;
}

struct Move*
movelist_find_equal(const struct List *list, const struct Move *move) {
    assert(list);
    assert(move_valid(move));

    for (struct List *each = list->next; each != list; each = each->next) {
        if (move_equal(each->data, move)) {
            return each->data;
        }
    }
    return NULL;
}

// N.B., promotion is lowercase because unambiguous
int move_name(char *buf, int len, const struct Move *move) {
    assert(buf && len >= 6); // e.g., "e7e8q\0"
    assert(move_valid(move));
    return snprintf(buf, len, "%s%s%c",
        square_name(move->from),
        square_name(move->to),
        move->promotion == EMPTY ? '\0' : tolower(move->promotion));
}

const char *move_name_static(const struct Move *move) {
    static char buf[6];
    return move_name(buf, sizeof buf, move) > 0 ? buf : NULL;
}

struct Move *move_named(const char *name) {
    assert(name);
    if (strlen(name) < 4) {
        return NULL;
    }

    const enum Square from = square_named(name + 0);
    const enum Square to   = square_named(name + 2);

    enum Piece promotion = name[4] || EMPTY;
    if (promotion != EMPTY) {
        switch (square_rank(to)) {
        case '8':
            promotion = toupper(promotion);
            break;
        case '1':
            promotion = tolower(promotion);
            break;
        default:
            return NULL;
        }
    }

    return move_new(from, to, promotion);
}

// Standard Algebraic Notation
// TODO check and mate?
int move_san(char *buf, int len, const struct Move *move) {
    assert(buf && len >= 0);
    assert(move_valid(move));

    if (len < 7) {
        // e.g., "dxe8=Q\0"
        return 0;
    }
    if (!move->before) {
        // Can't construct SAN without position
        return 0;
    }

    const enum Piece piece = position_piece(move->before, move->from);
    if (piece_color(piece) != move->before->turn) {
        // Not a valid move
        return 0;
    }

    // Handle castling as a special case
    if ((piece == 'K' && move->from == E1 && move->to == G1) ||
        (piece == 'k' && move->from == E8 && move->to == G8))
    {
        return snprintf(buf, len, "O-O");
    }
    if ((piece == 'K' && move->from == E1 && move->to == C1) ||
        (piece == 'k' && move->from == E8 && move->to == C8))
    {
        return snprintf(buf, len, "O-O-O");
    }

    // Piece symbol
    char *san = buf;
    if (piece != 'P' && piece != 'p') {
        *san++ = toupper(piece);
    }

    struct List *list = position_legal_moves(move->before);

    // Look for ambiguity (can skip search for pawns)
    struct List *it = (piece != 'P' && piece != 'p') ? list->next : list;
    for (; it != list; it = it->next) {
        struct Move *each = it->data;
        if (each->to == move->to && each->from != move->from &&
            position_piece(move->before, each->from) == piece)
        {
            // SAN is potentially ambiguous
            break;
        }
    }

    // Disambiguate from square
    if (it != list) {
        struct Move *each = it->data;
        if (square_rank(each->from) == square_rank(move->from)) {
            *san++ = square_file(move->from);
        }
        if (square_file(each->from) == square_file(move->from)) {
            *san++ = square_rank(move->from);
        }
    }

    // Capture
    if (position_piece(move->before, move->to) != EMPTY) {
        if (piece == 'P' || piece == 'p') {
            // File required for pawn captures
            *san++ = square_file(move->from);
        }
        *san++ = 'x';
    }

    // To square
    *san++ = square_file(move->to);
    *san++ = square_rank(move->to);

    // Promotion
    if (move->promotion != EMPTY) {
        *san++ = '=';
        *san++ = tolower(move->promotion);
    }

    *san = '\0';
    return san - buf;
}

const char *move_san_static(const struct Move *move) {
    static char buf[7];
    return move_san(buf, sizeof buf, move) > 0 ? buf : NULL;
}

// Standard Algebraic Notation
struct Move *move_from_san(struct Position *before, const char *san) {
    assert(before && san);

    // Handle castling as a special case
    if (!strcmp(san, "O-O")) {
        return move_named(before->turn == 'w' ? "e1g1" : "e8g8");
    }
    if (!strcmp(san, "O-O-O")) {
        return move_named(before->turn == 'w' ? "e1c1" : "e8c8");
    }

    char piece     = 'P';
    char from_file = '\0';
    char from_rank = '\0';
    char to_file   = '\0';
    char to_rank   = '\0';
    char promotion = EMPTY;

    // Piece symbol
    if (*san && strchr("NBRQK", *san)) {
        piece = *san++;
    }
    piece = before->turn == 'w' ? piece : tolower(piece);

    // Optional disambiguation
    if (*san && strchr("abcdefgh", *san)) {
        from_file = *san++;
    }
    if (*san && strchr("12345687", *san)) {
        from_rank = *san++;
    }

    // Optional separator
    bool capture = false; // Used for move validation
    if (*san == '-' || *san == 'x') {
        if (piece == 'P' || piece == 'p') {
            if (!from_file) {
                // File required for pawn captures
                return NULL;
            }
        } else if (!from_file && !from_rank) {
            // Not valid SAN
            return NULL;
        }
        capture = *san++ == 'x';
    }

    // To square
    if (!*san || !strchr("abcdefgh", *san)) {
        // There was no disambiguation, we've already read the to square
        if (!from_file || !from_rank) {
            return NULL;
        }
        to_file = from_file;
        to_rank = from_rank;
        from_file = '\0';
        from_rank = '\0';
        goto promotion;
    }
    to_file = *san++;

    if (!*san || !strchr("12345678", *san)) {
        return NULL;
    }
    to_rank = *san++;

promotion:
    if (*san == '=') {
        ++san;
    }
    if (*san && strchr("NBRQ", *san)) {
        promotion = before->turn == 'w' ? *san++ : tolower(*san++);
    }
    if (*san && !isspace(*san)) {
        return NULL;
    }

    const enum Square to = square(to_file, to_rank);
    struct Move *found = NULL;

    // Find matching move
    struct List *list = position_legal_moves(before);
    for (struct List *it = list->next; it != list; it = it->next) {
        struct Move *move = it->data;
        if (move->to == to &&
            position_piece(before, move->from) == piece &&
            (from_file == '\0'  || square_file(move->from) == from_file) &&
            (from_rank == '\0'  || square_rank(move->from) == from_rank) &&
            (promotion == EMPTY || promotion == move->promotion))
        {
            if (found) {
                // Ambiguous
                found = NULL;
                break;
            }
            found = move;
        }
    }

    if (found && capture && position_piece(before, found->to) == EMPTY) {
        // Nothing captured
        found = NULL;
    }

    return found;
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
