// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_pgn.h"
#include "chess.h"
#include "../utility/kv.h"
#include "../utility/list.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void pgn_write_tags(FILE *out, const struct Game *game)
{
    assert(out);
    assert(game_valid(game));

    if (!game->tags) {
        return;
    }

    struct KeyValue *begin = kv_begin(game->tags);
    for (; begin != kv_end(game->tags); begin = begin->next) {
        fprintf(out, "[%s \"%s\"]\n", begin->key, (const char*)begin->data);
    }
    fprintf(out, "\n");
}

static void pgn_write_move(
    FILE                  *out,
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

        const struct Move *move = (const struct Move*)begin->data;
        pgn_write_move(out, before, move, show_move_number);
        show_move_number = false;

        for (begin = begin->next; begin != end; begin = begin->next) {
            fprintf(out, " (");
            const struct Move *variation = (const struct Move*)begin->data;
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
    pgn_write_tags(out, game);
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

    return buf;
}

void game_save_pgn(const struct Game *game, const char *filename) {
    assert(game_valid(game));

    FILE *out = fopen(filename, "w");
    if (out) {
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

    const struct Position *before = (const struct Position*)begin->data;
    bool is_first_move = true;
    for (begin = begin->next; begin != end; begin = begin->next) {
        const struct Position *after = (const struct Position*)begin->data;

        const struct Move *move = NULL;
        const struct List *each = list_begin(before->moves_played);
        for (; each != list_end(before->moves_played); each = each->next) {
            move = (const struct Move*)each->data;
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
        const struct Move *move = (const struct Move*)each->data;
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

    struct List *list = list_new();
    while (buf) {
        char *line = strsep(&buf, "\n");
        if (*line == ' ') {
            line++;
        }
        if (*line) {
            list_push(list, line);
        }
    }
    return list;
}

static void skip_whitespace(char **pgn) {
    while (isspace(**pgn)) {
        ++*pgn;
    }
}

static char *read_string(char **pgn) {
    skip_whitespace(pgn);
    if (**pgn != '"') {
        return NULL;
    }
    ++*pgn;

    char *begin = *pgn;
    char *end   = begin;
    while (**pgn && **pgn != '"') {
        if (**pgn == '\\') {
            ++*pgn;
        }
        *end++ = **pgn;
        ++*pgn;
    }

    if (**pgn != '"') {
        return NULL;
    }

    *end = '\0';
    ++*pgn;
    return begin;
}

static char *read_symbol(char **pgn) {
    skip_whitespace(pgn);

    char *begin = *pgn;
    if (!isalnum(**pgn)) {
        return NULL;
    }

    while (**pgn && (isalnum(**pgn) || strchr("_+#=:-", **pgn))) {
        ++*pgn;
    }

    **pgn = '\0';
    ++*pgn;
    return begin;
}

static char *read_tag(char **value, char **pgn) {
    skip_whitespace(pgn);
    if (**pgn != '[') {
        return NULL;
    }
    ++*pgn;

    char *name = read_symbol(pgn);
    if (!name) {
        return NULL;
    }

    *value = read_string(pgn);
    if (!*value) {
        return NULL;
    }

    skip_whitespace(pgn);
    if (**pgn != ']') {
        return NULL;
    }
    ++*pgn;

    return name;
}

static int read_tags(struct KeyValue *tags, char **pgn) {
    (void)tags;
    char *value = NULL;
    char *name  = read_tag(&value, pgn);
    (void)name;
    // while (name) {
    //     if (strcmp(name, "Event") == 0) {
    //         roster->event = value;
    //     } else if (strcmp(name, "Site") == 0) {
    //         roster->site = value;
    //     } else if (strcmp(name, "Date") == 0) {
    //         roster->date = value;
    //     } else if (strcmp(name, "Round") == 0) {
    //         roster->round = value;
    //     } else if (strcmp(name, "White") == 0) {
    //         roster->white = value;
    //     } else if (strcmp(name, "Black") == 0) {
    //         roster->black = value;
    //     } else if (strcmp(name, "Result") == 0) {
    //         roster->result = value;
    //     }
    //     value = NULL;
    //     name  = read_tag(&value, pgn);
    // }
    return 0;
}

static int read_move_number(char **pgn) {
    skip_whitespace(pgn);

    int n = 0;
    while (isdigit(**pgn)) {
        n = n * 10 + (**pgn - '0');
        ++*pgn;
    }

    skip_whitespace(pgn);
    while (**pgn == '.') {
        ++*pgn;
    }

    return n;
}

static int read_movetext(struct Game *game, char **pgn) {
    while (**pgn) {
        skip_whitespace(pgn);
        if (**pgn == ')') {
            return 0;
        }

        if (**pgn == '(') {
            struct List *history = list_copy(game->history);
            game_takeback(game);
            const int err = read_movetext(game, pgn);
            if (err) {
                return err;
            }
            if (**pgn != ')') {
                return 1;
            }
            game->history = history;
            continue;
        }

        (void)read_move_number(pgn);
        char *san = read_symbol(pgn);
        if (!san) {
            return 1;
        }

        const int err = game_move_san(game, san);
        if (err) {
            return err;
        }
    }
    return 0;
}

int game_read_pgn(struct Game *game, const char *pgn) {
    assert(game_valid(game));
    assert(pgn);

    char *working = strdup(pgn);
    int err = read_tags(game->tags, &working);
    if (!err) {
        err = read_movetext(game, &working);
    }
    free(working);
    return err;
}

struct Game *game_from_pgn(const char *pgn) {
    struct Game *game = game_from_fen(NULL);
    const int err = game_read_pgn(game, pgn);
    if (err) {
        return NULL;
    }
    return game;
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
