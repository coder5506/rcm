// Copyright (C) 2024 Eric Sessoms
// See license at end of file
#pragma once

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "chess_position.h"
#include "../utility/model.h"

#include <ctime>
#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

#include <sqlite3.h>

class Game : public Model<Game>, public Observer<Game> {
public:
    std::vector<PositionPtr> history;
    std::time_t   started{0};
    sqlite3_int64 rowid{0};  // SQLite ROWID
    std::string   settings;  // Opaque
    std::map<std::string, std::string> tags;

    explicit Game(std::string_view pgn = {}, std::string_view fen = {});
    Game(const Game&) = default;

    // Re-initialize to start new game
    void clear();

    // Initialize new game with starting position given as FEN
    void fen(std::string_view fen);

    // Restore game from PGN
    void pgn(std::string_view pgn);

    std::string& tag(const std::string& key);

    void on_changed(Game&) override;

    // Current position as Forsyth-Edwards Notation
    std::string fen() const;

    // Game as Portable Game Notation
    std::string pgn() const;

    PositionPtr current() const;
    PositionPtr previous() const;
    PositionPtr start() const;

    // Piece at square
    char at(thc::Square square) const;

    Bitmap bitmap() const { return current()->bitmap(); }
    inline bool WhiteToPlay() const { return current()->WhiteToPlay(); }

    // Legal moves in current position
    MoveList legal_moves() const;

    // Parse move given in Standard Algebraic Notation
    thc::Move san_move(std::string_view san_move) const;

    // Parse move given in pure coordinate notation (as used in UCI)
    thc::Move uci_move(std::string_view uci_move) const;

    void play_move(thc::Move move);
    void play_san_move(std::string_view san_move);
    void play_uci_move(std::string_view uci_move);

    void play_takeback();
    void play_takeback(thc::Move move);

    void revise_move(thc::Move takeback, thc::Move move);

    bool read_move(
        Bitmap            boardstate,
        const ActionList& actions,
        MoveList&         candidates,
        std::optional<thc::Move>& takeback);

private:
    // Write PGN
    void write_tags(std::ostream&) const;
    void write_move(
        std::ostream&,
        PositionPtr before,
        thc::Move   move,
        bool        show_move_number) const;
    void write_moves(std::ostream&, PositionPtr before, bool is_first_move) const;
    void write_movetext(std::ostream&) const;
    void write_pgn(std::ostream&) const;

    // Read PGN
    void read_tags(char*&);
    bool read_movetext(char*&);

    // Restore from PGN and FEN
    bool recover_history(PositionPtr target);
    void recover_position(std::string_view fen);
};

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
