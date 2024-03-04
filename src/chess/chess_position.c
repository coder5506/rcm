// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_position.h"
#include "chess.h"
#include "../list.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gc/gc.h>

// Piece at square
enum Piece position_piece(const struct Position *position, enum Square square) {
    return position->mailbox.cells[mailbox_index[square]];
}

static bool bitmap_valid(uint64_t bitmap) {
    const int popcount = __builtin_popcountll(bitmap);
    return 2 <= popcount && popcount <= 32;
}

static bool color_bitmap_valid(uint64_t bitmap) {
    const int popcount = __builtin_popcountll(bitmap);
    return 1 <= popcount && popcount <= 16;
}

static bool position_valid(const struct Position *position) {
    return position &&
        bitmap_valid(position->bitmap) &&
        color_bitmap_valid(position->white_bitmap) &&
        color_bitmap_valid(position->bitmap & ~position->white_bitmap) &&
        (position->turn == WHITE || position->turn == BLACK) &&
        (position->castle & ~0x0F) == 0 &&
        (position->en_passant == INVALID_SQUARE ||
         (A3 <= position->en_passant && position->en_passant <= H6)) &&
        (0 <= position->halfmove && position->halfmove <= 100) &&
        (1 <= position->fullmove);
}

static void position_update_bitmap(const struct Position *position) {
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

    struct Position *mutable = (struct Position*)position;
    mutable->bitmap = white | black;
    mutable->white_bitmap = white;
}

struct Position *position_new(void) {
    struct Position *position = GC_MALLOC(sizeof *position);
    *position = (struct Position){
        .bitmap       = 0,
        .white_bitmap = 0,
        .turn         = WHITE,
        .castle       = 0,
        .en_passant   = INVALID_SQUARE,
        .halfmove     = 0,
        .fullmove     = 1,
        .moves_played = list_new(),
        .legal_moves  = NULL,
    };
    mailbox_empty(&position->mailbox);
    position_update_bitmap(position);
    return position;
}

struct Position *position_dup(const struct Position *src) {
    struct Position *dst = position_new();
    memcpy(dst, src, sizeof *dst);  // Bit-copy picks up mailbox and bitmaps
    dst->moves_played = list_new(); // Don't share moves played
    dst->legal_moves  = NULL;       // Don't share legal moves
    return dst;
}

struct Position *position_from_fen(const char *fen) {
    struct Position *position = position_new();
    if (!fen || !position_read_fen(position, fen)) {
        position_read_fen(position, STARTING_FEN);
    }
    position_update_bitmap(position);
    assert(position_valid(position));
    return position;
}

bool position_equal(const struct Position *a, const struct Position *b) {
    assert(position_valid(a));
    assert(position_valid(b));
    return a->turn   == b->turn   &&
           a->castle == b->castle &&
           a->en_passant == b->en_passant &&
           a->halfmove   == b->halfmove   &&
           a->fullmove   == b->fullmove   &&
           mailbox_equal(&a->mailbox, &b->mailbox);
}

// Result of making move in position
//
// N.B., We don't check move legality here, because we have to generate
// positions to determine if moves are legal.  The appropriate checks happen
// higher-up, in game_apply_move.
struct Position*
position_apply_move(const struct Position *before, const struct Move *move) {
    struct Position *after = position_dup(before);

    // Move and capture
    const int from_cell = mailbox_index[move->from];
    const int to_cell   = mailbox_index[move->to];
    const enum Piece moving = after->mailbox.cells[from_cell];
    enum Piece captured = after->mailbox.cells[to_cell];
    if (piece_color(moving)   != before->turn ||
        piece_color(captured) == before->turn)
    {
        // Not a possible move
        return NULL;
    }

    // Update mailbox
    after->mailbox.cells[from_cell] = ' ';
    after->mailbox.cells[to_cell]   = move->promotion != ' ' ? move->promotion : moving;

    // Capture en passant
    if ((moving == 'P' || moving == 'p') && move->to == after->en_passant) {
        // Direction of moving pawn, not captured pawn
        const int direction = piece_color(moving) == WHITE ? -10 : 10;

        // Capture opposite moving pawn's direction of travel
        captured = after->mailbox.cells[to_cell - direction];
        after->mailbox.cells[to_cell + direction] = ' ';
    }

    // Move rook when castling
    if (moving == 'K' && move->from == E1 && move->to == G1) {
        after->mailbox.cells[mailbox_index[H1]] = ' ';
        after->mailbox.cells[mailbox_index[F1]] = 'R';
    } else if (moving == 'K' && move->from == E1 && move->to == C1) {
        after->mailbox.cells[mailbox_index[A1]] = ' ';
        after->mailbox.cells[mailbox_index[D1]] = 'R';
    } else if (moving == 'k' && move->from == E8 && move->to == G8) {
        after->mailbox.cells[mailbox_index[H8]] = ' ';
        after->mailbox.cells[mailbox_index[F8]] = 'r';
    } else if (moving == 'k' && move->from == E8 && move->to == C8) {
        after->mailbox.cells[mailbox_index[A8]] = ' ';
        after->mailbox.cells[mailbox_index[D8]] = 'r';
    }

    // Mailbox updated
    position_update_bitmap(after);

    // Update turn
    after->turn = color_other(after->turn);

    // Update castling
    if (moving == 'K') {
        after->castle &= ~(K_CASTLE | Q_CASTLE);
    } else if (moving == 'k') {
        after->castle &= ~(k_CASTLE | q_CASTLE);
    }

    if (moving == 'R' && move->from == H1) {
        after->castle &= ~K_CASTLE;
    } else if (moving == 'R' && move->from == A1) {
        after->castle &= ~Q_CASTLE;
    } else if (moving == 'r' && move->from == H8) {
        after->castle &= ~k_CASTLE;
    } else if (moving == 'r' && move->from == A8) {
        after->castle &= ~q_CASTLE;
    }

    // Update en passant
    after->en_passant = INVALID_SQUARE;
    if ((moving == 'P' || moving == 'p') && abs(move->to - move->from) == 20) {
        // i.e., pawn moved two rows, en passant is on the skipped row
        after->en_passant = move->from + (move->to - move->from) / 2;
    }

    // Update halfmove clock
    ++after->halfmove;
    if (moving == 'P' || moving == 'p' || captured != ' ') {
        after->halfmove = 0;
    }

    // Update fullmove number
    if (after->turn == 'w') {
        ++after->fullmove;
    }

    return after;
}

// True if boardstate might represent a transition into this position
bool position_incomplete(const struct Position *position, uint64_t boardstate) {
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
    struct List           *candidates,
    const struct Position *before,
    uint64_t               boardstate)
{
    assert(candidates && list_empty(candidates));
    assert(before);

    bool maybe_valid = false;

    struct List *legal_moves = position_legal_moves(before);
    struct List *each = list_begin(legal_moves);
    for (; each != list_end(legal_moves); each = each->next) {
        const struct Move     *move  = each->data;
        const struct Position *after = move->after;
        if (after->bitmap == boardstate) {
            list_push(candidates, (struct Move*)move);
            maybe_valid = true;
        } else {
            maybe_valid = maybe_valid || position_incomplete(after, boardstate);
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
    struct List           *candidates,
    const struct Position *before,
    uint64_t               boardstate,
    struct Action         *actions,
    int                    num_actions)
{
    assert(candidates && list_empty(candidates));
    assert(before);
    assert(actions || num_actions >= 0);

    const bool maybe_valid = position_read_moves(candidates, before, boardstate);
    if (list_empty(candidates)) {
        return maybe_valid;
    }

    assert(maybe_valid);

    // This is a check.  When we're done, the list should contain either moves
    // or promotions, but not both.
    int num_moves      = 0;
    int num_promotions = 0;

    struct List *each = list_begin(candidates);
    while (each != list_end(candidates)) {
        struct List *next = each->next;

        const struct Move *move = each->data;
        if (move->promotion != EMPTY) {
            // Can't resolve promotion here
            ++num_promotions;
            goto next;
        }

        const enum Piece capture = position_piece(before, move->to);
        if (capture == EMPTY) {
            // No capture, not ambiguous
            ++num_moves;
            goto next;
        }

        // History should show a lift followed by a place on the capture square
        bool got_place = false;
        bool got_lift  = false;
        for (int i = num_actions - 1; i >= 0; --i) {
            if (!got_place) {
                if (actions[i].place == move->to) {
                    got_place = true;
                }
            } else if (!got_lift) {
                if (actions[i].lift == move->to) {
                    got_lift = true;
                }
            } else {
                // Got both lift and place, so we're done
                break;
            }
        }

        if (!got_place || !got_lift) {
            list_unlink(each);
            goto next;
        }
        ++num_moves;

    next:
        each = next;
    }

    assert(num_moves == 0 || num_promotions == 0);
    assert(num_moves == 0 || num_moves == 1);
    assert(0 <= num_promotions && num_promotions <= 4);
    assert(list_length(candidates) == num_moves + num_promotions);
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
