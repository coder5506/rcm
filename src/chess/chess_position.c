// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_position.h"
#include "chess.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gc/gc.h>

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
    struct Position *position = GC_MALLOC(sizeof *position);
    *position = (struct Position){
        .bitmap       = 0,
        .white_bitmap = 0,
        .turn         = WHITE,
        .castle       = 0,
        .en_passant   = INVALID_SQUARE,
        .halfmove     = 0,
        .fullmove     = 1,
        .legal_moves  = LIST_INIT(position->legal_moves),
        .moves_played = LIST_INIT(position->moves_played),
    };
    mailbox_invalid(&position->mailbox);
    for (enum Square sq = A8; sq <= H1; ++sq) {
        position->mailbox.cells[mailbox_index[sq]] = EMPTY;
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

struct Position *position_from_fen(const char *fen) {
    struct Position *position = position_alloc();
    if (!fen || !position_read_fen(position, fen)) {
        position_read_fen(position, STARTING_FEN);
    }
    position_update_bitmap(position);
    return position;
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

// True if boardstate might represent a transition into this position
bool position_incomplete(struct Position *position, uint64_t boardstate) {
    assert(position);

    // The boardstate for a move in-progress can differ only in small ways from
    // the boardstate of the completed move:
    // - It can be missing up-to two pieces of the same color, but only when
    //   castling.
    // - It can be missing up-to one piece of each color, but only when capturing.
    // - It can have up-to one extra piece of the opposite color, but only when
    //   capturing en-passant.

    // Present in position but not on board
    const uint64_t removed_bmp = position->bitmap & ~boardstate;
    const int removed = __builtin_popcountll(removed_bmp);

    // Present on board but not in position
    const int added   = __builtin_popcountll(boardstate & ~position->bitmap);

    // First check: just count the differences
    if (removed > 2 || added > 1 || removed + added > 2) {
        return false;
    }

    // Second check: look at the colors involved
    const uint64_t white_bitmap = position->white_bitmap;
    const uint64_t black_bitmap = position->bitmap & ~white_bitmap;
    const int white_removed = __builtin_popcountll(white_bitmap & ~boardstate);
    const int black_removed = __builtin_popcountll(black_bitmap & ~boardstate);
    assert(removed == white_removed + black_removed);

    if (position->turn == 'w' && black_removed > 1) {
        return false;
    }
    if (position->turn == 'b' && white_removed > 1) {
        return false;
    }

    if (white_removed == 2) {
        if ((position->castle & K_CASTLE) && removed_bmp == (1ull << E1 | 1ull << H1)) {
            return true;
        }
        if ((position->castle & Q_CASTLE) && removed_bmp == (1ull << E1 | 1ull << A1)) {
            return true;
        }
        return false;
    }
    if (black_removed == 2) {
        if ((position->castle & k_CASTLE) && removed_bmp == (1ull << E8 | 1ull << H8)) {
            return true;
        }
        if ((position->castle & q_CASTLE) && removed_bmp == (1ull << E8 | 1ull << A8)) {
            return true;
        }
        return false;
    }

    // Third check: consider the possibility of en passant
    if (added == 0) {
        // i.e., we're unable to reject the possibility
        return true;
    }

    assert(added == 1);
    if (position->en_passant == INVALID_SQUARE) {
        return false;
    }

    // In mailbox coordinates...
    const int direction = position->turn == 'w' ? -10 : 10;
    const int captured  = mailbox_index[position->en_passant] - direction;

    uint64_t mask = 1 << position->en_passant | 1 << board_index[captured];
    if (board_index[captured-1] != INVALID_SQUARE) {
        mask |= 1 << board_index[captured-1];
    }
    if (board_index[captured+1] != INVALID_SQUARE) {
        mask |= 1 << board_index[captured+1];
    }

    const uint64_t diff = position->bitmap ^ boardstate;
    return (diff & ~mask) == 0;
}

// Construct a list of candidate moves in this position that match the given
// boardstate.  The return indicates if there are any viable candidates:
// - true if any candidates are found OR if the boardstate could be in
//   transition to a valid move,
// - false if the boardstate is incompatible with all legal moves in this
//   position.
static bool position_read_moves(
    struct Node     *candidates,
    struct Position *position,
    uint64_t         boardstate)
{
    assert(candidates && list_empty(candidates));
    assert(position);

    // Ensure we've generated the legal moves for this position
    if (list_empty(&position->legal_moves)) {
        position_legal_moves(&position->legal_moves, position);
    }

    bool maybe_valid = false;

    struct Node *begin = position->legal_moves.next;
    for (; begin != &position->legal_moves; begin = begin->next) {
        struct Move *m = begin->data;
        assert(m->after);
        if (m->after->bitmap == boardstate) {
            list_push(candidates, m);
            maybe_valid = true;
        } else {
            maybe_valid =
                maybe_valid || position_incomplete(m->after, boardstate);
        }
    }

    assert(maybe_valid || list_empty(candidates));
    return maybe_valid;
}

// The boardstate alone can be ambiguous in the case of piece captures.  Here
// we use the actions history to disambiguate the move.  Note that we still
// generate a list, because (a) the candidates might be promotions which cannot
// be resolved here and (b) the actions history might be incomplete.
bool position_read_move(
    struct Node     *candidates,
    struct Position *position,
    uint64_t         boardstate,
    struct Action   *actions,
    int              num_actions)
{
    assert(candidates && list_empty(candidates));
    assert(position);
    assert(actions || num_actions >= 0);

    const bool maybe_valid =
        position_read_moves(candidates, position, boardstate);
    if (list_empty(candidates)) {
        return maybe_valid;
    }

    assert(maybe_valid);

    // This is a check.  When we're done, the list should contain either moves
    // or promotions, but not both.
    int num_moves      = 0;
    int num_promotions = 0;

    struct Node *begin = candidates->next;
    for (; begin != candidates; begin = begin->next) {
        struct Move *m = begin->data;
        if (m->promotion != EMPTY) {
            // Can't resolve promotion here
            ++num_promotions;
            continue;
        }

        const enum Piece capture = position_piece(position, m->to);
        if (capture == EMPTY) {
            // No capture, not ambiguous
            ++num_moves;
            continue;
        }

        // History should show a lift followed by a place on the capture square
        bool got_place = false;
        bool got_lift  = false;
        for (int i = num_actions - 1; i >= 0; --i) {
            if (!got_place) {
                if (actions[i].place == m->to) {
                    got_place = true;
                }
            } else if (!got_lift) {
                if (actions[i].lift == m->to) {
                    got_lift = true;
                }
            } else {
                // Got both lift and place, so we're done
                break;
            }
        }

        if (!got_place || !got_lift) {
            continue;
        }
        ++num_moves;
    }

    assert(num_moves == 0 || num_promotions == 0);
    assert(num_moves == 0 || num_moves == 1);
    assert(0 <= num_promotions && num_promotions <= 4);
    return maybe_valid;
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
