// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_action.h"

#include <algorithm>

using namespace std;
using namespace thc;


//
// ActionPattern
//

ActionPattern::ActionPattern(const vector<Action>& actions,
                             const vector<int>&    dependencies)
    : actions(actions), dependencies(dependencies) {}

// Construct pattern of actions required to perform mave.
ActionPattern ActionPattern::move(const Move& mv) {
    vector<Action> actions;
    vector<int>    dependencies;

    switch (mv.special) {
    default:
        // Ordinary moves
        if (mv.capture == ' ') {
            // Not capturing
            actions = {
                lift(mv.src),   // Lift piece
                place(mv.dst),  // Place piece
            };
            dependencies = {
                0,     // Lift has no dependencies
                1, 0,  // Place has one dependency, on actions[0]
            };
        }
        else {
            // Copturing
            actions = {
                lift(mv.dst),   // Lift capture
                lift(mv.src),   // Lift piece
                place(mv.dst),  // Place piece
            };
            dependencies = {
                0,
                0,
                2, 0, 1,  // Place has two dependencies, on actions[0] and actions[1]
            };
        }
        break;

    case SPECIAL_WEN_PASSANT:
        // White pawn captures en passant.  Removes black pawn south of destination.
        actions = {
            lift(SOUTH(mv.dst)),  // Lift capture
            lift(mv.src),         // Lift pawn
            place(mv.dst),        // Place pawn
        };
        dependencies = {
            0,
            0,
            1, 1,  // Place must occur after lifting moving pawn
        };
        break;

    case SPECIAL_BEN_PASSANT:
        // Black pawn captures en passant.  Removes white pawn north of destination.
        actions = {
            lift(NORTH(mv.dst)),  // Lift capture
            lift(mv.src),         // Lift pawn
            place(mv.dst),        // Place pawn
        };
        dependencies = {
            0,
            0,
            1, 1,  // Place must occur after lifting moving pawn
        };
        break;

    case SPECIAL_WK_CASTLING:
        actions = {
            lift(e1),   // Lift king
            lift(h1),   // Lift rook
            place(f1),  // Place rook
            place(g1),  // Place king
        };
        dependencies = {
            0,
            0,
            1, 1,  // Lift rook before placing
            1, 0,  // Lift king before placing
        };
        break;

    case SPECIAL_BK_CASTLING:
        actions = {
            lift(e8),   // Lift king
            lift(h8),   // Lift rook
            place(f8),  // Place rook
            place(g8),  // Place king
        };
        dependencies = {
            0,
            0,
            1, 1,  // Lift rook before placing
            1, 0,  // Lift king before placing
        };
        break;

    case SPECIAL_WQ_CASTLING:
        actions = {
            lift(e1),   // Lift king
            lift(a1),   // Lift rook
            place(d1),  // Place rook
            place(c1),  // Place king
        };
        dependencies = {
            0,
            0,
            1, 1,  // Lift rook before placing
            1, 0,  // Lift king before placing
        };
        break;

    case SPECIAL_BQ_CASTLING:
        actions = {
            lift(e8),   // Lift king
            lift(a8),   // Lift rook
            place(d8),  // Place rook
            place(c8),  // Place king
        };
        dependencies = {
            0,
            0,
            1, 1,  // Lift rook before placing
            1, 0,  // Lift king before placing
        };
        break;
    }

    return ActionPattern(actions, dependencies);
}

// Construct pattern of actions required to perform takeback.
ActionPattern ActionPattern::takeback(const Move& mv) {
    vector<Action> actions;
    vector<int>    dependencies;

    switch (mv.special) {
    default:
        // Ordinary moves
        if (mv.capture == ' ') {
            // Not capturing
            actions = {
                lift(mv.dst),   // Lift piece
                place(mv.src),  // Place piece
            };
            dependencies = {
                0,
                1, 0,  // Lift piece before placing
            };
        }
        else {
            // Copturing
            actions = {
                lift(mv.dst),   // Lift piece
                place(mv.src),  // Place piece
                place(mv.dst),  // Place capture
            };
            dependencies = {
                0,
                1, 0,  // Lift piece before placing
                1, 0,  // Lift piece before restoring capture
            };
        }
        break;

    case SPECIAL_WEN_PASSANT:
        // White pawn captures en passant.  Removes black pawn south of destination.
        actions = {
            lift(mv.dst),          // Lift pawn
            place(mv.src),         // Place pawn
            place(SOUTH(mv.dst)),  // Place capture
        };
        dependencies = {
            0,
            1, 1,  // Place must occur after lifting moving pawn
            0,
        };
        break;

    case SPECIAL_BEN_PASSANT:
        // Black pawn captures en passant.  Removes white pawn north of destination.
        actions = {
            lift(mv.dst),          // Lift pawn
            place(mv.src),         // Place pawn
            place(NORTH(mv.dst)),  // Place capture
        };
        dependencies = {
            0,
            1, 1,  // Place must occur after lifting moving pawn
            0,
        };
        break;

    case SPECIAL_WK_CASTLING:
        actions = {
            lift(g1),   // Lift king
            lift(f1),   // Lift rook
            place(h1),  // Place rook
            place(e1),  // Place king
        };
        dependencies = {
            0,
            0,
            1, 1,  // Lift rook before placing
            1, 0,  // Lift king before placing
        };
        break;

    case SPECIAL_BK_CASTLING:
        actions = {
            lift(g8),   // Lift king
            lift(f8),   // Lift rook
            place(h8),  // Place rook
            place(e8),  // Place king
        };
        dependencies = {
            0,
            0,
            1, 1,  // Lift rook before placing
            1, 0,  // Lift king before placing
        };
        break;

    case SPECIAL_WQ_CASTLING:
        actions = {
            lift(c1),   // Lift king
            lift(d1),   // Lift rook
            place(a1),  // Place rook
            place(e1),  // Place king
        };
        dependencies = {
            0,
            0,
            1, 1,  // Lift rook before placing
            1, 0,  // Lift king before placing
        };
        break;

    case SPECIAL_BQ_CASTLING:
        actions = {
            lift(c8),   // Lift king
            lift(d8),   // Lift rook
            place(a8),  // Place rook
            place(e8),  // Place king
        };
        dependencies = {
            0,
            0,
            1, 1,  // Lift rook before placing
            1, 0,  // Lift king before placing
        };
        break;
    }

    return ActionPattern(actions, dependencies);
}

int ActionPattern::find_dependencies(int action) const {
    auto index = 0;
    for (auto i = 0; i != action; ++i) {
        auto count = dependencies[index];
        index += 1;      // Skip over count
        index += count;  // Skip over count items
    }
    return index;
}

bool ActionPattern::can_match(int action, const vector<bool>& matched) const {
    auto index = find_dependencies(action);
    auto count = dependencies[index];
    ++index;

    for (auto i = 0; i != count; ++i) {
        auto dependency = dependencies[index + i];
        if (!matched[dependency]) {
            // Missing required dependency
            return false;
        }
    }
    // All dependencies satisfied
    return true;
}

void ActionPattern::invalidate(int action, vector<bool>& matched) const {
    // Forget we saw this action
    matched[action] = false;

    // And forget anything that may have depended on it
    //
    // N.B., Our move dependencies are simple: never more than one level of
    // dependency, and earlier (in the list) actions never depend on later
    // actions.  This means a single pass through the loop is quite sufficient,
    // we don't need to do anything fancy.
    for (auto i = 0; i != actions.size(); ++i) {
        if (!can_match(i, matched)) {
            matched[i] = false;
        }
    }

}

bool ActionPattern::match_actions(
    vector<bool>&               matched,
    ActionList::const_iterator& begin,
    ActionList::const_iterator  end) const
{
    if (find(matched.begin(), matched.end(), false) == matched.end()) {
        // All ncessary actions have been matched
        return true;
    }

    if (begin == end) {
        // No possibility of further matches
        return false;
    }

    // Find next action to match
    auto action = -1;
    for (; action == -1 && begin != end; ++begin) {
        for (auto i = 0; action == -1 && i != actions.size(); ++i) {
            if (actions[i] == *begin) {
                action = i;
            }
        }
    }
    if (action == -1) {
        // Failed to find any matching action
        return false;
    }

    if (matched[action]) {
        // We've seen this before.  Later moves take precedence over earlier,
        // so that last time didn't count.  We have to forget it and anything
        // that depended on it.
        invalidate(action, matched);
    }

    if (!can_match(action, matched)) {
        // We're not yet ready for this
        return false;
    }

    // Try it
    matched[action] = true;
    return match_actions(matched, begin, end);
}

bool ActionPattern::match_actions(
    ActionList::const_iterator& begin,
    ActionList::const_iterator  end) const
{
    for (auto p = begin; p != end; ++p) {
        vector<bool> matched(actions.size(), false);
        auto ok = match_actions(matched, p, end);
        if (ok) {
            begin = p;
            return true;
        }
    }
    return false;
}


//
// ActionHistory
//

void ActionHistory::pop_front() {
    erase(begin(), begin() + 1);
}

bool ActionHistory::match_move(const thc::Move& move) const {
    const auto pattern = ActionPattern::move(move);
    auto begin = this->begin();
    return pattern.match_actions(begin, end());
}

bool ActionHistory::match_takeback(const thc::Move& takeback) const {
    const auto pattern = ActionPattern::takeback(takeback);
    auto begin = this->begin();
    return pattern.match_actions(begin, end());
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
