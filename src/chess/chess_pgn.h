// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef RCM_CHESS_PGN_H
#define RCM_CHESS_PGN_H

#ifdef __cplusplus
extern "C" {
#endif

struct Game;

// Portable Game Notation
char *game_pgn(const struct Game *game);
void game_save_pgn(const struct Game *game, const char *filename);

struct List *game_pgn_list(const struct Game *game);

int game_read_pgn(struct Game *game, const char *pgn);
struct Game *game_from_pgn(const char *pgn);

#ifdef __cplusplus
}
#endif

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
