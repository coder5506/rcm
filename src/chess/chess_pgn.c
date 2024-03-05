// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_pgn.h"
#include "chess.h"
#include "../list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gc/gc.h>

static bool position_is_checkmate(const struct Position *position)
{
    (void)position;
    return false;
}

static bool position_is_check(const struct Position *position)
{
    (void)position;
    return false;
}

static void pgn_write_move(
    FILE *out,
    const struct Position *before,
    const struct Move     *move,
    bool                   show_move_number)
{
    if (!show_move_number && before->turn == 'w') {
        fputc(' ', out);
    }

    if (show_move_number || before->turn == 'w') {
        fprintf(out, before->turn == 'w' ? "%d." : "%d...", before->fullmove);
    }
    fputc(' ', out);
    san_write_move(out, before, move);

    const struct Position *after = move->after;
    if (after) {
        if (position_is_checkmate(after)) {
            fputc('#', out);
        } else if (position_is_check(after)) {
            fputc('+', out);
        }
    }
}

static void
pgn_write_moves(FILE *out, const struct Position *before, bool is_first_move)
{
    assert(out);
    assert(position_valid(before));

    bool show_move_number = is_first_move;

    while (before && !list_empty(before->moves_played)) {
        const struct List *begin = list_begin(before->moves_played);
        const struct List *end   = list_end(before->moves_played);

        const struct Move *move = begin->data;
        pgn_write_move(out, before, move, show_move_number);
        show_move_number = false;

        for (begin = begin->next; begin != end; begin = begin->next) {
            fprintf(out, " (");
            const struct Move *variation = begin->data;
            pgn_write_move(out, before, variation, true);
            pgn_write_moves(out, variation->after, false);
            fprintf(out, ") ");

            show_move_number = true;
        }

        before = move->after;
    }
}

static void pgn_write_movetext(FILE *out, const struct Game *game)
{
    pgn_write_moves(out, game_start((struct Game*)game), true);
}

static int pgn_write(FILE *out, const struct Game *game) {
    pgn_write_movetext(out, game);
    return 0;
}

char *game_pgn(const struct Game *game) {
    assert(game_valid(game));

    char  *buf = NULL;
    size_t len = 0;
    FILE *out = open_memstream(&buf, &len);
    if (!out) {
        return NULL;
    }

    const int err = pgn_write(out, game);
    fclose(out);
    if (buf == NULL || len == 0) {
        return NULL;
    }
    if (err != 0) {
        free(buf);
        return NULL;
    }

    // Move to GC heap
    char *result = GC_MALLOC(len + 1);
    strncpy(result, buf, len + 1);
    free(buf);
    return result;
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
