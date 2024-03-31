// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef CHESS_POSITION_H
#define CHESS_POSITION_H

#include "../thc/thc.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

// State of board fields.  Set bit indicates presence of piece.
// MSB: H1=63 G1 F1 ... A1, H2 G2 ... A2, ..., H8 G8 ... A8=0
using Bitmap = std::uint64_t;

// An action can be either a lift or a place.
struct Action {
    thc::Square lift{thc::SQUARE_INVALID};
    thc::Square place{thc::SQUARE_INVALID};
};

// Sequence of actions from oldest to newest.
using ActionList = std::vector<Action>;

class Position;
using PositionPtr = std::shared_ptr<const Position>;

using MoveList = std::vector<thc::Move>;

// Represents both a move and the resulting position.
using MovePair = std::pair<thc::Move, PositionPtr>;

class Position : public thc::ChessRules {
    // Cache
    mutable MoveList legal_moves_;

public:
    mutable std::vector<MovePair> moves_played;

    explicit Position(std::string_view fen = {});

    // If move has previously been played in this position, return the shared
    // resulting position.
    PositionPtr move_played(thc::Move move) const;

    // Play move and return resulting position.  Result may be new or shared.
    PositionPtr play_move(thc::Move move) const;

    std::optional<thc::Move> find_move_played(PositionPtr after) const;
    void remove_move_played(thc::Move move) const;

    Bitmap bitmap() const;
    Bitmap difference_bitmap(const Position& other) const;

    const MoveList& legal_moves() const;
    MoveList castle_moves() const;

    // True if boardstate might represent a transition into position `after`
    bool incomplete(Bitmap boardstate, const Position& after) const;

    // Yield list of legal moves matching both boardstate and action history.
    bool read_move(
        Bitmap            boardstate,
        const ActionList& actions,
        MoveList&         candidates) const;

private:
    // Yield list of legal moves matching boardstate.
    bool read_moves(Bitmap boardstate, MoveList& candidates) const;
};

// True if two positions are equivalent, without considering moves played.
bool operator==(const Position& lhs, const Position& rhs);

#endif

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
