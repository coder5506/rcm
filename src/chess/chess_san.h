// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef CHESS_SAN_H
#define CHESS_SAN_H

#include <stdio.h>

struct Move;
struct Position;

// Standard Algebraic Notation
int
san_write_move(FILE *out, const struct Position *before, const struct Move *move);

int move_san(char *buf, int len, const struct Move *move);
const char *move_san_static(const struct Move *move);
struct Move *move_from_san(const struct Position *before, const char *san);

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
