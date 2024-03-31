// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef STANDARD_H
#define STANDARD_H

#include "chess/chess.h"
#include "utility/model.h"

// Either player can be computer or human, no restrictions
enum PlayerType {
    COMPUTER,
    HUMAN,
};

struct Player {
    PlayerType type;
    union {
        // Engine settings for computer players
        struct {
            const char* engine;  // e.g. "stockfish"
            int         elo;     // Desired playing strength 1400..2800
            // N.B., 2800 is about as good as Stockfish can do on a Pi Zero
        } computer;
        // Coaching settings for human players
        struct {
            // centipawns, zero to disable
            int error;        // Alert moves that actively worsen your position
            int opportunity;  // Alert moves that fail to improve your position
        } human;
    };
};

// Standard gameplay module:
// - Standard chess
// - Casual play
// - Human vs. Computer or Human vs. Human
class StandardGame : public Observer<Game> {
    Player white;
    Player black;

public:
    virtual ~StandardGame();
    StandardGame();

    void main();

    void on_changed(Game&) override;
    void set_game(std::unique_ptr<Game>);

private:
    // To/from JSON for persistence
    char* settings_to_json();
    int   settings_from_json(const char*);

    // Implementation of main()
    void start();
    void run();
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
