// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_engine.h"
#include "chess.h"

#include <cstdlib>
#include <string>
#include <vector>

using namespace std;
using namespace thc;

Engine::Engine(string_view name) {
    vector<string> argv = { "stockfish", nullptr };
    uci_ = UCIEngine::execvp("/usr/games/stockfish", argv);
}

void Engine::play(const Game& game, int elo) {
    auto play = make_unique<UCIPlayMessage>(&game, elo);
    request_ = play.get();
    uci_->send(std::move(play));
}

optional<Move> Engine::move() {
    if (request_) {
        if (auto response = uci_->receive(); response.get() == request_) {
            request_ = nullptr;
            if (auto p = dynamic_cast<UCIPlayMessage*>(response.get())) {
                return p->move;
            }
        }
    }
    return nullopt;
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
