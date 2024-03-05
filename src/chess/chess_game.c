// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_game.h"
#include "chess.h"
#include "../list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

bool game_valid(const struct Game *game) {
    return game && !list_empty(game->history) &&
           position_valid(game->history->prev->data);
}

struct Position *game_position(struct Game *game, int index) {
    assert(game_valid(game));
    struct Position *position = list_index(game->history, index);
    assert(!position || position_valid(position));
    return position;
}

struct Position *game_start(struct Game *game) {
    return game_position(game, 0);
}

struct Position *game_current(struct Game *game) {
    return game_position(game, -1);
}

struct Position *game_previous(struct Game *game) {
    return game_position(game, -2);
}

void game_from_position(struct Game *game, const struct Position *start) {
    assert(position_valid(start));
    game->history = list_new();
    // We share a lot, but don't share positions between games!
    list_push(game->history, position_dup(start));
    assert(game_valid(game));
}

void game_from_fen(struct Game *game, const char *fen) {
    struct Position *start = position_from_fen(fen);
    game_from_position(game, start);
}

struct List *game_legal_moves(const struct Game *game) {
    assert(game_valid(game));
    return position_legal_moves(game_current((struct Game*)game));
}

int game_apply_move(struct Game *game, const struct Move *move) {
    assert(game_valid(game));

    struct Position *current = game_current(game);
    struct Move *existing = movelist_find_equal(current->moves_played, move);
    if (existing) {
        // Move already in graph
        list_push(game->history, (struct Position*)existing->after);
        return 0;
    }

    struct Move *candidate =
        movelist_find_equal(position_legal_moves(current), move);
    if (!candidate) {
        // Not a legal move
        return 1;
    }

    list_push(current->moves_played, candidate);
    list_push(game->history, (struct Position*)candidate->after);
    assert(position_valid(candidate->after));
    assert(game_valid(game));
    return 0;
}

int game_apply_move_name(struct Game *game, const char *name) {
    return game_apply_move(game, move_from_name(name));
}

int game_move_san(struct Game *game, const char *san) {
    return game_apply_move(game, move_from_san(game_current(game), san));
}

int game_takeback(struct Game *game) {
    assert(game_valid(game));
    if (list_length(game->history) <= 1) {
        // No takeback available
        return 1;
    }

    list_pop(game->history);
    assert(game_valid(game));
    return 0;
}

int game_apply_takeback(struct Game *game, const struct Move *takeback) {
    assert(game_valid(game));
    assert(takeback);

    struct Position *current  = game_current(game);
    struct Position *previous = game_previous(game);
    if (!previous) {
        // No takeback available
        return 1;
    }

    struct Move *matching =
        movelist_find_equal(previous->moves_played, takeback);
    if (!matching) {
        // Not a legal takeback
        return 1;
    }

    assert(matching->after == current);
    list_pop(game->history);
    assert(game_valid(game));
    return 0;
}

// Here we try to interpret the move at a higher-level than the position,
// taking into account the context of the game.  So yes, the boardstate
// might represent a move or a move in-progress, but it could also be the
// completion of a two-part move (i.e., castling rook first) or a takeback.
bool game_read_move(
    struct List   *candidates,
    struct Move  **takeback,
    struct Game   *game,
    uint64_t       boardstate,
    struct Action *actions,
    int            num_actions)
{
    assert(list_valid(candidates) && list_empty(candidates));
    assert(takeback && !*takeback);
    assert(game_valid(game));
    assert(actions && num_actions >= 0);

    // If boardstate matches current position, there's no move to read.
    struct Position *current = game_current(game);
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
    struct List     *castling = list_new();
    struct Position *previous = game_previous(game);
    if (previous) {
        generate_castle_moves(castling, previous);
    }
    for (struct List *each = castling->next; each != castling; each = each->next) {
        struct Position *after = position_apply_move(previous, each->data);
        if (!position_legal(after)) {
            continue;
        }
        if (after->bitmap != boardstate) {
            maybe_valid = maybe_valid || position_incomplete(after, boardstate);
            continue;
        }

        // Find takeback move
        struct List *it = previous->moves_played->next;
        for (; it != previous->moves_played; it = it->next) {
            struct Move *move = it->data;
            if (position_equal(move->after, after)) {
                *takeback = move;
                break;
            }
        }
        assert(*takeback);

        // Undo previous move and apply castling move
        list_push(candidates, each->data);

        return true;
    }

    // Is this a takeback?
    if (previous && previous->bitmap == boardstate) {
        struct List *each = previous->moves_played->next;
        for (; each != previous->moves_played; each = each->next) {
            struct Move *move = each->data;
            if (position_equal(move->after, current)) {
                *takeback = move;
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
