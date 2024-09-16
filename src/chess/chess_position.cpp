// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_position.h"
#include "../thc/MoveGen.h"

#include <algorithm>
#include <cassert>
#include <cstring>

using namespace std;
using namespace thc;

Position::Position(string_view fen) {
    if (!fen.empty()) {
        Forsyth(fen.data());
    }
}

PositionPtr Position::move_played(const Move& move) const {
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

PositionPtr Position::apply_move(const Move& move) const {
    return make_shared<Position>(ChessPosition::play_move(move));
}

PositionPtr Position::play_move(const Move& move) const {
    if (auto existing = move_played(move)) {
        return existing;
    }

    auto after = make_shared<Position>(ChessPosition::play_move(move));
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

void Position::remove_move_played(const Move& move) const {
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

// Bitmap of differences between two positions (*not* difference of their
// bitmaps).  Accounts for when a square is occupied by a different piece.
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

// Note: does not check legality of castling moves, only availability
MoveList Position::castle_moves() const {
    MoveList king_moves;
    if (WhiteToPlay() && d.wking_square == e1) {
        thc::KingMoves(*this, e1, king_moves);
    }
    else if (BlackToPlay() && d.bking_square == e8) {
        thc::KingMoves(*this, e8, king_moves);
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
    // What squares differ between this position and the next?
    const auto position_diff = difference_bitmap(after);

    // What squares differ between this position and the board?
    const auto board_diff = boardstate ^ bitmap();

    // There should be no board differences not explained by the position
    // differences.
    return (board_diff & ~position_diff) == 0;
}

// Construct list of candidate moves in this position that match the given
// boardstate.  The return indicates if there are any viable candidates:
// - true if any candidates are found OR if the boardstate could be in
//   transition to a valid move,
// - false if the boardstate is incompatible with all legal moves in this
//   position.
//
// We use the actions history to disambiguate captures, as necessary.
bool Position::read_move(
    Bitmap               boardstate,
    const ActionHistory& actions,
    MoveList&            candidates) const
{
    candidates.clear();

    // True if boardstate is compatible with some legal move in this position.
    auto maybe_valid = false;

    for (auto move : legal_moves()) {
        const auto after = apply_move(move);
        if (after->bitmap() != boardstate) {
            maybe_valid = maybe_valid || incomplete(boardstate, *after);
            continue;
        }

        // Legal move, so valid.
        maybe_valid = true;

        // Should only generate multiple candidates for promotions.
        assert(move.is_promotion() || candidates.empty());

        // Use action history to disambiguate captures.
        if (move.is_capture()) {
            if (actions.match_move(move)) {
                candidates.push_back(move);
                if (!move.is_promotion()) {
                    // This is the one, we don't need to check anything else.
                    return true;
                }
            }
        }
        else {
            // Not a capture.
            candidates.push_back(move);
            if (!move.is_promotion()) {
                // Not ambiguous, can short-circuit.
                return true;
            }
        }
    }

    // If we found any candidates, the boardstate must be valid.
    assert(candidates.empty() || maybe_valid);

    // Because of short-circuiting above, any candidates will here will be promotions.
    assert(candidates.empty() || candidates.size() == 4);

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
