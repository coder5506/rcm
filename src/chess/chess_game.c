// Copyright (c) 2024 Eric Sessoms
// See license at end of file

#include "chess_game.h"
#include "chess.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void game_free_moves(struct Game *game) {
    while (!movelist_empty(&game->moves)) {
        struct Move *move = movelist_shift(&game->moves);
        position_free(move->after);
        move_free(move);
    }
}

void game_destroy(struct Game *game) {
    struct Position *start = game->history.next;
    positionlist_clear(&game->history);

    struct Position queue = LIST_INIT(queue);
    positionlist_push(&queue, start);

    // Collect all positions in graph
    struct Position collected = LIST_INIT(collected);
    while (!positionlist_empty(&queue)) {
        struct Position *pos = positionlist_shift(&queue);
        if (positionlist_find(&collected, pos)) {
            // Already visited
            continue;
        }

        // Visit node
        positionlist_push(&collected, pos);

        // Follow edges
        while (!movelist_empty(&pos->moves)) {
            struct Move *move = movelist_shift(&pos->moves);
            positionlist_push(&queue, move->after);
            move_free(move); // Edges are not shared, OK to free
        }
    }

    // Free collected positions
    positionlist_free(&collected);

    // Available moves may introduce new positions
    game_free_moves(game);
}

static void game_update_moves(struct Game *game) {
    game_free_moves(game);
    const struct Position *position = game->history.prev;
    position_legal_moves(&game->moves, position);
}

void game_init(struct Game *game, const char *fen) {
    game->history = (struct Position)LIST_INIT(game->history);
    game->moves   = (struct Move)LIST_INIT(game->moves);

    struct Position *start = position_new(fen);
    positionlist_push(&game->history, start);
    game_update_moves(game);
}

int game_move(struct Game *game, const struct Move *move) {
    struct Move *candidate = movelist_find_equal(&game->moves, move);
    if (!candidate) {
        // Not a legal move
        return 1;
    }

    struct Position *position = game->history.prev;
    struct Move *existing = movelist_find_equal(&position->moves, candidate);
    if (existing) {
        // Move/position already in graph
        candidate = existing;
    } else {
        movelist_remove(candidate);
        movelist_push(&position->moves, candidate);
    }

    positionlist_push(&game->history, candidate->after);
    game_update_moves(game);
    return 0;
}

// True only if boardstate might represent transition into position
static bool
in_progress(const struct Position *position, uint64_t boardstate) {
    assert(position);

    // The boardstate for a move in-progress can differ only in small
    // ways from the boardstate of the completed move:
    // - It can be missing up-to two pieces of the same color, but only
    //   when castling.
    // - It can be missing up-to one piece of each color, but only when
    //   capturing.
    // - It can have up-to one extra piece of the opposite color, but
    //   only when capturing en-passant.

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
        if ((position->castle & K_CASTLE) && removed_bmp == (1ul << E1 | 1ul << H1)) {
            return true;
        }
        if ((position->castle & Q_CASTLE) && removed_bmp == (1ul << E1 | 1ul << A1)) {
            return true;
        }
        return false;
    }
    if (black_removed == 2) {
        if ((position->castle & k_CASTLE) && removed_bmp == (1ul << E8 | 1ul << H8)) {
            return true;
        }
        if ((position->castle & q_CASTLE) && removed_bmp == (1ul << E8 | 1ul << A8)) {
            return true;
        }
        return false;
    }

    // Third check: consider the possibility of en passant
    if (added == 0) {
        // i.e., we're unable to reject the possibility
        return true;
    }
    if (position->en_passant == INVALID_SQUARE) {
        return false;
    }

    // ...in mailbox coordinates
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

// We should be able to either match a position or flag the move as
// in-progress.  If we can't do either, the move is illegal.
struct Position*
game_read_move(
    struct Game  *game,
    uint64_t      boardstate,
    struct Move **move,
    struct Move **takeback,
    bool         *incomplete,
    bool         *promotion)
{
    assert(game && move && takeback && incomplete && promotion);
    *move       = NULL;
    *takeback   = NULL;
    *incomplete = false;
    *promotion  = false;

    // Match start position?
    struct Position *start = game->history.next;
    if (start->bitmap == boardstate) {
        return start;
    }

    // Match current position?
    struct Position *current = game->history.prev;
    if (current->bitmap == boardstate) {
        return current;
    }
    *incomplete = *incomplete || in_progress(current, boardstate);

    // Position reached by legal move?
    struct Move *begin = game->moves.next;
    for (; begin != &game->moves; begin = begin->next) {
        assert(begin->after);
        if (begin->after->bitmap == boardstate) {
            if (*move) {
                // Can have multiple matches only for pawn promotion
                *promotion = true;
            }
            // Default to the last promotion, which will be the queen
            *move = begin;
        } else {
            *incomplete = *incomplete || in_progress(begin->after, boardstate);
        }
    }
    if (*move) {
        return (*move)->after;
    }

    // Completion of castling move?
    struct Move castling = LIST_INIT(castling);
    struct Position *previous = game->history.prev->prev;
    if (previous != &game->history) {
        position_castle_moves(&castling, previous);
    }
    begin = castling.next;
    for (; begin != &castling; begin = begin->next) {
        struct Position *p = position_move(previous, begin);
        if (!position_legal(p)) {
            continue;
        }
        if (p->bitmap != boardstate) {
            *incomplete = *incomplete || in_progress(p, boardstate);
            continue;
        }

        // Find takeback move
        struct Move *m = previous->moves.next;
        for (; m != &previous->moves; m = m->next) {
            if (m->after == p) {
                *takeback = m;
                break;
            }
        }
        assert(*takeback);

        // Undo previous move and apply castling move
        *move = begin;
        return p;
    }

    // Takeback?
    if (previous->bitmap == boardstate) {
        struct Move *m = previous->moves.next;
        for (; m != &previous->moves; m = m->next) {
            if (m->after == current) {
                *takeback = m;
                return previous;
            }
        }
    }
    *incomplete = *incomplete || in_progress(previous, boardstate);

    return NULL;
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
