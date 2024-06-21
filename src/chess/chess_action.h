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

inline bool operator==(const Action& lhs, const Action& rhs) {
    return lhs.lift == rhs.lift && lhs.place == rhs.place;
}

// Sequence of actions from oldest to newest.
using ActionList = std::vector<Action>;

// Describe loosely-ordered sequence of actions
class ActionPattern {
private:
    // Describe required actions
    std::vector<Action> actions;

    // Describe dependencies between required actions
    //
    // Treat as a list, entries are variably-sized.  For each action, there will
    // be an integer denoting the number of dependencies (required preceeding
    // actions).  Then follows an action index for each dependency.
    // i.e.,
    //   ..., 0, ...        -- no dependencies
    //   ..., 1, x, ...     -- one dependency on action #x
    //   ..., 2, x, y, ...  -- two dependencies on actions #x and #y
    std::vector<int> depends;

public:
    // Construct pattern of actions required to perform move
    ActionPattern(const thc::Move&);

    bool match_actions(
        ActionList::const_iterator& begin,
        ActionList::const_iterator  end) const;

private:
    // Return index of "count" cell for requirements of action
    int find_dependencies(int action) const;

    bool can_match(int action, const std::vector<bool>& matched) const;

    void invalidate(int action, std::vector<bool>& matched) const;

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
