// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_move.h"
#include "chess.h"
#include "../mem.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct Move move_freelist = LIST_INIT(move_freelist);

void move_free(struct Move *move) {
    assert(move);
    mem_erase(move, sizeof *move);
    move->next = NULL;
    move->prev = NULL;
    movelist_push(&move_freelist, move);
}

void move_free_after(struct Move *move) {
    assert(move);
    if (move->after) {
        position_free(move->after);
        move->after = NULL;
    }
    move_free(move);
}

struct Move *move_alloc(void) {
    struct Move *move = movelist_pop(&move_freelist);
    if (!move) {
        mem_alloc((void**)&move, sizeof *move);
    }
    mem_erase(move, sizeof *move);
    *move = (struct Move){
        .next      = NULL,
        .prev      = NULL,
        .before    = NULL,
        .after     = NULL,
        .from      = INVALID_SQUARE,
        .to        = INVALID_SQUARE,
        .promotion = EMPTY,
    };
    return move;
}

bool move_validate(enum Square from, enum Square to, enum Piece promotion) {
    if (from == to || !square_valid(from) || !square_valid(to)) {
        return false;
    }
    if (!promotion || !strchr(" NBRQnbrq", promotion)) {
        return false;
    }
    return promotion == EMPTY || is_last_rank(to, piece_color(promotion));
}

bool move_valid(const struct Move *move) {
    assert(move);
    return move_validate(move->from, move->to, move->promotion);
}

struct Move *move_new(enum Square from, enum Square to, enum Piece promotion) {
    assert(move_validate(from, to, promotion));
    struct Move *move = move_alloc();
    move->from = from;
    move->to   = to;
    move->promotion = promotion;
    assert(move_valid(move));
    return move;
}

bool move_equal(const struct Move *a, const struct Move *b) {
    assert(move_valid(a));
    assert(move_valid(b));
    return a->from == b->from && a->to == b->to && a->promotion == b->promotion;
}

struct Move *move_copy(const struct Move *move) {
    return move_new(move->from, move->to, move->promotion);
}

struct Move *move_copy_after(const struct Move *move) {
    struct Move *copy = move_copy(move);
    copy->after = position_copy(move->after);
    return copy;
}

struct Move*
movelist_find_equal(const struct Move *list, const struct Move *move) {
    assert(list && move_valid(move));
    for (struct Move *begin = list->next; begin != list; begin = begin->next) {
        if (move_equal(begin, move)) {
            return begin;
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

    struct Move list = LIST_INIT(list);
    position_legal_moves(&list, move->before);

    // Look for ambiguity (can skip search for pawns)
    struct Move *begin = (piece != 'P' && piece != 'p') ? list.next : &list;
    for (; begin != &list; begin = begin->next) {
        if (begin->to == move->to && begin->from != move->from &&
            position_piece(move->before, begin->from) == piece)
        {
            // SAN is potentially ambiguous
            break;
        }
    }

    // Disambiguate from square
    if (begin != &list) {
        if (square_rank(begin->from) == square_rank(move->from)) {
            *san++ = square_file(move->from);
        }
        if (square_file(begin->from) == square_file(move->from)) {
            *san++ = square_rank(move->from);
        }
    }

    movelist_free_after(&list);

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

    struct Move list = LIST_INIT(list);
    position_legal_moves(&list, before);

    // Find matching move
    struct Move *begin = list.next;
    for (; begin != &list; begin = begin->next) {
        if (begin->to == to &&
            position_piece(before, begin->from) == piece &&
            (from_file == '\0'  || square_file(begin->from) == from_file) &&
            (from_rank == '\0'  || square_rank(begin->from) == from_rank) &&
            (promotion == EMPTY || promotion == begin->promotion))
        {
            if (found) {
                // Ambiguous
                found = NULL;
                break;
            }
            found = begin;
        }
    }

    if (found && capture && position_piece(before, found->to) == EMPTY) {
        // Nothing captured
        found = NULL;
    }

    if (found) {
        movelist_remove(found);
        position_free(found->after);
        found->before = before;
        found->after  = NULL;
    }

    movelist_free_after(&list);
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
