// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_game.h"
#include "chess.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void game_update_moves(struct Game *game) {
    list_clear(&game->moves);
    const struct Position *position = game->history.prev->data;
    position_legal_moves(&game->moves, position);
}

void game_from_position(struct Game *game, const struct Position *start) {
    game->history = LIST_INIT(game->history);
    game->moves   = LIST_INIT(game->moves);
    list_push(&game->history, (struct Position*)start);
    game_update_moves(game);
}

void game_from_fen(struct Game *game, const char *fen) {
    struct Position *start = position_from_fen(fen);
    game_from_position(game, start);
}

int game_move(struct Game *game, const struct Move *move) {
    assert(game && game->history.prev != &game->history);

    struct Position *current = game->history.prev->data;
    struct Move *existing = movelist_find_equal(&current->moves_played, move);
    if (existing) {
        // Move already in graph
        list_push(&game->history, existing->after);
        game_update_moves(game);
        return 0;
    }

    struct Move *candidate = movelist_find_equal(&game->moves, move);
    if (!candidate) {
        // Not a legal move
        return 1;
    }

    list_push(&current->moves_played, candidate);
    list_push(&game->history, candidate->after);
    game_update_moves(game);
    return 0;
}

int game_takeback(struct Game *game, const struct Move *takeback) {
    assert(game && game->history.prev != &game->history);
    assert(takeback);

    struct Position *current  = game->history.prev->data;
    if (game->history.prev->prev == &game->history) {
        // No takeback available
        return 1;
    }

    struct Position *previous = game->history.prev->prev->data;
    struct Move *matching =
        movelist_find_equal(&previous->moves_played, takeback);
    if (!matching) {
        // Not a legal takeback
        return 1;
    }

    assert(matching->after == current);
    list_pop(&game->history);
    game_update_moves(game);
    return 0;
}

// Here we try to interpret the move at a higher-level than the position,
// taking into account the context of the game.  So yes, the boardstate
// might represent a move or a move in-progress, but it could also be the
// completion of a two-part move (i.e., castling rook first) or a takeback.
bool game_read_move(
    struct Node   *candidates,
    struct Move  **takeback,
    struct Game   *game,
    uint64_t       boardstate,
    struct Action *actions,
    int            num_actions)
{
    assert(candidates && list_empty(candidates));
    assert(takeback && !*takeback);
    assert(game);
    assert(actions && num_actions >= 0);

    // If boardstate matches current position, there's no move to read.
    struct Position *current = game->history.prev->data;
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
    struct Node castling = LIST_INIT(castling);
    struct Position *previous = NULL;
    if (game->history.prev->prev != &game->history) {
        previous = game->history.prev->prev->data;
        position_castle_moves(&castling, previous);
    }
    struct Node *begin = castling.next;
    for (; begin != &castling; begin = begin->next) {
        struct Position *after = position_move(previous, begin->data);
        if (!position_legal(after)) {
            continue;
        }
        if (after->bitmap != boardstate) {
            maybe_valid = maybe_valid || position_incomplete(after, boardstate);
            continue;
        }

        // Find takeback move
        struct Node *b = previous->moves_played.next;
        for (; b != &previous->moves_played; b = b->next) {
            struct Move *m = b->data;
            if (position_equal(m->after, after)) {
                *takeback = m;
                break;
            }
        }
        assert(*takeback);

        // Undo previous move and apply castling move
        list_push(candidates, begin->data);

        return true;
    }

    // Is this a takeback?
    if (previous && previous->bitmap == boardstate) {
        struct Node *b = previous->moves_played.next;
        for (; b != &previous->moves_played; b = b->next) {
            struct Move *m = b->data;
            if (position_equal(m->after, current)) {
                *takeback = m;
                return true;
            }
        }
    }

    // Possibly a takeback in progress?
    return previous && position_incomplete(previous, boardstate);
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
