// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef CHESS_POSITION_H
#define CHESS_POSITION_H

#include "chess_action.h"
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

class Position;
using PositionPtr = std::shared_ptr<const Position>;

using MoveList = std::vector<thc::Move>;

// Represents both a move and the resulting position.
using MovePair = std::pair<thc::Move, PositionPtr>;

class Position : public thc::ChessPosition {
public:
    mutable std::vector<MovePair> moves_played;

    Position(const thc::ChessPosition& position) : thc::ChessPosition(position) {}
    explicit Position(std::string_view fen = {});

    // If move has previously been played in this position, return the shared
    // resulting position.
    PositionPtr move_played(const thc::Move&) const;

    std::optional<thc::Move> find_move_played(PositionPtr) const;
    void remove_move_played(const thc::Move&) const;

    // Play move and return resulting position.  Result may be new or shared.
    //
    // `play_move` updates `moves_played` with the move and resulting position.
    // `apply_move` does not.
    PositionPtr play_move(const thc::Move&) const;
    PositionPtr apply_move(const thc::Move&) const;

    // Bitmap of pieces on board.
    Bitmap bitmap() const;

    // Bitmap of differences between two positions.
    Bitmap difference_bitmap(const Position&) const;

    // True if boardstate might represent a transition into position `after`
    bool incomplete(Bitmap boardstate, const Position& after) const;

    // Yield list of legal moves matching both boardstate and action history.
    bool read_move(
        Bitmap               boardstate,
        const ActionHistory& actions,
        MoveList&            candidates) const;

    MoveList castle_moves() const;
};

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
