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

    Game(std::string_view pgn = {}, std::string_view fen = {});
    Game(const Game&) = default;

    void on_changed(Game&) override;

    // Re-initialize to start new game
    void clear();

    // Initialize new game with starting position given as FEN
    void fen(std::string_view fen);

    // Restore game from PGN
    void pgn(std::string_view pgn);

    // Current position as Forsyth-Edwards Notation
    std::string fen() const;

    // Game as Portable Game Notation
    std::string pgn() const;

    PositionPtr current() const;
    PositionPtr previous() const;
    PositionPtr start() const;

    inline bool WhiteToPlay() const { return current()->WhiteToPlay(); }
    inline bool BlackToPlay() const { return current()->BlackToPlay(); }

    inline int half_move_clock() const { return current()->half_move_clock; }
    inline int full_move_count() const { return current()->full_move_count; }
    inline const thc::DETAIL& d() const { return current()->d; }

    // Piece at square
    char at(thc::Square square) const;

    inline bool wking_allowed() const { return current()->wking_allowed(); }
    inline bool wqueen_allowed() const { return current()->wqueen_allowed(); }
    inline bool bking_allowed() const { return current()->bking_allowed(); }
    inline bool bqueen_allowed() const { return current()->bqueen_allowed(); }

    // Parse move given in Standard Algebraic Notation
    thc::Move san_move(std::string_view san_move) const;

    // Parse move given in pure coordinate notation (as used in UCI)
    thc::Move uci_move(std::string_view uci_move) const;

    std::string move_san(const thc::Move& move) const {
        return current()->move_san(move);
    }

    void play_move(thc::Move move);
    void play_san_move(std::string_view san_move);
    void play_uci_move(std::string_view uci_move);

    void play_takeback();
    void play_takeback(thc::Move move);

    void revise_move(thc::Move takeback, thc::Move move);

    // Legal moves in current position
    MoveList legal_moves() const;

    Bitmap bitmap() const { return current()->bitmap(); }

    bool read_move(
        Bitmap               boardstate,
        const ActionHistory& actions,
        MoveList&            candidates,
        std::optional<thc::Move>& takeback);


    std::string& tag(const std::string& key);

    bool IsInsufficientDraw(bool white_asks, thc::DRAWTYPE& result) const;
    int GetRepetitionCount() const;
    bool IsDraw(bool white_asks, thc::DRAWTYPE& result) const;
    bool IsLegal(thc::ILLEGAL_REASON& reason) const;

private:
    // Restore from PGN and FEN
    bool recover_history(PositionPtr target);
    void recover_position(std::string_view fen);

    bool read_revised_move(
        Bitmap                    boardstate,
        MoveList&                 candidates,
        std::optional<thc::Move>& takeback);
    bool read_simple_takeback(Bitmap boardstate, std::optional<thc::Move>& takeback);
    bool read_multiple_takeback(Bitmap boardstate, std::optional<thc::Move>& takeback);
    bool read_takeback(Bitmap boardstate, std::optional<thc::Move>& takeback);
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
