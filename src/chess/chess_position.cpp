// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_position.h"

#include <algorithm>
#include <cassert>
#include <cstring>

using namespace std;
using namespace thc;

bool operator==(const Position& lhs, const Position& rhs) {
    return lhs.white == rhs.white &&
        lhs.half_move_clock == rhs.half_move_clock &&
        lhs.full_move_count == rhs.full_move_count &&
        lhs.d == rhs.d &&
        memcmp(lhs.squares, rhs.squares, sizeof lhs.squares) == 0;
}

Position::Position(string_view fen) {
    if (!fen.empty()) {
        Forsyth(fen.data());
    }
}

PositionPtr Position::move_played(Move move) const {
    // Find move in moves_played
    auto existing = find_if(
        moves_played.begin(),
        moves_played.end(),
        [move](const MovePair& pair) {
            return pair.first == move;
        }
    );
    return existing != moves_played.end() ? existing->second : nullptr;
}

PositionPtr Position::play_move(Move move) const {
    if (auto existing = move_played(move)) {
        return existing;
    }

    auto after = make_shared<Position>(*this);
    after->moves_played.clear();
    after->PlayMove(move);
    moves_played.push_back({move, after});
    return after;
}

optional<Move> Position::find_move_played(PositionPtr after) const {
    // Find move in moves_played
    auto existing = find_if(
        moves_played.begin(),
        moves_played.end(),
        [after](const MovePair& pair) {
            return pair.second == after;
        }
    );
    if (existing != moves_played.end()) {
        return existing->first;
    }
    else {
        return nullopt;
    }
}

void Position::remove_move_played(Move move) const {
    moves_played.erase(
        remove_if(
            moves_played.begin(),
            moves_played.end(),
            [move](const MovePair& pair) {
                return pair.first == move;
            }
        ),
        moves_played.end()
    );
}

Bitmap Position::bitmap() const {
    Bitmap bitmap{0};
    Bitmap mask{1};
    for (auto sq = a8; sq <= h1; ++sq) {
        if (!IsEmptySquare(at(sq))) {
            bitmap |= mask;
        }
        mask <<= 1;
    }
    return bitmap;
}

// Bitmap of the differences between two positions (*not* the difference of
// their bitmaps!), so takes into account when a square is occupied by a
// different piece
Bitmap Position::difference_bitmap(const Position& other) const {
    Bitmap bitmap{0};
    Bitmap mask{1};
    for (auto sq = a8; sq <= h1; ++sq) {
        if (at(sq) != other.at(sq)) {
            bitmap |= mask;
        }
        mask <<= 1;
    }
    return bitmap;
}

// Legal moves in this position
MoveList Position::legal_moves() const {
    MoveList moves;
    const_cast<Position*>(this)->GenLegalMoveList(moves);
    return moves;
}

// Note: does not check legality of castling moves, only availability
MoveList Position::castle_moves() const {
    MoveList king_moves;
    if (WhiteToPlay() && d.wking_square == e1) {
        const_cast<Position*>(this)->KingMoves(king_moves, e1);
    }
    else if (BlackToPlay() && d.bking_square == e8) {
        const_cast<Position*>(this)->KingMoves(king_moves, e8);
    }

    MoveList result;
    for (auto move : king_moves) {
        if (SPECIAL_WK_CASTLING <= move.special && move.special <= SPECIAL_BQ_CASTLING) {
            result.push_back(move);
        }
    }
    return result;
}

// A boardstate might represent a transition into a new position only if the
// differences between the boardstate and the resulting position are confined
// to those squares that differ between the two positions.
//
// Or, in other words, if the user has lifted or placed some completely
// unrelated piece, it's not a transition.
bool Position::incomplete(Bitmap boardstate, const Position& after) const {
    const auto diff = boardstate ^ bitmap();
    return (diff & ~difference_bitmap(after)) == 0;
}

// Construct list of candidate moves in this position that match the given
// boardstate.  The return indicates if there are any viable candidates:
// - true if any candidates are found OR if the boardstate could be in
//   transition to a valid move,
// - false if the boardstate is incompatible with all legal moves in this
//   position.
bool Position::read_moves(Bitmap boardstate, MoveList& candidates) const {
    auto maybe_valid = false;
    Position after{*this};

    candidates.clear();
    for (auto move : legal_moves()) {
        after.PushMove(move);
        if (after.bitmap() == boardstate) {
            candidates.push_back(move);
            maybe_valid = true;
        }
        else if (!maybe_valid) {
            maybe_valid = incomplete(boardstate, after);
        }
        after.PopMove(move);
    }

    assert(maybe_valid || candidates.empty());
    return maybe_valid;
}

// The boardstate alone can be ambiguous in the case of captures.  Here we
// use the actions history to disambiguate the move.  Note that we still
// generate a list because the candidates might be promotions which cannot be
// resolved here.
bool Position::read_move(
    Bitmap            boardstate,
    const ActionList& actions,
    MoveList&         candidates) const
{
    candidates.clear();

    MoveList local_candidates;
    const auto maybe_valid = read_moves(boardstate, local_candidates);
    if (local_candidates.empty()) {
        // Nothing to disambiguate.
        return maybe_valid;
    }
    assert(maybe_valid);

    MoveList captures;
    for (auto move : local_candidates) {
        if (move.capture == ' ') {
            candidates.push_back(move);
        }
        else {
            captures.push_back(move);
        }
    }

    if (captures.size() == 1) {
        // Not ambiguous, no need to consult history
        candidates.push_back(captures.front());
    }
    if (captures.size() <= 1) {
        return true;
    }

    // For a capture, history should show a place on the target square
    for (auto move : captures) {
        for (auto p = actions.rbegin(); p != actions.rend(); ++p) {
            if (p->lift == move.dst) {
                // We're interested in only the most recent action on square.
                // If the place was followed by a lift, then it wasn't our
                // capture.
                break;
            }
            if (p->place == move.dst) {
                candidates.push_back(move);
                break;
            }
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
