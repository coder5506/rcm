// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_engine.h"
#include "chess.h"

#include <cstdlib>

using namespace std;
using namespace thc;

static unique_ptr<UCIEngine> engine;

static UCIEngine* start_engine() {
    if (!engine) {
        char *argv[] = {(char*)"stockfish", NULL};
        engine = UCIEngine::execvp("/usr/games/stockfish", argv);
    }
    return engine.get();
}

static UCIEngine* get_engine(const char* name) {
    (void)name;
    return start_engine();
}

optional<Move> engine_move(const Game& game, const char* engine_name) {
    (void)game;

    auto engine = get_engine(engine_name);
    if (!engine) {
        return {};
    }

    auto response = engine->receive();
    if (auto play = dynamic_cast<UCIPlayMessage*>(response.get())) {
        return play->move;
    }
    return {};
}

void engine_play(const Game& game, const char* engine_name, int elo) {
    auto engine = get_engine(engine_name);
    if (!engine) {
        return;
    }

    engine->send(make_unique<UCIPlayMessage>(&game, elo));
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
