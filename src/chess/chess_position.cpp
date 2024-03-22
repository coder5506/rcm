// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_position.h"

#include <algorithm>
#include <cassert>

Position::Position(const char* txt) {
    if (txt) {
        Forsyth(txt);
    }
}

PositionPtr Position::move_played(thc::Move move) const {
    auto existing = std::find_if(
        moves_played.begin(),
        moves_played.end(),
        [move](const MovePair& pair) {
            return pair.first == move;
        }
    );
    return existing != moves_played.end() ? existing->second : nullptr;
}

PositionPtr Position::apply_move(thc::Move move) const {
    if (auto existing = move_played(move)) {
        return existing;
    }

    auto after = std::make_shared<Position>(*this);
    after->PlayMove(move);
    return after;
}

char Position::at(thc::Square square) const {
    return squares[square];
}

Bitmap Position::white_bitmap() const {
    Bitmap bitmap{0};
    Bitmap mask{1};
    for (thc::Square sq = thc::a8; sq <= thc::h1; ++sq) {
        switch (at(sq)) {
        case 'P': case 'N': case 'B': case 'R': case 'Q': case 'K':
            bitmap |= mask;
            break;
        default:
            break;
        }
        mask <<= 1;
    }
    return bitmap;
}

Bitmap Position::black_bitmap() const {
    Bitmap bitmap{0};
    Bitmap mask{1};
    for (thc::Square sq = thc::a8; sq <= thc::h1; ++sq) {
        switch (at(sq)) {
        case 'p': case 'n': case 'b': case 'r': case 'q': case 'k':
            bitmap |= mask;
            break;
        default:
            break;
        }
        mask <<= 1;
    }
    return bitmap;
}

Bitmap Position::bitmap() const {
    return white_bitmap() | black_bitmap();
}

const MoveList& Position::legal_moves() const {
    if (legal_moves_.empty()) {
        const_cast<Position*>(this)->GenLegalMoveList(legal_moves_);
    }
    return legal_moves_;
}

// True if boardstate might represent a transition into this position
bool Position::incomplete(Bitmap boardstate) const {
    // The boardstate for a move in-progress can differ only in small ways from
    // the boardstate of the completed move:
    // - It can be missing up-to two pieces of the same color, but only when
    //   castling.
    // - It can be missing up-to one piece of each color, but only when capturing.
    // - It can have up-to one extra piece of the opposite color, but only when
    //   capturing en-passant.

    // Present in position but not on board
    const auto removed_bmp = bitmap() & ~boardstate;
    const auto removed = __builtin_popcountll(removed_bmp);

    // Present on board but not in position
    const auto added   = __builtin_popcountll(boardstate & ~bitmap());

    // First check: just count the differences
    if (removed > 2 || added > 1 || removed + added > 2) {
        return false;
    }

    // Second check: look at the colors involved
    const auto white_removed = __builtin_popcountll(white_bitmap() & ~boardstate);
    const auto black_removed = __builtin_popcountll(black_bitmap() & ~boardstate);
    assert(removed == white_removed + black_removed);

    if ( WhiteToPlay() && black_removed > 1) {
        return false;
    }
    if (!WhiteToPlay() && white_removed > 1) {
        return false;
    }

    if (white_removed == 2) {
        if (wking_allowed()  && removed_bmp == (1ull << thc::e1 | 1ull << thc::h1)) {
            return true;
        }
        if (wqueen_allowed() && removed_bmp == (1ull << thc::e1 | 1ull << thc::a1)) {
            return true;
        }
        return false;
    }
    if (black_removed == 2) {
        if (bking_allowed()  && removed_bmp == (1ull << thc::e8 | 1ull << thc::h8)) {
            return true;
        }
        if (bqueen_allowed() && removed_bmp == (1ull << thc::e8 | 1ull << thc::a8)) {
            return true;
        }
        return false;
    }

    // Third check: consider the possibility of en passant
    if (added == 0) {
        // i.e., we're unable to reject the possibility
        return true;
    }

    assert(added == 1);
    if (groomed_enpassant_target() == thc::SQUARE_INVALID) {
        return false;
    }

    // // In mailbox coordinates...
    // const int direction = position->turn == 'w' ? -10 : 10;
    // const int captured  = mailbox_index[position->en_passant] - direction;

    // uint64_t mask = 1 << position->en_passant | 1 << board_index[captured];
    // if (board_index[captured-1] != thc::SQUARE_INVALID) {
    //     mask |= 1 << board_index[captured-1];
    // }
    // if (board_index[captured+1] != thc::SQUARE_INVALID) {
    //     mask |= 1 << board_index[captured+1];
    // }

    // const uint64_t diff = position->bitmap ^ boardstate;
    // return (diff & ~mask) == 0;

    return true;
}

// Construct a list of candidate moves in this position that match the given
// boardstate.  The return indicates if there are any viable candidates:
// - true if any candidates are found OR if the boardstate could be in
//   transition to a valid move,
// - false if the boardstate is incompatible with all legal moves in this
//   position.
bool Position::read_moves(Bitmap boardstate, MoveList& candidates) const {
    auto maybe_valid = false;
    Position before{*this};

    candidates.clear();
    for (const auto move : legal_moves()) {
        before.PushMove(move);
        if (before.bitmap() == boardstate) {
            candidates.push_back(move);
            maybe_valid = true;
        } else {
            maybe_valid = maybe_valid || before.incomplete(boardstate);
        }
        before.PopMove(move);
    }

    assert(maybe_valid || candidates.empty());
    return maybe_valid;
}

// The boardstate alone can be ambiguous in the case of piece captures.  Here
// we use the actions history to disambiguate the move.  Note that we still
// generate a list, because (a) the candidates might be promotions which cannot
// be resolved here and (b) the actions history might be incomplete.
bool Position::read_move(
    Bitmap boardstate, const ActionList& actions, MoveList& candidates) const
{
    candidates.clear();

    MoveList local_candidates;
    const bool maybe_valid = read_moves(boardstate, local_candidates);
    if (local_candidates.empty()) {
        return maybe_valid;
    }

    assert(maybe_valid);

    // This is a check.  When we're done, the list should contain either moves
    // or promotions, but not both.
    auto num_moves = 0;
    auto num_promotions = 0;

    for (const auto move : local_candidates) {
        if (move.is_promotion()) {
            // Can't resolve promotion here
            candidates.push_back(move);
            ++num_promotions;
            continue;
        }

        if (move.capture == ' ') {
            // No capture, not ambiguous
            candidates.push_back(move);
            ++num_moves;
            continue;
        }

        // History should show a lift followed by a place on the capture square
        auto got_lift  = false;
        auto got_place = false;
        for (auto p = actions.rbegin(); p != actions.rend(); ++p) {
            if (!got_place) {
                got_place = p->place == move.capture;
            }
            else if (!got_lift) {
                got_lift = p->lift == move.capture;
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
