// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_fen.h"
#include "chess.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gc/gc.h>

const char *STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

static void fen_write_board(FILE *out, const struct Board *board) {
    assert(out && board);

    int sq = A8;
    for (char r = '8'; r >= '1'; --r) {
        int empty = 0;
        for (char f = 'a'; f <= 'h'; ++f) {
            const enum Piece piece = board->squares[sq++];
            if (piece == EMPTY) {
                ++empty;
            } else {
                if (empty) {
                    fputc('0' + empty, out);
                    empty = 0;
                }
                fputc(piece, out);
            }
        }
        if (empty) {
            fputc('0' + empty, out);
        }
        if (r > '1') {
            fputc('/', out);
        }
    }
    assert(sq == 64);
}

static void fen_write(FILE *out, const struct Position *position) {
    assert(out);
    assert(position_valid(position));

    struct Board board;
    board_from_mailbox(&board, &position->mailbox);
    fen_write_board(out, &board);

    fputc(' ', out);
    fputc(position->turn, out);

    fputc(' ', out);
    if (position->castle & K_CASTLE) {
        fputc('K', out);
    }
    if (position->castle & Q_CASTLE) {
        fputc('Q', out);
    }
    if (position->castle & k_CASTLE) {
        fputc('k', out);
    }
    if (position->castle & q_CASTLE) {
        fputc('q', out);
    }

    fputc(' ', out);
    if (position->en_passant == INVALID_SQUARE) {
        fputc('-', out);
    } else {
        fputc(square_file(position->en_passant), out);
        fputc(square_rank(position->en_passant), out);
    }

    fprintf(out, " %d", position->halfmove);
    fprintf(out, " %d", position->fullmove);
}

char *position_fen(const struct Position *position) {
    assert(position_valid(position));

    char  *buf = NULL;
    size_t len = 0;
    FILE *out = open_memstream(&buf, &len);
    if (!out) {
        return NULL;
    }

    fen_write(out, position);
    fclose(out);
    if (buf == NULL || len == 0) {
        return NULL;
    }

    // Move to GC heap
    char *result = GC_MALLOC(len + 1);
    strncpy(result, buf, len + 1);
    free(buf);
    return result;
}

char *game_fen(const struct Game *game) {
    return position_fen(game_current((struct Game*)game));
}

void game_save_fen(const struct Game *game, const char *filename) {
    assert(game_valid(game));

    FILE *out = fopen(filename, "w");
    if (out) {
        fprintf(out, "%s\n", game_fen(game));
        fclose(out);
    }
}

static int fen_read_board(struct Board *board, FILE *in) {
    assert(board && in);

    board_empty(board);
    int sq = A8;
    while (sq <= H1) {
        const int c = fgetc(in);
        if (c == '/') {
            if (square_file(sq) != 'a') {
                // Misplaced separator
                return 1;
            }
            continue;
        }
        if ('1' <= c && c <= '8') {
            sq += c - '0';
        } else if (piece_valid(c)) {
            board->squares[sq++] = c;
        } else {
            // Invalid character
            return 1;
        }
    }
    return sq == 64 ? 0 : 1;
}

static int fen_read(struct Position *position, FILE *in) {
    assert(position && in);

    struct Board board;
    const int err = fen_read_board(&board, in);
    if (err) {
        return err;
    }
    if (fgetc(in) != ' ') {
        return 1;
    }

    mailbox_from_board(&position->mailbox, &board);

    int c = fgetc(in);
    if (c != 'w' && c != 'b') {
        return 1;
    }
    if (fgetc(in) != ' ') {
        return 1;
    }
    position->turn = c;

    position->castle = 0;
    while ((c = fgetc(in)) != ' ') {
        switch (c) {
        case 'K': position->castle |= K_CASTLE; break;
        case 'Q': position->castle |= Q_CASTLE; break;
        case 'k': position->castle |= k_CASTLE; break;
        case 'q': position->castle |= q_CASTLE; break;
        default:
            return 1;
        }
    }
    assert(c == ' ');

    c = fgetc(in);
    if (c == '-') {
        position->en_passant = INVALID_SQUARE;
    } else {
        if (c < 'a' || 'h' < c) {
            return 1;
        }
        const char file = c;
        c = fgetc(in);
        if (c != '3' && c != '6') {
            return 1;
        }
        const char rank = c;
        position->en_passant = square(file, rank);
    }
    if (fgetc(in) != ' ') {
        return 1;
    }

    c = fgetc(in);
    if (c < '0' || '9' < c) {
        return 1;
    }
    position->halfmove = 0;
    while ('0' <= c && c <= '9') {
        position->halfmove = 10 * position->halfmove + (c - '0');
        c = fgetc(in);
    }
    if (c != ' ') {
        return 1;
    }

    c = fgetc(in);
    if (c < '1' || '9' < c) {
        return 1;
    }
    position->fullmove = 0;
    while ('0' <= c && c <= '9') {
        position->fullmove = 10 * position->fullmove + (c - '0');
        c = fgetc(in);
    }

    return position->fullmove > 0 ? 0 : 1;
}

int position_read_fen(struct Position *position, const char *fen) {
    assert(position && fen);

    FILE *in = fmemopen((void *)fen, strlen(fen), "r");
    if (!in) {
        return 1;
    }

    const int err = fen_read(position, in);
    fclose(in);
    return err;
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
