// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_pgn.h"
#include "chess_game.h"

#include <cstring>
#include <iostream>

using namespace std;
using namespace thc;


//
// Write PGN
//

static void write_tags(ostream& out, const Game& game) {
    for (const auto& tag : game.tags) {
        out << "[" << tag.first << " \"" << tag.second << "\"]\n";
    }
    out << "\n";
}


static void write_move(
    ostream&    out,
    PositionPtr before,
    Move        move,
    bool        show_move_number)
{
    if (!show_move_number && before->WhiteToPlay()) {
        out << " ";
    }

    if (before->WhiteToPlay()) {
        out << before->full_move_count << ". ";
    } else if (show_move_number) {
        out << before->full_move_count << "... ";
    } else {
        out << " ";
    }
    out << before->move_san(move);
}


static void write_moves(ostream& out, PositionPtr before, bool is_first_move) {
    auto show_move_number = is_first_move;
    while (before) {
        auto begin = before->moves_played.cbegin();
        if (begin == before->moves_played.cend()) {
            break;
        }

        auto movepair = *begin;
        write_move(out, before, movepair.first, show_move_number);
        show_move_number = false;

        ++begin;
        for (; begin != before->moves_played.cend(); ++begin) {
            out << " (";
            auto variation = *begin;
            write_move(out, before, variation.first, true);
            write_moves(out, variation.second, false);
            out << ") ";
            show_move_number = true;
        }

        before = movepair.second;
    }
}


static void write_movetext(ostream& out, const Game& game) {
    write_moves(out, game.start(), true);
}


void pgn::write_pgn(ostream& out, const Game& game) {
    write_tags(out, game);
    write_movetext(out, game);
}


//
// Read PGN
//

static void skip_whitespace(char*& pgn) {
    while (isspace(*pgn)) {
        ++pgn;
    }
}


static char* read_string(char*& pgn) {
    skip_whitespace(pgn);
    if (*pgn != '"') {
        return nullptr;
    }
    ++pgn;

    auto begin = pgn;
    auto end   = begin;
    while (*pgn && *pgn != '"') {
        if (*pgn == '\\') {
            ++pgn;
        }
        *end++ = *pgn++;
    }

    if (*pgn != '"') {
        return nullptr;
    }

    *end = '\0';
    ++pgn;
    return begin;
}


static char* read_symbol(char*& pgn) {
    skip_whitespace(pgn);
    if (!isalpha(*pgn)) {
        return nullptr;
    }

    auto begin = pgn;
    while (*pgn && (isalnum(*pgn) || strchr("_+#=:-", *pgn))) {
        ++pgn;
    }

    return begin;
}


static char* read_tag(char** value, char*& pgn) {
    skip_whitespace(pgn);
    if (*pgn != '[') {
        return nullptr;
    }
    ++pgn;

    auto name = read_symbol(pgn);
    if (!name) {
        return nullptr;
    }

    *value = read_string(pgn);
    if (!*value) {
        return nullptr;
    }

    skip_whitespace(pgn);
    if (*pgn != ']') {
        return nullptr;
    }
    ++pgn;

    return name;
}


void pgn::read_tags(char*& pgn, Game& game) {
    game.tags.clear();

    char* value = nullptr;
    char* name  = read_tag(&value, pgn);
    while (name) {
        game.tags[name] = value;
        value = nullptr;
        name  = read_tag(&value, pgn);
    }
}


static int read_move_number(char*& pgn) {
    skip_whitespace(pgn);

    auto n = 0;
    while (isdigit(*pgn)) {
        n = n * 10 + (*pgn++ - '0');
    }

    skip_whitespace(pgn);
    while (*pgn == '.') {
        ++pgn;
    }

    return n;
}


bool pgn::read_movetext(char*& pgn, Game& game) {
    while (*pgn) {
        skip_whitespace(pgn);
        if (*pgn == ')') {
            return true;
        }

        if (*pgn == '(') {
            auto save_history = game.history;
            game.play_takeback();
            ++pgn;
            if (!read_movetext(pgn, game)) {
                return false;
            }
            if (*pgn != ')') {
                return false;
            }
            ++pgn;
            game.history = save_history;
            continue;
        }

        (void)read_move_number(pgn);
        auto san = read_symbol(pgn);
        if (!san) {
            return false;
        }

        try {
            game.play_san_move(san);
        }
        catch (const logic_error&) {
            return false;
        }
    }
    return true;
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
