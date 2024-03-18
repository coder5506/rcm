// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_game.h"
#include "chess.h"
#include "../utility/kv.h"
#include "../utility/list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

const char *game_tag(struct Game *game, const char *key) {
    assert(game && kv_valid(game->tags));
    assert(key && *key);
    const struct KeyValue *tag = kv_find(game->tags, key);
    return tag ? tag->data : NULL;
}

void game_set_tag(struct Game *game, const char *key, void *data) {
    assert(game && kv_valid(game->tags));
    assert(key && *key);
    assert(data);

    struct KeyValue *tag = kv_find(game->tags, key);
    if (tag) {
        tag->data = data;
    } else {
        kv_push(game->tags, key, data);
    }
}

static void game_reset(struct Game *game) {
    assert(game);
    assert(list_valid(game->history));
    assert(kv_valid(game->tags));

    list_clear(game->history);
    kv_clear(game->tags);
    game->started = 0;
    game->id = 0;
    game->settings = NULL;

    // Reset default metadata
    game_set_tag(game, "Event", "?");
    game_set_tag(game, "Site", "?");
    game_set_tag(game, "Date", "????.??.??");
    game_set_tag(game, "Round", "-");
    game_set_tag(game, "White", "?");
    game_set_tag(game, "Black", "?");
    game_set_tag(game, "Result", "*");
}

static void game_changed(struct Game *game, void *data) {
    (void)data;

    if (list_length(game->history) <= 1 || game->started > 0) {
        // Either not started, or bookkeeping already done
        return;
    }

    game->started = time(NULL);
    struct tm timestamp;
    localtime_r(&game->started, &timestamp);

    char *date = malloc(11);
    strftime(date, 11, "%Y.%m.%d", &timestamp);
    game_set_tag(game, "Date", date);
}

static struct Game *game_alloc(void) {
    struct Game *game = malloc(sizeof *game);

    MODEL_INIT(game);
    game->history = list_new();
    game->tags = kv_new();
    game_reset(game);

    MODEL_OBSERVE(game, game_changed, NULL);
    return game;
}

bool game_valid(const struct Game *game) {
    return game &&
        list_valid(game->history) && !list_empty(game->history) &&
        kv_valid(game->tags) &&
        game->id >= 0;
}

void game_set_start(struct Game *game, const struct Position *start) {
    assert(game);
    assert(position_valid(start));

    game_reset(game);
    list_push(game->history, position_dup(start));

    assert(game_valid(game));
    MODEL_CHANGED(game);
}

struct Game *game_from_position(const struct Position *start) {
    assert(position_valid(start));
    struct Game *game = game_alloc();
    game_set_start(game, start);
    return game;
}

struct Game *game_from_fen(const char *fen) {
    struct Position *start = position_from_fen(fen);
    return game_from_position(start);
}

struct Position *game_position(struct Game *game, int index) {
    assert(game_valid(game));
    struct Position *position = list_index(game->history, index);
    assert(!position || position_valid(position));
    return position;
}

const struct Position *game_start(const struct Game *game) {
    return game_position((struct Game*)game, 0);
}

struct Position *game_current(struct Game *game) {
    return game_position(game, -1);
}

struct Position *game_previous(struct Game *game) {
    return game_position(game, -2);
}

struct Game *game_fork(const struct Game *game) {
    assert(game_valid(game));
    return game_from_position(game_current((struct Game*)game));
}

static int game_recover_history(struct Game *game, const struct Position *target) {
    assert(game_valid(game));
    assert(position_valid(target));

    struct Position *current = game_current(game);
    if (position_equal(current, target)) {
        return 0;
    }

    struct List *begin = list_begin(current->moves_played);
    for (; begin != list_end(current->moves_played); begin = begin->next) {
        struct Move *move = begin->data;
        list_push(game->history, (struct Position*)move->after);
        if (game_recover_history(game, target) == 0) {
            return 0;
        }
        list_pop(game->history);
    }

    return 1;
}

static int game_recover_position(struct Game *game, const struct Position *target) {
    assert(game_valid(game));
    assert(position_valid(target));

    const struct Position *start = game_start(game);
    list_clear(game->history);
    list_push(game->history, (struct Position*)start);
    assert(game_valid(game));

    return game_recover_history(game, target);
}

struct Game *game_from_pgn_and_fen(const char *pgn, const char *fen) {
    struct Game *game = game_from_pgn(pgn);
    struct Position *target = position_from_fen(fen);
    const int err = game_recover_position(game, target);
    if (err) {
        return NULL;
    }

    assert(game_valid(game));
    return game;
}

struct List *game_legal_moves(const struct Game *game) {
    assert(game_valid(game));
    return position_legal_moves(game_current((struct Game*)game));
}

// Undo last move, whatever it was
int game_takeback(struct Game *game) {
    assert(game_valid(game));
    if (list_length(game->history) <= 1) {
        // No takeback available
        return 1;
    }

    list_pop(game->history);
    assert(game_valid(game));
    MODEL_CHANGED(game);
    return 0;
}

// Undo last move, only if it can be validated
int game_apply_takeback(struct Game *game, const struct Move *takeback) {
    assert(game_valid(game));
    assert(move_valid(takeback));

    struct Position *current  = game_current(game);
    struct Position *previous = game_previous(game);
    if (!previous) {
        // No takeback available
        return 1;
    }

    struct Move *matching = movelist_find_equal(previous->moves_played, takeback);
    if (!matching) {
        // Not a legal takeback
        return 1;
    }

    assert(matching->after == current);
    return game_takeback(game);
}

int game_apply_move(struct Game *game, const struct Move *move) {
    assert(game_valid(game));
    assert(move_valid(move));

    struct Position *current = game_current(game);
    struct Move *matching = movelist_find_equal(current->moves_played, move);
    if (matching) {
        // Move already in graph
        list_push(game->history, (struct Position*)matching->after);
        goto success;
    }

    struct Move *candidate = movelist_find_equal(position_legal_moves(current), move);
    if (!candidate) {
        // Not a legal move
        return 1;
    }

    list_push(current->moves_played, candidate);
    list_push(game->history, (struct Position*)candidate->after);
    assert(position_valid(candidate->after));
    assert(game_valid(game));

success:
    MODEL_CHANGED(game);
    return 0;
}

int game_apply_move_name(struct Game *game, const char *name) {
    return game_apply_move(game, move_from_name(name));
}

int game_move_san(struct Game *game, const char *san) {
    return game_apply_move(game, move_from_san(game_current(game), san));
}

int
game_complete_move(
    struct Game       *game,
    const struct Move *takeback,
    const struct Move *move)
{
    assert(game_valid(game));
    assert(move_valid(takeback));
    assert(move_valid(move));

    // Unlike takeback, prior move was not a variation, just a transitional
    // arrangement of pieces.  So we erase it completely.
    list_pop(game->history);
    list_pop(game_current(game)->moves_played);
    return game_apply_move(game, move);
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
        struct List *it = list_begin(previous->moves_played);
        for (; it != list_end(previous->moves_played); it = it->next) {
            struct Move *move = it->data;
            if (position_equal(move->after, current)) {
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
