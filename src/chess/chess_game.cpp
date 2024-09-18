// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_game.h"
#include "chess_pgn.h"
#include "../thc/gen.h"

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
    if (previous()->move_played(takeback) == current()) {
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


/****************************************************************************
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

// Check insufficient material draw rule
bool Game::IsInsufficientDraw(bool white_asks, DRAWTYPE& result) const {
    auto   piece_count=0;
    auto   bishop_or_knight = false, lone_wking = true, lone_bking = true;
    auto   draw = false;

    auto curr = current();
    for (auto square = a8; square <= h1; ++square) {
        const auto piece = curr->at(square);
        switch (piece) {
        case 'B': case 'b':
        case 'N': case 'n': bishop_or_knight = true;  // fallthrough
        case 'Q': case 'q':
        case 'R': case 'r':
        case 'P': case 'p':
            piece_count++;
            if (IsWhite(piece)) {
                lone_wking = false;
            }
            else {
                lone_bking = false;
            }
            break;
        }
    }

    // Automatic draw if K v K or K v K+N or K v K+B
    //  (note that K+B v K+N etc. is not auto granted due to
    //   selfmates in the corner)
    if (piece_count == 0 || (piece_count == 1 && bishop_or_knight)) {
        draw = true;
        result = DRAWTYPE_INSUFFICIENT_AUTO;
    }
    else {
        // Otherwise side playing against lone K can claim a draw
        if (white_asks && lone_bking) {
            draw   = true;
            result = DRAWTYPE_INSUFFICIENT;
        }
        else if (!white_asks && lone_wking) {
            draw   = true;
            result = DRAWTYPE_INSUFFICIENT;
        }
    }
    return draw;
}


// Get number of times position has been repeated
int Game::GetRepetitionCount() const {
    auto matches = 0;

    // Search backwards ....
    auto begin = history.rbegin();
    auto current = *begin;
    for (; begin != history.rend(); ++begin) {
        auto previous = *begin;
        // ... looking for matching positions
        if (previous->white          == current->white          &&  // quick ones first!
            previous->d.wking_square == current->d.wking_square &&
            previous->d.bking_square == current->d.bking_square &&
            memcmp(previous->squares, current->squares, sizeof current->squares) == 0)
        {
            matches++;
            if (previous->d == current->d) {  // Castling flags and/or enpassant target different?
                continue;
            }

            // It might not be a match (but it could be - we have to unpack what
            // the differences really mean)
            auto revoke_match = false;

            // Revoke match if different value of en-passant target square means
            // different en passant possibilities
            if (previous->d.enpassant_target != current->d.enpassant_target) {
                int ep_saved = current->d.enpassant_target;
                int ep_now   = previous->d.enpassant_target;

                // Work out whether each en_passant is a real one, i.e. is there
                // an opposition pawn in place to capture (if not it's just a
                // double pawn advance with no actual enpassant consequences)
                auto real=false;
                auto ep = ep_saved;
                char const *squ = current->squares;
                for (auto j = 0; j < 2; j++) {
                    if (ep == a6) {
                        real = (squ[SE(ep)] == 'P');
                    }
                    else if (b6 <= ep && ep <= g6) {
                        real = (squ[SW(ep)] == 'P' || squ[SE(ep)] == 'P');
                    }
                    else if (ep == h6) {
                        real = (squ[SW(ep)] == 'P');
                    }
                    else if (ep == a3) {
                        real = (squ[NE(ep)] == 'p');
                    }
                    else if (b3 <= ep && ep <= g3) {
                        real = (squ[NE(ep)] == 'p' || squ[NW(ep)] == 'p');
                    }
                    else if (ep == h3) {
                        real = (squ[NW(ep)] == 'p' );
                    }
                    if (j > 0) {
                        ep_now = real ? ep : SQUARE_INVALID;    // evaluate second time through
                    }
                    else {
                        ep_saved = real ? ep : SQUARE_INVALID;  // evaluate first time through
                        ep = ep_now;                            // setup second time through
                        squ = previous->squares;
                        real = false;
                    }
                }

                // If for example one en_passant is real and the other not, it's
                // not a real match
                if (ep_saved != ep_now) {
                    revoke_match = true;
                }
            }

            // Revoke match if different value of castling flags means different
            //  castling possibilities
            if (!revoke_match && !eq_castling(previous->d, current->d)) {
                bool wking_saved  = current->squares[e1]=='K' && current->squares[h1]=='R' && current->d.wking();
                bool wking_now    = previous->squares[e1]=='K' && previous->squares[h1]=='R' && previous->d.wking();
                bool bking_saved  = current->squares[e8]=='k' && current->squares[h8]=='r' && current->d.bking();
                bool bking_now    = previous->squares[e8]=='k' && previous->squares[h8]=='r' && previous->d.bking();
                bool wqueen_saved = current->squares[e1]=='K' && current->squares[a1]=='R' && current->d.wqueen();
                bool wqueen_now   = previous->squares[e1]=='K' && previous->squares[a1]=='R' && previous->d.wqueen();
                bool bqueen_saved = current->squares[e8]=='k' && current->squares[a8]=='r' && current->d.bqueen();
                bool bqueen_now   = previous->squares[e8]=='k' && previous->squares[a8]=='r' && previous->d.bqueen();
                revoke_match = (
                    wking_saved  != wking_now  ||
                    bking_saved  != bking_now  ||
                    wqueen_saved != wqueen_now ||
                    bqueen_saved != bqueen_now
                );
            }

            // If the real castling or enpassant possibilities differ, it's not
            // a match.  At one stage we just did a naive binary match of the
            // details - not good enough. For example a rook moving away from h1
            // doesn't affect the WKING flag, but does disallow white king side
            // castling
            if (revoke_match) {
                matches--;
            }
        }

        // For performance reasons, abandon search early if pawn move or capture
        // if( squares[m.src]=='P' || squares[m.src]=='p' || !IsEmptySquare(m.capture) )
        //     break;
    }

    return matches+1;  // +1 counts original position
}


// Check draw rules (50 move rule etc.)
bool Game::IsDraw(bool white_asks, DRAWTYPE& result) const {
    // Insufficient mating material
    if (auto draw = IsInsufficientDraw(white_asks, result)) {
        return draw;
    }

    // 50 move rule
    if (current()->half_move_clock >= 100) {
        result = DRAWTYPE_50MOVE;
        return true;
    }

    // 3 times repetition,
    if (GetRepetitionCount() >= 3) {
        result = DRAWTYPE_REPITITION;
        return true;
    }

    result = NOT_DRAW;
    return false;
}


// Test for legal position, sets reason to a mask of possibly multiple reasons
bool Game::IsLegal(ILLEGAL_REASON& reason) const {
    auto  ireason = 0;
    auto  wkings = 0, bkings = 0, wpawns = 0, bpawns = 0, wpieces = 0, bpieces = 0;
    auto legal = true;
    Square opposition_king_location = SQUARE_INVALID;

    auto curr = current();
    for (auto sq = a8; sq <= h1; ++sq) {
        const auto p = curr->at(sq);
        if ((p == 'P' || p == 'p') && (RANK(sq) == '1' || RANK(sq) == '8')) {
            legal = false;
            ireason |= IR_PAWN_POSITION;
        }
        if (IsWhite(p)) {
            if (p == 'P') {
                wpawns++;
            }
            else {
                wpieces++;
                if (p == 'K') {
                    wkings++;
                    if (curr->BlackToPlay()) {
                        opposition_king_location = sq;
                    }
                }
            }
        }
        if (IsBlack(p)) {
            if (p == 'p') {
                bpawns++;
            }
            else {
                bpieces++;
                if (p == 'k') {
                    bkings++;
                    if (curr->WhiteToPlay()) {
                        opposition_king_location = sq;
                    }
                }
            }
        }
    }

    if (wkings != 1 || bkings != 1) {
        legal = false;
        ireason |= IR_NOT_ONE_KING_EACH;
    }
    if (opposition_king_location != SQUARE_INVALID && gen::AttackedPiece(*curr, opposition_king_location)) {
        legal = false;
        ireason |= IR_CAN_TAKE_KING;
    }
    if (wpieces > 8 && wpieces + wpawns > 16) {
        legal = false;
        ireason |= IR_WHITE_TOO_MANY_PIECES;
    }
    if (bpieces > 8 && bpieces + bpawns > 16) {
        legal = false;
        ireason |= IR_BLACK_TOO_MANY_PIECES;
    }
    if (wpawns > 8) {
        legal = false;
        ireason |= IR_WHITE_TOO_MANY_PAWNS;
    }
    if (bpawns > 8) {
        legal = false;
        ireason |= IR_BLACK_TOO_MANY_PAWNS;
    }

    reason = static_cast<ILLEGAL_REASON>(ireason);
    return legal;
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
