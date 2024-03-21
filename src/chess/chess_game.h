// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "../thc/thc.h"
#include "../utility/model.h"

#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <time.h>

struct Action {
    thc::Square lift{thc::SQUARE_INVALID};
    thc::Square place{thc::SQUARE_INVALID};
};

class Game : public Model {
public:
    thc::ChessRules rules;
    time_t          started{0}; // Timestamp of first move played
    std::int64_t    rowid{0};   // SQLite ROWID
    std::string     settings;   // Opaque
    std::map<std::string, std::string> tags;

    Game();
    explicit Game(const char* txt);

    std::string fen() const {
        return const_cast<thc::ChessRules&>(rules).ForsythPublish();
    }

    std::string& tag(const std::string& key);

    void apply_move(thc::Move move);
    void apply_takeback(thc::Move takeback);

    bool read_move(
        std::uint64_t              boardstate,
        std::vector<Action>::const_iterator begin,
        std::vector<Action>::const_iterator end,
        std::vector<thc::Move>&    candidates,
        std::optional<thc::Move>&  takeback);

    char at(thc::Square square) const { return rules.squares[square]; }
    std::uint64_t bitmap() const;
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
