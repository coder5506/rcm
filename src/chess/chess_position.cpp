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

const MoveList& Position::legal_moves() const {
    if (legal_moves_.empty()) {
        const_cast<Position*>(this)->GenLegalMoveList(legal_moves_);
    }
    return legal_moves_;
}

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
        } else if (!maybe_valid) {
            maybe_valid = incomplete(boardstate, after);
        }
        after.PopMove(move);
    }

    assert(maybe_valid || candidates.empty());
    return maybe_valid;
}

// The boardstate alone can be ambiguous in the case of piece captures.  Here
// we use the actions history to disambiguate the move.  Note that we still
// generate a list, because (a) the candidates might be promotions which cannot
// be resolved here and (b) the actions history might be incomplete.
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

    // This is a check.  When we're done, the list should contain either moves
    // or promotions, but not both.
    auto num_moves = 0;
    auto num_promotions = 0;

    for (auto move : local_candidates) {
        if (move.is_promotion()) {
            // Can't resolve promotion here
            candidates.push_back(move);
            ++num_promotions;
            continue;
        }

        if (move.capture == ' ') {
            // Not a capture, so not ambiguous
            candidates.push_back(move);
            ++num_moves;
            continue;
        }

        // History should show a lift followed by a place on the capture square
        auto got_lift  = false;
        auto got_place = false;
        for (auto p = actions.rbegin(); p != actions.rend(); ++p) {
            if (!got_place) {
                got_place = p->place == move.dst;
            }
            else if (!got_lift) {
                got_lift = p->lift == move.dst;
            }
        }
        if (got_lift && got_place) {
            candidates.push_back(move);
            ++num_moves;
        }
    }

    assert(num_moves == 0 || num_promotions == 0);
    assert(num_moves == 0 || num_moves == 1);
    assert(0 <= num_promotions && num_promotions <= 4);
    assert(candidates.size() == num_moves + num_promotions);
    return maybe_valid;
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
