// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef CENTAUR_H
#define CENTAUR_H

#include "board.h"
#include "screen.h"

#include <optional>
#include <vector>

class Centaur : public Observer<Game> {
public:
    // Hardware RAII
    Board  board;
    Screen screen;

    // Current game
    std::unique_ptr<Game> game;

    // Render display
    std::unique_ptr<View> screen_view;

    // Remember recent user actions
    ActionHistory actions;

    Centaur();

    // Play the black pieces without rotating the board
    bool reversed() const;
    void reversed(bool);

    // Respond to changes in game state
    void on_changed(Game&) override;

    // Update display
    void render();

    // Replace current game
    void set_game(std::unique_ptr<Game>);

    int batterylevel();
    int charging();

    // Read current state of board fields
    // MSB: H1=63 G1 F1 ... A1, H2 G2 ... A2, ..., H8 G8 ... A8=0
    Bitmap getstate();

    // Read new actions, adding to cached history
    int update_actions();

    // Forget all cached actions
    void purge_actions();

    bool read_move(
        Bitmap    boardstate,
        MoveList& candidates,
        std::optional<thc::Move>& takeback);

    void clear_feedback();
    void led(thc::Square);
    void led_from_to(thc::Square, thc::Square);
    void show_feedback(Bitmap);

private:
    bool reconstruct_move(
        Game&                     game,
        Bitmap                    boardstate,
        ActionHistory&            actions,
        MoveList&                 candidates,
        std::optional<thc::Move>& takeback);
    bool reconstruct_moves(Bitmap boardstate, ActionHistory& actions);
};

extern Centaur centaur;

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
