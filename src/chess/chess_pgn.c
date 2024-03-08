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

static void pgn_write_tags(FILE *out, const struct Game *game)
{
    assert(out);
    assert(game_valid(game));

    fprintf(out, "[Event \"%s\"]\n", game->event);
    fprintf(out, "[Site \"%s\"]\n", game->site);
    fprintf(out, "[Date \"%s\"]\n", game->date);
    fprintf(out, "[Round \"%s\"]\n", game->round);
    fprintf(out, "[White \"%s\"]\n", game->white);
    fprintf(out, "[Black \"%s\"]\n", game->black);
    fprintf(out, "[Result \"%s\"]\n", game->result);
    fprintf(out, "\n");
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
    if (after && position_is_check(after)) {
        if (position_is_checkmate(after)) {
            fputc('#', out);
        } else {
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

static void pgn_write(FILE *out, const struct Game *game) {
    pgn_write_movetext(out, game);
}

char *game_pgn(const struct Game *game) {
    assert(game_valid(game));

    char  *buf = NULL;
    size_t len = 0;
    FILE *out = open_memstream(&buf, &len);
    if (!out) {
        return NULL;
    }

    pgn_write(out, game);
    fclose(out);
    if (buf == NULL || len == 0) {
        return NULL;
    }

    // Move to GC heap
    char *result = GC_MALLOC_ATOMIC(len + 1);
    strncpy(result, buf, len + 1);
    free(buf);
    return result;
}

void game_save_pgn(const struct Game *game, const char *filename) {
    assert(game_valid(game));

    FILE *out = fopen(filename, "w");
    if (out) {
        pgn_write_tags(out, game);
        pgn_write(out, game);
        fclose(out);
    }
}

static void pgn_write_list(FILE *out, const struct Game *game)
{
    assert(out);
    assert(game_valid(game));

    struct List *begin = list_begin(((struct Game*)game)->history);
    struct List *end   = list_end(((struct Game*)game)->history);

    const struct Position *before = begin->data;
    bool is_first_move = true;
    for (begin = begin->next; begin != end; begin = begin->next) {
        const struct Position *after = begin->data;

        const struct Move *move = NULL;
        const struct List *each = list_begin(before->moves_played);
        for (; each != list_end(before->moves_played); each = each->next) {
            move = each->data;
            if (move->after == after) {
                break;
            }
        }

        pgn_write_move(out, before, move, is_first_move);
        is_first_move = false;

        if (before->turn == 'b') {
            fputc('\n', out);
        }
        before = after;
    }
    if (before->turn == 'b') {
        fputc('\n', out);
    }

    const struct List *each = list_begin(before->moves_played);
    for (; each != list_end(before->moves_played); each = each->next) {
        const struct Move *move = each->data;
        pgn_write_move(out, before, move, true);
        fputc('\n', out);
    }
}

struct List *game_pgn_list(const struct Game *game) {
    assert(game_valid(game));

    char  *buf = NULL;
    size_t len = 0;
    FILE *out = open_memstream(&buf, &len);
    if (!out) {
        return NULL;
    }

    pgn_write_list(out, game);
    fclose(out);
    if (buf == NULL || len == 0) {
        return NULL;
    }

    // Move to GC heap
    char *result = GC_MALLOC_ATOMIC(len + 1);
    strncpy(result, buf, len + 1);
    free(buf);

    struct List *list = list_new();
    while (result) {
        char *line = strsep(&result, "\n");
        if (*line == ' ') {
            line++;
        }
        if (*line) {
            list_push(list, line);
        }
    }
    return list;
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
