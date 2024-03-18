// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_san.h"
#include "chess.h"
#include "../utility/list.h"

#include <assert.h>
#include <ctype.h>
#include <string.h>

int
san_write_move(FILE *out, const struct Position *before, const struct Move *move)
{
    assert(out);
    assert(position_valid(before));
    assert(move_valid(move));

    const char piece = position_piece(before, move->from);
    if (piece_color(piece) != before->turn) {
        // Not a valid move
        return 1;
    }

    // Handle castling as a special case
    if ((piece == 'K' && move->from == E1 && move->to == G1) ||
        (piece == 'k' && move->from == E8 && move->to == G8))
    {
        fputs("O-O", out);
        return 0;
    }
    if ((piece == 'K' && move->from == E1 && move->to == C1) ||
        (piece == 'k' && move->from == E8 && move->to == C8))
    {
        fputs("O-O-O", out);
        return 0;
    }

    // Piece symbol
    if (piece != 'P' && piece != 'p') {
        fputc(toupper(piece), out);
    }

    // Look for ambiguity (can skip search for pawns)
    struct List *list = position_legal_moves(before);
    struct List *each = (piece != 'P' && piece != 'p') ? list->next : list;
    for (; each != list; each = each->next) {
        struct Move *mv = each->data;
        if (mv->to   == move->to   &&
            mv->from != move->from &&
            position_piece(before, mv->from) == piece)
        {
            // SAN is potentially ambiguous
            break;
        }
    }

    // Disambiguate from square
    if (each != list) {
        struct Move *mv = each->data;
        if (square_rank(mv->from) == square_rank(move->from)) {
            fputc(square_file(move->from), out);
        }
        if (square_file(mv->from) == square_file(move->from)) {
            fputc(square_rank(move->from), out);
        }
    }

    // Capture
    if (position_piece(before, move->to) != ' ') {
        if (piece == 'P' || piece == 'p') {
            // File required for pawn captures, b/c no piece symbol
            fputc(square_file(move->from), out);
        }
        fputc('x', out);
    }

    // To square
    fputc(square_file(move->to), out);
    fputc(square_rank(move->to), out);

    // Promotion
    if (move->promotion != ' ') {
        fputc('=', out);
        fputc(toupper(move->promotion), out);
    }

    return 0;
}

// Standard Algebraic Notation
int move_san(char *buf, int len, const struct Move *move) {
    assert(buf && len >= 0);
    assert(move_valid(move));

    if (len < 7) {
        // e.g., "dxe8=Q\0"
        return 0;
    }

    const struct Position *before = move->before;
    if (!before) {
        // Can't construct SAN without position
        return 0;
    }

    FILE *out = fmemopen(buf, len, "w");
    if (!out) {
        return 0;
    }

    const int err = san_write_move(out, before, move);
    fclose(out);
    return err ? 0 : strlen(buf);
}

const char *move_san_static(const struct Move *move) {
    static char buf[7];
    return move_san(buf, sizeof buf, move) > 0 ? buf : NULL;
}

// Standard Algebraic Notation
struct Move *move_from_san(const struct Position *before, const char *san) {
    assert(before && san);

    // Handle castling as a special case
    if (!strcmp(san, "O-O")) {
        return move_from_name(before->turn == 'w' ? "e1g1" : "e8g8");
    }
    if (!strcmp(san, "O-O-O")) {
        return move_from_name(before->turn == 'w' ? "e1c1" : "e8c8");
    }

    char piece     = 'P';
    char from_file = '\0';
    char from_rank = '\0';
    char to_file   = '\0';
    char to_rank   = '\0';
    char promotion = ' ';

    // Piece symbol
    if (*san && strchr("NBRQK", *san)) {
        piece = *san++;
    }
    piece = before->turn == 'w' ? piece : tolower(piece);

    // Optional disambiguation
    if (*san && strchr("abcdefgh", *san)) {
        from_file = *san++;
    }
    if (*san && strchr("12345687", *san)) {
        from_rank = *san++;
    }

    // Optional separator
    bool capture = false; // Used for move validation
    if (*san == '-' || *san == 'x') {
        if (piece == 'P' || piece == 'p') {
            if (!from_file) {
                // File required for pawn captures
                return NULL;
            }
        } else if (!from_file && !from_rank) {
            // Not valid SAN
            return NULL;
        }
        capture = *san++ == 'x';
    }

    // To square
    if (!*san || !strchr("abcdefgh", *san)) {
        // There was no disambiguation, we've already read the to square
        if (!from_file || !from_rank) {
            return NULL;
        }
        to_file = from_file;
        to_rank = from_rank;
        from_file = '\0';
        from_rank = '\0';
        goto promotion;
    }
    to_file = *san++;

    if (!*san || !strchr("12345678", *san)) {
        return NULL;
    }
    to_rank = *san++;

promotion:
    if (*san == '=') {
        ++san;
    }
    if (*san && strchr("NBRQ", *san)) {
        promotion = before->turn == 'w' ? *san++ : tolower(*san++);
    }
    if (*san && !isspace(*san)) {
        return NULL;
    }

    const int to = square(to_file, to_rank);
    struct Move *found = NULL;

    // Find matching move
    struct List *list = position_legal_moves(before);
    for (struct List *each = list->next; each != list; each = each->next) {
        struct Move *move = each->data;
        if (move->to == to &&
            position_piece(before, move->from) == piece &&
            (from_file == '\0'  || square_file(move->from) == from_file) &&
            (from_rank == '\0'  || square_rank(move->from) == from_rank) &&
            (promotion == ' ' || promotion == move->promotion))
        {
            if (found) {
                // Ambiguous
                found = NULL;
                break;
            }
            found = move;
        }
    }

    if (found && capture && position_piece(before, found->to) == ' ') {
        // Nothing captured
        found = NULL;
    }

    return found;
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
