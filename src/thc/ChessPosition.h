/****************************************************************************
 * ChessPosition.h Chess classes - Representation of the position on the board
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/
#pragma once

#ifndef CHESSPOSITION_H
#define CHESSPOSITION_H

#include "Detail.h"
#include "Move.h"

#include <string>
#include <vector>

namespace thc {

class ChessPosition {
public:
    // Who to play
    bool white{true};

    // Pieces on board in readable form; Forsyth like but without compression,
    //  eg "rnbqkbnr"
    //     "pppppppp"
    //     "        "
    //     "        "
    //     "        "
    //     "        "
    //     "PPPPPPPP"
    //     "RNBQKBNR"
    //  (represents starting position)
    char squares[64 +1];  // +1 allows a trailing '\0'
    // note indexed according to Square convention, a8=0 etc.

    // Half moves since pawn move or capture (for 50 move rule)
    //  e.g. after 1.e4 it's 0
    int  half_move_clock{0};

    // Full move count. Initially 1 and increments after black moves
    //  e.g. after 1.e4 it's 1
    //  e.g. after 1... d6 it's 2
    int  full_move_count{1};

    DETAIL d;

    ChessPosition();
    ChessPosition(const ChessPosition&) = default;

    bool Forsyth(const char*);
    std::string fen() const;

    // Who's turn is it anyway?
    inline bool WhiteToPlay() const { return  white; }
    inline bool BlackToPlay() const { return !white; }
    void Toggle() { white = !white; }

    char at(Square sq) const { return squares[sq]; }

    // Castling allowed?
    bool wking_allowed()  const { return d.wking()  && at(e1)=='K' && at(h1)=='R'; }
    bool wqueen_allowed() const { return d.wqueen() && at(e1)=='K' && at(a1)=='R'; }
    bool bking_allowed()  const { return d.bking()  && at(e8)=='k' && at(h8)=='r'; }
    bool bqueen_allowed() const { return d.bqueen() && at(e8)=='k' && at(a8)=='r'; }

    // Groomed enpassant target is enpassant target qualified by the possibility
    // to take enpassant. For example any double square pawn push creates an
    // enpassant target, but a groomed enpassant target will still be
    // SQUARE_INVALID unless there is an opposition pawn in position to make the
    // capture
    Square groomed_enpassant_target() const;

    Square king_square() const { return white ? d.wking_square : d.bking_square; }

    Move san_move(std::string_view) const;
    Move uci_move(std::string_view) const;
    std::string move_san(const Move&) const;
    std::string move_uci(const Move&) const;

    ChessPosition play_move(const Move&) const;
    ChessPosition play_san_move(std::string_view) const;
    ChessPosition play_uci_move(std::string_view) const;

    bool Evaluate() const;
    bool Evaluate(const Move&) const;

    std::vector<Move> legal_moves() const;

private:
    void apply_move(const Move&);
};

}

// True if two positions are equivalent, without considering moves played.
bool operator==(const thc::ChessPosition& lhs, const thc::ChessPosition& rhs);

#endif
