// Copyright (c) 2024 Eric Sessoms
// See license at end of file

#include "chess_position.h"
#include "chess.h"
#include "../mem.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Moves
//

static struct Move move_freelist = LIST_INIT(move_freelist);

void move_free(struct Move *move) {
    assert(move);
    mem_erase(move, sizeof *move);
    move->next = NULL;
    move->prev = NULL;
    movelist_push(&move_freelist, move);
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
        .after     = NULL,
        .from      = INVALID_SQUARE,
        .to        = INVALID_SQUARE,
        .promotion = EMPTY,
    };
    return move;
}

bool move_valid(const struct Move *move) {
    assert(move);
    if (!square_valid(move->from) ||
        !square_valid(move->to)   ||
        move->from == move->to)
    {
        return false;
    }
    if (!strchr(" NBRQnbrq", move->promotion)) {
        return false;
    }
    if (move->promotion != EMPTY) {
        return !is_last_rank(move->to, piece_color(move->promotion));
    }
    return true;
}

struct Move *move_new(enum Square from, enum Square to, enum Piece promotion) {
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
    return a->from      == b->from      &&
           a->to        == b->to        &&
           a->promotion == b->promotion;
}

struct Move*
movelist_find_equal(const struct Move *list, const struct Move *move) {
    assert(move_valid(move));
    struct Move *begin = list->next;
    while (begin != list) {
        if (move_equal(begin, move)) {
            return begin;
        }
        begin = begin->next;
    }
    return NULL;
}

char *move_name(char *buf, size_t size, const struct Move *move) {
    assert(buf && size >= 5);
    assert(move_valid(move));
    snprintf(buf, size, "%s%s%c",
        square_name(move->from),
        square_name(move->to),
        move->promotion == EMPTY ? '\0' : move->promotion);
    return buf;
}

char *move_name_static(const struct Move *move) {
    static char buf[5];
    return move_name(buf, sizeof buf, move);
}

struct Move *move_named(const char *name) {
    assert(name);
    if (strlen(name) < 4) {
        return NULL;
    }

    struct Move *move = move_alloc();
    move->from = square_named(name + 0);
    move->to   = square_named(name + 2);
    move->promotion = name[4] || EMPTY;
    if (move_valid(move)) {
        return move;
    }

    move_free(move);
    return NULL;
}

//
// Positions
//

static struct Position position_freelist = LIST_INIT(position_freelist);

void position_free(struct Position *position) {
    assert(position);

    movelist_free(&position->moves);

    mem_erase(position, sizeof *position);
    position->next = NULL;
    position->prev = NULL;
    positionlist_push(&position_freelist, position);
}

enum Piece position_piece(const struct Position *position, enum Square square) {
    return position->mailbox.cells[mailbox_index[square]];
}

void position_update_bitmap(struct Position *position) {
    uint64_t white = 0;
    uint64_t black = 0;
    uint64_t mask  = 1;
    for (enum Square sq = A8; sq <= H1; ++sq) {
        switch (position_piece(position, sq)) {
            case 'P': case 'N': case 'B': case 'R': case 'Q': case 'K':
                white |= mask;
                break;
            case 'p': case 'n': case 'b': case 'r': case 'q': case 'k':
                black |= mask;
                break;
            default:
                break;
        }
        mask <<= 1;
    }
    position->bitmap = white | black;
    position->white_bitmap = white;
}

struct Position *position_alloc(void) {
    struct Position *position = positionlist_pop(&position_freelist);
    if (!position) {
        mem_alloc((void**)&position, sizeof *position);
    }
    mem_erase(position, sizeof *position);
    *position = (struct Position){
        .next       = NULL,
        .prev       = NULL,
        .moves      = (struct Move)LIST_INIT(position->moves),
        .bitmap     = 0,
        .turn       = WHITE,
        .castle     = 0,
        .en_passant = INVALID_SQUARE,
        .halfmove   = 0,
        .fullmove   = 1,
    };
    mailbox_invalid(&position->mailbox);
    for (enum Square sq = A8; sq <= H1; ++sq) {
        position->mailbox.cells[mailbox_index[sq]] = EMPTY;
    }
    position_update_bitmap(position);
    return position;
}

struct Position *position_new(const char *fen) {
    struct Position *position = position_alloc();
    if (!fen || !position_read_fen(position, fen)) {
        position_read_fen(position, STARTING_FEN);
    }
    position_update_bitmap(position);
    return position;
}

struct Position *position_dup(const struct Position *position) {
    struct Position *copy = position_alloc();
    copy->bitmap     = position->bitmap;
    copy->turn       = position->turn;
    copy->castle     = position->castle;
    copy->en_passant = position->en_passant;
    copy->halfmove   = position->halfmove;
    copy->fullmove   = position->fullmove;
    memcpy(&copy->mailbox, &position->mailbox, sizeof copy->mailbox);
    return copy;
}

bool position_equal(const struct Position *a, const struct Position *b) {
    return a->bitmap == b->bitmap &&
           a->turn   == b->turn   &&
           a->castle == b->castle &&
           a->en_passant == b->en_passant &&
           a->halfmove   == b->halfmove   &&
           a->fullmove   == b->fullmove   &&
           memcmp(&a->mailbox, &b->mailbox, sizeof a->mailbox) == 0;
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
