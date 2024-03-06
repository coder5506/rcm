// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef CHESS_GENERATE_H
#define CHESS_GENERATE_H

#include <stdbool.h>

struct List;
struct Position;

void generate_castle_moves(struct List *list, const struct Position *position);

// List of all legal moves and their resulting positions
void generate_legal_moves(struct List *list, const struct Position *position);

bool position_legal(const struct Position *position);
struct List *position_legal_moves(const struct Position *before);

bool position_is_check(const struct Position *position);
bool position_is_checkmate(const struct Position *position);

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
