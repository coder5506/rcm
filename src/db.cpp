// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "db.h"
#include "cfg.h"
#include "chess/chess.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sqlite3.h>

static const char *SCHEMA =
    "CREATE TABLE IF NOT EXISTS games ("
    "  event    TEXT,"  // Seven Tag Roster (STR)
    "  site     TEXT,"  // For convenience.  Both the STR and all other tags are
    "  date     TEXT,"  // included in the PGN.
    "  round    TEXT,"
    "  white    TEXT,"
    "  black    TEXT,"
    "  result   TEXT,"
    "  pgn      TEXT,"  // Full game, including variations
    "  fen      TEXT,"  // Identifies current position
    "  settings TEXT"   // JSON string describing game settings
    ");";

static sqlite3 *db = NULL;

void db_close(void) {
    sqlite3_close(db);
    db = NULL;
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

static int insert_game(Game* game) {
    assert(db);

    const char *sql =
        "INSERT INTO games"
        "  (event, site, date, round, white, black, result, pgn, fen, settings)"
        " VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return 1;
    }

    sqlite3_bind_text(stmt,  1, game->tag("Event").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  2, game->tag("Site").data(),   -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  3, game->tag("Date").data(),   -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  4, game->tag("Round").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  5, game->tag("White").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  6, game->tag("Black").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  7, game->tag("Result").data(), -1, SQLITE_STATIC);
    // sqlite3_bind_text(stmt,  8, game_pgn(game),           -1, SQLITE_STATIC);
    // sqlite3_bind_text(stmt,  9, game_fen(game),           -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 10, game->settings.data(),      -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    game->rowid = sqlite3_last_insert_rowid(db);
    return rc != SQLITE_DONE;
}

static int update_game(Game* game) {
    assert(db);

    const char *sql =
        "UPDATE games SET"
        "  event = ?, site  = ?, date     = ?, round = ?,"
        "  white = ?, black = ?, result   = ?,"
        "  pgn   = ?, fen   = ?, settings = ?"
        " WHERE rowid = ?";
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return 1;
    }

    sqlite3_bind_text(stmt,  1, game->tag("Event").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  2, game->tag("Site").data(),   -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  3, game->tag("Date").data(),   -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  4, game->tag("Round").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  5, game->tag("White").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  6, game->tag("Black").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  7, game->tag("Result").data(), -1, SQLITE_STATIC);
    // sqlite3_bind_text(stmt,  8, game_pgn(game),           -1, SQLITE_STATIC);
    // sqlite3_bind_text(stmt,  9, game_fen(game),           -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 10, game->settings.data(),      -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 11, game->rowid);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc != SQLITE_DONE;
}

int db_save_game(Game* game) {
    return game->rowid ? update_game(game) : insert_game(game);
}

Game* db_load_game(int64_t rowid) {
    assert(rowid > 0);
    assert(db);

    const char *sql = "SELECT pgn, fen, settings FROM games WHERE rowid = ?";
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return NULL;
    }

    sqlite3_bind_int64(stmt, 1, rowid);
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    const char *pgn = (const char*)sqlite3_column_text(stmt, 0);
    const char *fen = (const char*)sqlite3_column_text(stmt, 1);

    return NULL;

    // Game* game = game_from_pgn_and_fen(pgn, fen);
    // if (!game) {
    //     sqlite3_finalize(stmt);
    //     return NULL;
    // }

    // game->id = rowid;
    // game->settings = NULL;
    // if (sqlite3_column_bytes(stmt, 2) > 0) {
    //     game->settings = (const char*)malloc(sqlite3_column_bytes(stmt, 2) + 1);
    //     strcpy((char*)game->settings, (const char*)sqlite3_column_text(stmt, 2));
    // }

    // sqlite3_finalize(stmt);
    // return game;
}

Game* db_load_latest(void) {
    assert(db);

    const char *sql = "SELECT MAX(rowid) FROM games";
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return NULL;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    int64_t rowid = sqlite3_column_int64(stmt, 0);
    sqlite3_finalize(stmt);
    return rowid > 0 ? db_load_game(rowid) : NULL;
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
