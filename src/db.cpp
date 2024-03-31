// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "db.h"
#include "cfg.h"
#include "chess/chess.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

using namespace std;

Database db;

static auto SCHEMA =
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

Database::~Database() {
    sqlite3_close(db);
}

Database::Database() {
    char* path = nullptr;
    asprintf(&path, "%s/rcm.db", cfg_data_dir());
    if (sqlite3_open(path, &db) != SQLITE_OK) {
        throw runtime_error("Failed to open database");
    }
    sqlite3_exec(db, SCHEMA, nullptr, nullptr, nullptr);
}

int Database::insert_game(Game& game) {
    auto sql =
        "INSERT INTO games"
        "  (event, site, date, round, white, black, result, pgn, fen, settings)"
        " VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt = nullptr;
    auto rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return 1;
    }

    sqlite3_bind_text(stmt,  1, game.tag("Event").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  2, game.tag("Site").data(),   -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  3, game.tag("Date").data(),   -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  4, game.tag("Round").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  5, game.tag("White").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  6, game.tag("Black").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  7, game.tag("Result").data(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  8, game.pgn().data(),         -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt,  9, game.fen().data(),         -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 10, game.settings.data(),      -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    game.rowid = sqlite3_last_insert_rowid(db);
    return rc != SQLITE_DONE;
}

int Database::update_game(Game& game) {
    auto sql =
        "UPDATE games SET"
        "  event = ?, site  = ?, date     = ?, round = ?,"
        "  white = ?, black = ?, result   = ?,"
        "  pgn   = ?, fen   = ?, settings = ?"
        " WHERE rowid = ?";
    sqlite3_stmt* stmt = nullptr;
    auto rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return 1;
    }

    sqlite3_bind_text( stmt,  1, game.tag("Event").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text( stmt,  2, game.tag("Site").data(),   -1, SQLITE_STATIC);
    sqlite3_bind_text( stmt,  3, game.tag("Date").data(),   -1, SQLITE_STATIC);
    sqlite3_bind_text( stmt,  4, game.tag("Round").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text( stmt,  5, game.tag("White").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text( stmt,  6, game.tag("Black").data(),  -1, SQLITE_STATIC);
    sqlite3_bind_text( stmt,  7, game.tag("Result").data(), -1, SQLITE_STATIC);
    sqlite3_bind_text( stmt,  8, game.pgn().data(),         -1, SQLITE_STATIC);
    sqlite3_bind_text( stmt,  9, game.fen().data(),         -1, SQLITE_STATIC);
    sqlite3_bind_text( stmt, 10, game.settings.data(),      -1, SQLITE_STATIC);
    sqlite3_bind_int64(stmt, 11, game.rowid);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc != SQLITE_DONE;
}

int Database::save_game(Game& game) {
    return game.rowid ? update_game(game) : insert_game(game);
}

unique_ptr<Game> Database::load_game(sqlite3_int64 rowid) {
    assert(rowid > 0);

    auto sql = "SELECT pgn, fen, settings FROM games WHERE rowid = ?";
    sqlite3_stmt* stmt = nullptr;
    auto rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return nullptr;
    }

    sqlite3_bind_int64(stmt, 1, rowid);
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return nullptr;
    }

    auto pgn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    auto fen = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

    auto game = make_unique<Game>(pgn, fen);
    if (!game) {
        sqlite3_finalize(stmt);
        return nullptr;
    }

    game->rowid    = rowid;
    game->settings = "";
    if (sqlite3_column_bytes(stmt, 2) > 0) {
        game->settings = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    }

    sqlite3_finalize(stmt);
    return game;
}

unique_ptr<Game> Database::load_latest(void) {
    const char* sql = "SELECT MAX(rowid) FROM games";
    sqlite3_stmt* stmt = nullptr;
    auto rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return nullptr;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return nullptr;
    }

    sqlite3_int64 rowid = sqlite3_column_int64(stmt, 0);
    sqlite3_finalize(stmt);
    return rowid > 0 ? load_game(rowid) : nullptr;
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
