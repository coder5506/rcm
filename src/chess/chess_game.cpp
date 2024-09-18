// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_game.h"
#include "chess_pgn.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;
using namespace thc;


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


void Game::on_changed(Game&) {
    if (started > 0) {
        return;
    }

    started = time(nullptr);
    ostringstream date;
    date << put_time(localtime(&started), "%Y.%m.%d");
    tags["Date"] = date.str();
}


// Reset game to initial state
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


// Set start position from FEN string
void Game::fen(string_view fen) {
    clear();
    history.push_back(make_shared<Position>(fen));
}


void Game::pgn(string_view pgn) {
    clear();
    history.push_back(make_shared<Position>());

    auto copy = strdup(pgn.data());
    auto working = copy;
    pgn::read_tags(working, *this);
    auto ok = pgn::read_movetext(working, *this);
    free(copy);
    if (!ok) {
        throw domain_error("Invalid PGN");
    }
}


string Game::fen() const {
    return current()->fen();
}


string Game::pgn() const {
    ostringstream pgn;
    pgn::write_pgn(pgn, *this);
    return pgn.str();
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


char Game::at(Square square) const {
    return current()->at(square);
}


Move Game::san_move(string_view san_move) const {
    return current()->san_move(san_move);
}


Move Game::uci_move(string_view uci_move) const {
    return current()->uci_move(uci_move);
}


void Game::play_move(Move move) {
    history.push_back(current()->play_move(move));
    changed();
}


// Play move given in Standard Algebraic Notation
void Game::play_san_move(string_view san_move) {
    play_move(this->san_move(san_move));
}


// Play move given in pure coordinate notation (as in UCI)
void Game::play_uci_move(string_view uci_move) {
    play_move(this->uci_move(uci_move));
}


// Non-validating: just updates game to use previous position
void Game::play_takeback() {
    if (history.size() > 1) {
        history.pop_back();
        changed();
    }
}


// Validating: takeback must match move played
void Game::play_takeback(Move takeback) {
    if (previous()->move_played(takeback)) {
        play_takeback();
    }
}


// *Corrects* the last move.  We're not taking back one move and playing
// another, we're asserting the last move should be stricken from the record.
// This is really only an issue with castling rook-first, which can
// legitimately be taken as a rook move until the player moves the king.
void Game::revise_move(Move takeback, Move move) {
    play_takeback(takeback);
    current()->remove_move_played(takeback);
    play_move(move);
}


MoveList Game::legal_moves() const {
    return current()->legal_moves();
}


// Here we try to interpret the move at a higher-level than the position,
// taking into account the context of the game.  So yes, the boardstate
// might represent a move or a move in-progress, but it could also be the
// completion of a two-part move (i.e., castling rook first) or a takeback.
//
// Input:
//   boardstate -- Map of currently occupied squares
//   actions    -- Recent lift/place actions by player
// Output:
//   candidates -- List of candidate moves
//   takeback   -- Possible takeback move
//   Note that typically the output (if any) will be either a single candidate
//   or a takeback.  Candidates will be a list only in the case of promotions,
//   where the user will need to indicate the desired piece.  Both a candidate
//   and a takeback will be provided when we need to revise the previous move.
// Return:
//   true  -- Boardstate is a valid position or could be an in-progress move
//   false -- Boardstate is not reachable by a legal move
bool Game::read_move(
    Bitmap               boardstate,
    const ActionHistory& actions,
    MoveList&            candidates,
    optional<Move>&      takeback)
{
    candidates.clear();
    takeback.reset();

    // If boardstate matches current position, there's no move to read.
    if (bitmap() == boardstate) {
        return true;
    }

    // Can boardstate be reached by a legal move?
    if (current()->read_move(boardstate, actions, candidates)) {
        return true;
    }

    // Is this the completion of a castling move?
    if (read_revised_move(boardstate, candidates, takeback)) {
        return true;
    }

    // Otherwise, maybe it's a takeback or restoration of a previous position.
    return read_takeback(boardstate, takeback);
}


string& Game::tag(const string& key) {
    return tags[key];
}


// Because we support takebacks and multiple variations, the position on the
// board is not necessarily the result of the last move in the PGN.  So given
// both the PGN for the game and the FEN for a specific position, we search
// through all positions in the game to select the position matching FEN.
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
        throw domain_error("Invalid position");
    }
}

// Is this the completion of a castling move?
bool Game::read_revised_move(
    Bitmap          boardstate,
    MoveList&       candidates,
    optional<Move>& takeback)
{
    auto maybe_valid = false;

    if (auto before = previous()) {
        for (auto castle : before->castle_moves()) {
            const auto after = before->apply_move(castle);
            if (after->bitmap() != boardstate) {
                // Castling may still be in-progress
                maybe_valid = maybe_valid || before->incomplete(boardstate, *after);
                continue;
            }

            // Find takeback move
            takeback = before->find_move_played(current());
            assert(takeback.has_value());

            // Undo previous move and apply castling move
            candidates.push_back(castle);
            return true;
        }
    }

    return maybe_valid;
}


bool Game::read_simple_takeback(Bitmap boardstate, optional<Move>& takeback) {
    if (auto before = previous()) {
        // Does boardstate match some previous position?
        if (before->bitmap() == boardstate) {
            // Takeback only most recent move
            takeback = before->find_move_played(current());
            return true;
        }

        // Possibly a takeback in progress?
        return before->incomplete(boardstate, *current());
    }
    return false;
}


bool Game::read_multiple_takeback(Bitmap boardstate, optional<Move>& takeback) {
    auto after  = history.rbegin();  // i.e., current()
    auto before = after + 1;         // i.e., previous()
    for (; before != history.rend(); ++after, ++before) {
        // Does boardstate match some previous position?
        if ((*before)->bitmap() == boardstate) {
            // Takeback only most recent move
            takeback = previous()->find_move_played(current());
            return true;
        }
    }

    return false;
}


// Is this a takeback?  If boardstate matches any previous position, takeback
// one move.  Multiple iterations will eventually get us back to the position in
// question.  It is safe to do this search because we consider all other
// possibilities before trying this one: we won't mistake a repeated position
// for a takeback.
//
// The reason for considering any previous position is so that in analysis, a
// player can quickly rearrange the pieces without bothering about move-order.
bool Game::read_takeback(Bitmap boardstate, optional<Move>& takeback) {
    if (read_simple_takeback(boardstate, takeback)) {
        return true;
    }
    return read_multiple_takeback(boardstate, takeback);
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
