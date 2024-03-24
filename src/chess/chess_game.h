// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "chess_position.h"
#include "../utility/model.h"

#include <cstdint>
#include <ctime>
#include <map>
#include <optional>
#include <string>
#include <vector>

class Game : public Model<Game>, public Observer<Game> {
public:
    std::vector<PositionPtr> history;
    std::time_t  started{0};
    std::int64_t rowid{0};  // SQLite ROWID
    std::string  settings;  // Opaque
    std::map<std::string, std::string> tags;

    explicit Game(const char* txt = nullptr);

    void model_changed(Game&) override;

    std::string& tag(const std::string& key);

    PositionPtr current() const;
    PositionPtr previous() const;

    const MoveList& legal_moves() const;

    inline bool WhiteToPlay() const { return current()->WhiteToPlay(); }

    std::string fen() const;

    char at(thc::Square square) const;

    void apply_move(thc::Move move);
    void apply_takeback(thc::Move takeback);

    void play_move(thc::Move move);
    void play_uci_move(std::string_view uci_move);

    bool read_move(
        Bitmap            boardstate,
        const ActionList& actions,
        MoveList&         candidates,
        std::optional<thc::Move>& takeback);

    Bitmap bitmap() const { return current()->bitmap(); }
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
