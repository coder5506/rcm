// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "chess_action.h"

#include <algorithm>

using namespace std;
using namespace thc;

// Construct pattern of actions required to perform `move`.
ActionPattern::ActionPattern(const Move& move) {
    switch (move.special) {
    default:
        // Ordinary moves
        if (move.capture == ' ') {
            // Not capturing
            actions = {
                Action{move.src, SQUARE_INVALID},  // Lift
                Action{SQUARE_INVALID, move.dst},  // Place
            };
            depends = {
                0,     // Lift has no dependencies
                1, 0,  // Place has one dependency, on actions[0]
            };
        }
        else {
            // Copturing
            actions = {
                Action{move.dst, SQUARE_INVALID},  // Lift copture
                Action{move.src, SQUARE_INVALID},  // Lift piece
                Action{SQUARE_INVALID, move.dst},  // Place piece
            };
            depends = {
                0,
                0,
                2, 0, 1,  // Place has two dependencies, on actions[0] and actions[1]
            };
        }
        break;

    case SPECIAL_WEN_PASSANT:
        // Capture white pawn en passont.  Removes pawn south of destination.
        actions = {
            Action{SOUTH(move.dst), SQUARE_INVALID},  // Lift capture
            Action{move.src, SQUARE_INVALID},         // Lift piece
            Action{SQUARE_INVALID, move.dst},         // Place piece
        };
        depends = {
            0,
            0,
            1, 1,  // Place must occur after lifting moving pawn
        };
        break;

    case SPECIAL_BEN_PASSANT:
        // Capture black pawn en passant.  Removes pawn north of destination.
        actions = {
            Action{NORTH(move.dst), SQUARE_INVALID},  // Lift capture
            Action{move.src, SQUARE_INVALID},         // Lift piece
            Action{SQUARE_INVALID, move.dst},         // Place piece
        };
        depends = {
            0,
            0,
            1, 1,  // Place must occur after lifting moving pawn
        };
        break;

    case SPECIAL_WK_CASTLING:
        actions = {
            Action{e1, SQUARE_INVALID},  // Lift king
            Action{h1, SQUARE_INVALID},  // Lift rook
            Action{SQUARE_INVALID, f1},  // Place rook
            Action{SQUARE_INVALID, g1},  // Place king
        };
        depends = {
            0,
            0,
            1, 1,  // Lift rook before placing
            1, 0,  // Lift king before placing
        };
        break;

    case SPECIAL_BK_CASTLING:
        actions = {
            Action{e8, SQUARE_INVALID},  // Lift king
            Action{h8, SQUARE_INVALID},  // Lift rook
            Action{SQUARE_INVALID, f8},  // Place rook
            Action{SQUARE_INVALID, g8},  // Place king
        };
        depends = {
            0,
            0,
            1, 1,  // Lift rook before placing
            1, 0,  // Lift king before placing
        };
        break;

    case SPECIAL_WQ_CASTLING:
        actions = {
            Action{e1, SQUARE_INVALID},  // Lift king
            Action{a1, SQUARE_INVALID},  // Lift rook
            Action{SQUARE_INVALID, d1},  // Place rook
            Action{SQUARE_INVALID, c1},  // Place king
        };
        depends = {
            0,
            0,
            1, 1,  // Lift rook before placing
            1, 0,  // Lift king before placing
        };
        break;

    case SPECIAL_BQ_CASTLING:
        actions = {
            Action{e8, SQUARE_INVALID},  // Lift king
            Action{a8, SQUARE_INVALID},  // Lift rook
            Action{SQUARE_INVALID, d8},  // Place rook
            Action{SQUARE_INVALID, c8},  // Place king
        };
        depends = {
            0,
            0,
            1, 1,  // Lift rook before placing
            1, 0,  // Lift king before placing
        };
        break;
    }
}

int ActionPattern::find_dependencies(int action) const {
    auto index = 0;
    for (auto i = 0; i != action; ++i) {
        auto count = depends[index];
        index += 1;      // Skip over count
        index += count;  // Skip over count items
    }
    return index;
}

bool ActionPattern::can_match(int action, const vector<bool>& matched) const {
    auto index = find_dependencies(action);
    auto count = depends[index];
    ++index;

    for (auto i = 0; i != count; ++i) {
        if (!matched[depends[index + i]]) {
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
    // dependence, and earlier (in the list) actions never depend on later
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
        // Nothing more to match
        return false;
    }

    // Find next action to match
    auto action = -1;
    for (; begin != end; ++begin) {
        for (auto i = 0; i != actions.size(); ++i) {
            if (actions[i] == *begin) {
                action = i;
                break;
            }
        }
        if (action >= 0) {
            // Found something of interest, consume it
            ++begin;
            break;
        }
    }
    if (action == -1) {
        // Can't find any matching actions
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
