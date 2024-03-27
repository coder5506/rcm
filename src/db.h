// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef DB_H
#define DB_H

#include <memory>
#include <sqlite3.h>

struct Game;

class Database {
    sqlite3 *db;

public:
    ~Database();
    Database();

    int save_game(Game&);
    std::unique_ptr<Game> load_game(sqlite3_int64 rowid);
    std::unique_ptr<Game> load_latest();

private:
    int insert_game(Game&);
    int update_game(Game&);
};

extern Database db;

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
