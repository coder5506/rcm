// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef CHESS_FEN_H
#define CHESS_FEN_H

struct Position;

// rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
extern const char *STARTING_FEN;
extern const int FEN_MAX;

// Return num chars written not including terminator
int position_fen(const struct Position *position, char *fen, int len);

// Return num chars consumed
int position_read_fen(struct Position *position, const char *fen);

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
