// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_game.h"
#include "chess.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Also frees resulting positions
static void game_free_moves(struct Game *game) {
    while (!movelist_empty(&game->moves)) {
        struct Move *move = movelist_shift(&game->moves);
        position_free(move->after);
        move_free(move);
    }
}

void game_destroy(struct Game *game) {
    // Collect all positions in graph
    struct Position collected = LIST_INIT(collected);
    while (!positionlist_empty(&game->history)) {
        struct Position *pos = positionlist_shift(&game->history);
        positionlist_push(&collected, pos);
    }

    struct Position *begin = collected.next;
    for (; begin != &collected; begin = begin->next) {
        // Follow edges
        while (!movelist_empty(&begin->moves_played)) {
            struct Move *move = movelist_shift(&begin->moves_played);
            if (!positionlist_find(&collected, move->after)) {
                positionlist_push(&collected, move->after);
            }
            move_free(move); // Edges are not shared, OK to free
        }
    }

    // Free collected positions
    positionlist_free(&collected);
    game_free_moves(game);
}

static void game_update_moves(struct Game *game) {
    game_free_moves(game);
    const struct Position *position = game->history.prev;
    position_legal_moves(&game->moves, position);
}

void game_from_position(struct Game *game, const struct Position *start) {
    game->history = (struct Position)LIST_INIT(game->history);
    game->moves   = (struct Move)LIST_INIT(game->moves);

    struct Position *copy = position_copy(start);
    positionlist_push(&game->history, copy);
    game_update_moves(game);
}

void game_from_fen(struct Game *game, const char *fen) {
    struct Position *start = position_from_fen(fen);
    game_from_position(game, start);
    position_free(start);
}

int game_move(struct Game *game, const struct Move *move) {
    assert(game && game->history.prev != &game->history);

    struct Position *current = game->history.prev;
    struct Move *existing = movelist_find_equal(&current->moves_played, move);
    if (existing) {
        // Move already in graph
        positionlist_push(&game->history, existing->after);
        game_update_moves(game);
        return 0;
    }

    struct Move *candidate = movelist_find_equal(&game->moves, move);
    if (!candidate) {
        // Not a legal move
        return 1;
    }

    movelist_remove(candidate);
    movelist_push(&current->moves_played, candidate);
    positionlist_push(&game->history, candidate->after);
    game_update_moves(game);
    return 0;
}

int game_takeback(struct Game *game, const struct Move *takeback) {
    assert(game && game->history.prev != &game->history);
    assert(takeback);

    struct Position *current  = game->history.prev;
    struct Position *previous = current->prev;
    if (previous == &game->history) {
        // No takeback available
        return 1;
    }

    struct Move *matching =
        movelist_find_equal(&previous->moves_played, takeback);
    if (!matching) {
        // Not a legal takeback
        return 1;
    }

    assert(matching->after == current);
    positionlist_pop(&game->history);
    game_update_moves(game);
    return 0;
}

// Here we try to interpret the move at a higher-level than the position,
// taking into account the context of the game.  So yes, the boardstate
// might represent a move or a move in-progress, but it could also be the
// completion of a two-part move (i.e., castling rook first) or a takeback.
bool game_read_move(
    struct Move   *candidates,
    struct Move  **takeback,
    struct Game   *game,
    uint64_t       boardstate,
    struct Action *actions,
    int            num_actions)
{
    assert(candidates && movelist_empty(candidates));
    assert(takeback && !*takeback);
    assert(game);
    assert(actions && num_actions >= 0);

    // If boardstate matches current position, there's no move to read.
    struct Position *current = game->history.prev;
    if (current->bitmap == boardstate) {
        // i.e., not an error
        return true;
    }

    // Can boardstate be reached by a legal move?
    bool maybe_valid = position_read_move(
        candidates, current, boardstate, actions, num_actions);
    if (maybe_valid) {
        return true;
    }

    // Is this the completion of a castling move?
    struct Move castling = LIST_INIT(castling);
    struct Position *previous = current->prev;
    if (previous != &game->history) {
        position_castle_moves(&castling, previous);
    }
    struct Move *begin = castling.next;
    for (; begin != &castling; begin = begin->next) {
        struct Position *after = position_move(previous, begin);
        if (!position_legal(after)) {
            continue;
        }
        if (after->bitmap != boardstate) {
            maybe_valid = maybe_valid || position_incomplete(after, boardstate);
            continue;
        }

        // Find takeback move
        struct Move *m = previous->moves_played.next;
        for (; m != &previous->moves_played; m = m->next) {
            if (position_equal(m->after, after)) {
                *takeback = m;
                break;
            }
        }
        assert(*takeback);

        // Undo previous move and apply castling move
        struct Move *copy = move_copy(begin);
        copy->after = after;
        movelist_push(candidates, copy);

        return true;
    }

    // Is this a takeback?
    if (previous->bitmap == boardstate) {
        struct Move *m = previous->moves_played.next;
        for (; m != &previous->moves_played; m = m->next) {
            if (position_equal(m->after, current)) {
                *takeback = m;
                return true;
            }
        }
    }

    // Possibly a takeback in progress?
    return position_incomplete(previous, boardstate);
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
