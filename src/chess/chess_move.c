// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_move.h"
#include "chess.h"
#include "../utility/list.h"

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
    return promotion == EMPTY || is_last_rank(to, piece_color(promotion));
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
    assert(list_valid(list));
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
    assert(buf && len >= 0);
    assert(move_valid(move));

    if (len < 6) {
        // e.g., "e7e8q\0"
        return 0;
    }

    return snprintf(buf, len, "%s%s%c",
        square_name(move->from),
        square_name(move->to),
        move->promotion == EMPTY ? '\0' : tolower(move->promotion));
}

const char *move_name_static(const struct Move *move) {
    static char buf[6];
    return move_name(buf, sizeof buf, move) > 0 ? buf : NULL;
}

struct Move *move_from_name(const char *name) {
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
