// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

#include "../thc/thc.h"

#include <memory>
#include <optional>
#include <string_view>

class Game;
class UCIEngine;

// Higher-level interface to UCI engines
class Engine {
    std::unique_ptr<UCIEngine> uci;

public:
    Engine(std::string_view name);

    // Prevent accidental copying
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    // Request engine to select a move
    void play(const Game& game, int elo);

    // Ask if engine has a move ready
    std::optional<thc::Move> move();
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
