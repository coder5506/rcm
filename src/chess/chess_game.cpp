// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_game.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;
using namespace thc;

void Game::clear() {
    history.clear();
    started  = 0;
    rowid    = 0;
    settings = "";

    tags.clear();
    tags["Event"]  = "?";
    tags["Site"]   = "?";
    tags["Date"]   = "????.??.??";
    tags["Round"]  = "-";
    tags["White"]  = "?";
    tags["Black"]  = "?";
    tags["Result"] = "*";
}

void Game::fen(string_view fen) {
    clear();
    history.push_back(make_shared<Position>(fen));
}

bool Game::recover_history(PositionPtr target) {
    if (*target == *current()) {
        return true;
    }

    for (auto movepair : current()->moves_played) {
        history.push_back(movepair.second);
        if (recover_history(target)) {
            return true;
        }
        history.pop_back();
    }

    return false;
}

void Game::recover_position(string_view fen) {
    auto start = this->start();
    history.clear();
    history.push_back(start);
    if (!recover_history(make_shared<Position>(fen))) {
        throw std::domain_error("Invalid position");
    }
}

Game::Game(string_view pgn, string_view fen) {
    clear();
    this->observe(this);
    if (pgn.empty()) {
        // May be empty/start position
        this->fen(fen);
    } else {
        this->pgn(pgn);
        if (!fen.empty()) {
            // Identifies current node in graph
            recover_position(fen);
        }
    }
}

void Game::model_changed(Game&) {
    if (started > 0) {
        return;
    }

    started = time(nullptr);
    ostringstream date;
    date << put_time(localtime(&started), "%Y.%m.%d");
    tags["Date"] = date.str();
}

string& Game::tag(const string& key) {
    return tags[key];
}

PositionPtr Game::current() const {
    return history.back();
}

PositionPtr Game::previous() const {
    return history.size() > 1 ? history.at(history.size() - 2) : nullptr;
}

PositionPtr Game::start() const {
    return history.front();
}

char Game::at(thc::Square square) const {
    return current()->at(square);
}

string Game::fen() const {
    return current()->fen();
}

const MoveList& Game::legal_moves() const {
    return current()->legal_moves();
}

Move Game::san_move(string_view san_move) const {
    return const_cast<Position*>(current().get())->san_move(san_move);
}

Move Game::uci_move(string_view uci_move) const {
    return const_cast<Position*>(current().get())->uci_move(uci_move);
}

void Game::play_move(Move move) {
    history.push_back(current()->play_move(move));
    changed();
}

void Game::play_san_move(string_view san_move) {
    play_move(this->san_move(san_move));
}

void Game::play_uci_move(string_view uci_move) {
    play_move(this->uci_move(uci_move));
}

void Game::play_takeback() {
    if (history.size() > 1) {
        history.pop_back();
        changed();
    }
}

void Game::play_takeback(Move takeback) {
    if (previous()->move_played(takeback)) {
        play_takeback();
    }
}

// Here we try to interpret the move at a higher-level than the position,
// taking into account the context of the game.  So yes, the boardstate
// might represent a move or a move in-progress, but it could also be the
// completion of a two-part move (i.e., castling rook first) or a takeback.
bool Game::read_move(
    Bitmap            boardstate,
    const ActionList& actions,
    MoveList&         candidates,
    std::optional<thc::Move>& takeback)
{
    candidates.clear();
    takeback.reset();

    // If boardstate matches current position, there's no move to read.
    if (bitmap() == boardstate) {
        // i.e., not an error
        return true;
    }

    // Can boardstate be reached by a legal move?
    bool maybe_valid = current()->read_move(boardstate, actions, candidates);
    if (maybe_valid) {
        return true;
    }

    // // Is this the completion of a castling move?
    // struct List     *castling = list_new();
    // struct Position *previous = game_previous(game);
    // if (previous) {
    //     generate_castle_moves(castling, previous);
    // }
    // for (struct List *each = castling->next; each != castling; each = each->next) {
    //     struct Position *after = position_apply_move(previous, (struct Move*)each->data);
    //     if (!position_legal(after)) {
    //         continue;
    //     }
    //     if (after->bitmap != boardstate) {
    //         maybe_valid = maybe_valid || position_incomplete(after, boardstate);
    //         continue;
    //     }

    //     // Find takeback move
    //     struct List *it = list_begin(previous->moves_played);
    //     for (; it != list_end(previous->moves_played); it = it->next) {
    //         struct Move *move = (struct Move*)it->data;
    //         if (position_equal(move->after, current)) {
    //             *takeback = move;
    //             break;
    //         }
    //     }
    //     assert(*takeback);

    //     // Undo previous move and apply castling move
    //     list_push(candidates, each->data);

    //     return true;
    // }

    // // Is this a takeback?
    // if (previous && previous->bitmap == boardstate) {
    //     struct List *each = previous->moves_played->next;
    //     for (; each != previous->moves_played; each = each->next) {
    //         struct Move *move = (struct Move*)each->data;
    //         if (position_equal(move->after, current)) {
    //             *takeback = move;
    //             return true;
    //         }
    //     }
    // }

    // // Possibly a takeback in progress?
    // return previous && position_incomplete(previous, boardstate);

    return false;
}

//
// Write PGN
//

void Game::write_tags(ostream& out) const {
    for (const auto& tag : tags) {
        out << "[" << tag.first << " \"" << tag.second << "\"]\n";
    }
    out << "\n";
}

void Game::write_move(
    ostream&    out,
    PositionPtr before,
    Move        move,
    bool        show_move_number) const
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
    out << const_cast<Position*>(before.get())->move_san(move);
}

void Game::write_moves(ostream& out, PositionPtr before, bool is_first_move) const {
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

void Game::write_movetext(ostream& out) const {
    write_moves(out, start(), true);
}

void Game::write_pgn(ostream& out) const {
    write_tags(out);
    write_movetext(out);
}

string Game::pgn() const {
    ostringstream pgn;
    write_pgn(pgn);
    return pgn.str();
}

//
// Read PGN
//

static void skip_whitespace(char*& pgn) {
    while (std::isspace(*pgn)) {
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
    if (!std::isalpha(*pgn)) {
        return nullptr;
    }

    auto begin = pgn;
    while (*pgn && (std::isalnum(*pgn) || std::strchr("_+#=:-", *pgn))) {
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

void Game::read_tags(char*& pgn) {
    tags.clear();

    char* value = nullptr;
    char* name  = read_tag(&value, pgn);
    while (name) {
        tags[name] = value;
        value = nullptr;
        name  = read_tag(&value, pgn);
    }
}

static int read_move_number(char*& pgn) {
    skip_whitespace(pgn);

    auto n = 0;
    while (std::isdigit(*pgn)) {
        n = n * 10 + (*pgn++ - '0');
    }

    skip_whitespace(pgn);
    while (*pgn == '.') {
        ++pgn;
    }

    return n;
}

bool Game::read_movetext(char*& pgn) {
    while (*pgn) {
        skip_whitespace(pgn);
        if (*pgn == ')') {
            return true;
        }

        if (*pgn == '(') {
            auto save_history = history;
            play_takeback();
            ++pgn;
            if (!read_movetext(pgn)) {
                return false;
            }
            if (*pgn != ')') {
                return false;
            }
            ++pgn;
            history = save_history;
            continue;
        }

        (void)read_move_number(pgn);
        auto san = read_symbol(pgn);
        if (!san) {
            return false;
        }

        try {
            play_san_move(san);
        }
        catch (const std::logic_error&) {
            return false;
        }
    }
    return true;
}

void Game::pgn(string_view pgn) {
    clear();
    history.push_back(make_shared<Position>());

    auto copy = strdup(pgn.data());
    auto working = copy;
    read_tags(working);
    auto ok = read_movetext(working);
    free(copy);
    if (!ok) {
        throw std::domain_error("Invalid PGN");
    }
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
