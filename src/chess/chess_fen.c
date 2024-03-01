// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_fen.h"
#include "chess.h"

#include <assert.h>
#include <stdio.h>

const char *STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// Space for 8 ranks, 7 separators, and 1 terminator
static const int BOARD_FEN_MAX = 8 * 8 + 7 + 1;

// Return num chars written not including terminator
static int to_board_fen(const struct Board *board, char *fen, int len) {
    assert(board && fen && len >= BOARD_FEN_MAX);

    char *p  = fen;
    int   sq = A8;
    for (int r = 8; r >= 1; --r) {
        int empty = 0;
        for (char f = 'a'; f <= 'h'; ++f) {
            const enum Piece piece = board->squares[sq++];
            if (piece == EMPTY) {
                ++empty;
            } else {
                if (empty) {
                    *p++ = '0' + empty;
                    empty = 0;
                }
                *p++ = piece;
            }
        }
        if (empty) {
            *p++ = '0' + empty;
        }
        if (r > 1) {
            *p++ = '/';
        }
    }
    assert(sq == 64);

    *p = '\0';
    return p - fen;
}

// Return num chars consumed
static int from_board_fen(struct Board *board, const char *fen) {
    assert(board && fen);
    board_empty(board);

    const char *p = fen;
    int sq = A8;
    while (sq <= H1) {
        const char c = *p++;
        if (c == '/') {
            if (square_file(sq) != 'a') {
                // Misplaced separator
                return 0;
            }
            continue;
        }
        if ('1' <= c && c <= '8') {
            sq += c - '0';
        } else if (piece_valid(c)) {
            board->squares[sq++] = c;
        } else {
            // Invalid character
            return 0;
        }
    }
    return sq == 64 ? p - fen : 0;
}

const int FEN_MAX =
    /* board      */ 71 +  // 8x8 + 7 separators
    /* turn       */  2 +
    /* castle     */  5 +
    /* en passant */  3 +
    /* halfmove   */  3 +  // max value 99
    /* fullmove   */  4 +  // max value 269... so far
    /* terminator */  1;

int position_fen(const struct Position *position, char *fen, int len) {
    assert(position && fen && len >= FEN_MAX);

    struct Board board;
    board_from_mailbox(&board, &position->mailbox);

    char *p = fen;
    p += to_board_fen(&board, fen, len);

    *p++ = ' ';
    *p++ = position->turn;

    *p++ = ' ';
    if (position->castle & K_CASTLE) {
        *p++ = 'K';
    }
    if (position->castle & Q_CASTLE) {
        *p++ = 'Q';
    }
    if (position->castle & k_CASTLE) {
        *p++ = 'k';
    }
    if (position->castle & q_CASTLE) {
        *p++ = 'q';
    }

    *p++ = ' ';
    if (position->en_passant == INVALID_SQUARE) {
        *p++ = '-';
    } else {
        *p++ = square_file(position->en_passant);
        *p++ = '0' + square_rank(position->en_passant);
    }

    p += snprintf(p, 4, " %d", position->halfmove);
    p += snprintf(p, 5, " %d", position->fullmove);
    return p - fen;
}

int position_read_fen(struct Position *position, const char *fen) {
    assert(position && fen);
    const char *p = fen;

    struct Board board;
    int consumed = from_board_fen(&board, p);
    if (!consumed) {
        return 0;
    }

    mailbox_from_board(&position->mailbox, &board);
    p += consumed;

    if (*p++ != ' ') {
        return 0;
    }
    if (*p != 'w' && *p != 'b') {
        return 0;
    }
    position->turn = *p++;

    if (*p++ != ' ') {
        return 0;
    }
    position->castle = 0;
    while (*p != ' ') {
        switch (*p++) {
        case 'K': position->castle |= K_CASTLE; break;
        case 'Q': position->castle |= Q_CASTLE; break;
        case 'k': position->castle |= k_CASTLE; break;
        case 'q': position->castle |= q_CASTLE; break;
        default: return 0;
        }
    }

    if (*p++ != ' ') {
        return 0;
    }
    if (*p == '-') {
        position->en_passant = INVALID_SQUARE;
        ++p;
    } else {
        if (*p < 'a' || 'h' < *p) {
            return 0;
        }
        const char file = *p++;
        if (*p != '3' && *p != '6') {
            return 0;
        }
        const char rank = *p++;
        position->en_passant = square(file, rank);
    }

    if (*p++ != ' ') {
        return 0;
    }
    if (*p < '0' || '9' < *p) {
        return 0;
    }
    position->halfmove = 0;
    while ('0' <= *p && *p <= '9') {
        position->halfmove = 10 * position->halfmove + (*p++ - '0');
    }

    if (*p++ != ' ') {
        return 0;
    }
    if (*p < '1' || '9' < *p) {
        return 0;
    }
    position->fullmove = 0;
    while ('0' <= *p && *p <= '9') {
        position->fullmove = 10 * position->fullmove + (*p++ - '0');
    }

    return position->fullmove > 0 ? p - fen : 0;
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
