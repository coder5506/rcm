// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef CHESS_POSITION_H
#define CHESS_POSITION_H

#include "../thc/thc.h"

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using Bitmap = std::uint64_t;

struct Action {
    thc::Square lift{thc::SQUARE_INVALID};
    thc::Square place{thc::SQUARE_INVALID};
};
using ActionList = std::vector<Action>;

class Position;
using PositionPtr = std::shared_ptr<const Position>;

using MoveList = std::vector<thc::Move>;
using MovePair = std::pair<const thc::Move, PositionPtr>;

class Position : public thc::ChessRules {
private:
    mutable MoveList legal_moves_;
    mutable std::vector<MovePair> moves_played;

public:
    explicit Position(const char* txt = nullptr);

    PositionPtr move_played(thc::Move move) const;
    PositionPtr apply_move(thc::Move move) const;

    char at(thc::Square square) const;

    Bitmap white_bitmap() const;
    Bitmap black_bitmap() const;
    Bitmap bitmap() const;

    const MoveList& legal_moves() const;
    bool incomplete(Bitmap boardstate) const;
    bool read_moves(Bitmap boardstate, MoveList& candidates) const;
    bool read_move(
        Bitmap boardstate, const ActionList& actions, MoveList& candidates) const;
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
