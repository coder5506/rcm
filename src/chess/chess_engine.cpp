// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_engine.h"
#include "chess.h"

#include <cstdlib>

using namespace std;
using namespace thc;

Engine::Engine(string_view name) {
    char *argv[] = {(char*)"stockfish", NULL};
    uci = UCIEngine::execvp("/usr/games/stockfish", argv);
}

void Engine::play(const Game& game, int elo) {
    uci->send(make_unique<UCIPlayMessage>(&game, elo));
}

optional<Move> Engine::move() {
    auto response = uci->receive();
    if (auto play = dynamic_cast<UCIPlayMessage*>(response.get())) {
        return play->move;
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
