// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_position.h"
#include "chess.h"
#include "../mem.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct Position position_freelist = LIST_INIT(position_freelist);

void position_free(struct Position *position) {
    assert(position);

    movelist_free(&position->moves_played);

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
        .next         = NULL,
        .prev         = NULL,
        .moves_played = (struct Move)LIST_INIT(position->moves_played),
        .bitmap       = 0,
        .turn         = WHITE,
        .castle       = 0,
        .en_passant   = INVALID_SQUARE,
        .halfmove     = 0,
        .fullmove     = 1,
    };
    mailbox_invalid(&position->mailbox);
    for (enum Square sq = A8; sq <= H1; ++sq) {
        position->mailbox.cells[mailbox_index[sq]] = EMPTY;
    }
    position_update_bitmap(position);
    return position;
}

struct Position *position_from_fen(const char *fen) {
    struct Position *position = position_alloc();
    if (!fen || !position_read_fen(position, fen)) {
        position_read_fen(position, STARTING_FEN);
    }
    position_update_bitmap(position);
    return position;
}

struct Position *position_copy(const struct Position *position) {
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

// This rank is part of the Raccoon's Centaur Mods (RCM).
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
