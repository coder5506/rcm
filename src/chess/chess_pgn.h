// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef CHESS_PGN_H
#define CHESS_PGN_H

#include <ostream>

class Game;

namespace pgn {
    void write_pgn(std::ostream&, const Game&);

    void read_tags(char*& pgn, Game& game);
    bool read_movetext(char*& pgn, Game& game);
}

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
