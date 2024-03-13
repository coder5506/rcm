// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "cfg.h"

#include <stddef.h>
#include <stdio.h>

#include <sqlite3.h>

static const char *SCHEMA =
    "CREATE TABLE IF NOT EXISTS games ("
    "  event TEXT, site  TEXT, date   TEXT, round TEXT,"
    "  white TEXT, black TEXT, result TEXT, pgn   TEXT"
    ");";

static sqlite3 *db;

void db_close() {
    sqlite3_close(db);
}

int db_open(void) {
    char *path = NULL;
    asprintf(&path, "%s/rcm.db", cfg_data_dir());
    int rc = sqlite3_open(path, &db);
    if (rc != SQLITE_OK) {
        db_close();
        return 1;
    }
    sqlite3_exec(db, SCHEMA, NULL, NULL, NULL);
    return 0;
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
