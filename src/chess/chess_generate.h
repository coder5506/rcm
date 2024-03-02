// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef CHESS_GENERATE_H
#define CHESS_GENERATE_H

#include <stdbool.h>

struct Move;
struct Position;

// Return result of making move in position
struct Position*
position_move(const struct Position *position, const struct Move *move);

void position_castle_moves(struct Move *list, const struct Position *position);
bool position_legal(const struct Position *position);

// Return list of all legal moves and their resulting positions
void position_legal_moves(struct Move *list, const struct Position *position);

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
