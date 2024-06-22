// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef CHESS_ACTION_H
#define CHESS_ACTION_H

#include "../thc/thc.h"

#include <vector>

// An action can be either a lift or a place.
class Action {
public:
    thc::Square lift{thc::SQUARE_INVALID};
    thc::Square place{thc::SQUARE_INVALID};
};

inline Action lift(thc::Square lift) {
    return Action{lift, thc::SQUARE_INVALID};
}

inline Action place(thc::Square place) {
    return Action{thc::SQUARE_INVALID, place};
}

inline bool operator==(const Action& lhs, const Action& rhs) {
    return lhs.lift == rhs.lift && lhs.place == rhs.place;
}

// Sequence of actions from oldest to newest.
using ActionList = std::vector<Action>;


// Describe sequence of player actions required to perform a move.
class ActionPattern {
private:
    // Describe required actions
    std::vector<Action> actions;

    // Describe dependencies between required actions
    //
    // Entries are variably-sized, so treat as a list.  For each action, there
    // will be an integer denoting the number of required preceedin actions,
    // followed by an index for each dependency.  i.e.,
    //   ..., 0, ...        -- no dependencies
    //   ..., 1, x, ...     -- one dependency on action #x
    //   ..., 2, x, y, ...  -- two dependencies on actions #x and #y
    //
    // Most often, the dependencies are nothing more than "lift before place".
    std::vector<int> dependencies;

    ActionPattern(const std::vector<Action>& actions,
                  const std::vector<int>&    dependencies);

public:
    // Describe actions required to perform move or takeback
    static ActionPattern move(const thc::Move&);
    static ActionPattern takeback(const thc::Move&);

    // Match pattern against sequence of actions
    bool match_actions(
        ActionList::const_iterator& begin,  // in/out: Next unused action
        ActionList::const_iterator  end) const;

private:
    // Index of "count" cell for an action's dependencies
    int find_dependencies(int action) const;

    // True iff all dependencies for an action are satisfied
    bool can_match(int action, const std::vector<bool>& matched) const;

    // Invalidate an action and any actions that depend on it
    void invalidate(int action, std::vector<bool>& matched) const;

    // Helper
    bool match_actions(
        std::vector<bool>&          matched,
        ActionList::const_iterator& begin,
        ActionList::const_iterator  end) const;
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
