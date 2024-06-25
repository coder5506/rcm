/****************************************************************************
 * ChessPosition.h Chess classes - Representation of the position on the board
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/
#pragma once

#ifndef CHESSPOSITION_H
#define CHESSPOSITION_H

#include "ChessDefs.h"

#include <bitset>
#include <string>

namespace thc {

class Move;


class Castling {
private:
    enum {
        W_KING  = 1,
        W_QUEEN = 2,
        B_KING  = 4,
        B_QUEEN = 8
    };
    unsigned char flags;

public:
    Castling() : flags{0x0F} {}

    bool wking()  const { return flags & W_KING; }
    bool wqueen() const { return flags & W_QUEEN; }
    bool bking()  const { return flags & B_KING; }
    bool bqueen() const { return flags & B_QUEEN; }

    void wking(bool val)  { if (val) flags |= W_KING;  else flags &= ~W_KING; }
    void wqueen(bool val) { if (val) flags |= W_QUEEN; else flags &= ~W_QUEEN; }
    void bking(bool val)  { if (val) flags |= B_KING;  else flags &= ~B_KING; }
    void bqueen(bool val) { if (val) flags |= B_QUEEN; else flags &= ~B_QUEEN; }

    friend bool operator==(const Castling& lhs, const Castling& rhs);
    friend bool operator!=(const Castling& lhs, const Castling& rhs);
};

static_assert(sizeof(Castling) == sizeof(unsigned char));

inline bool operator==(const Castling& lhs, const Castling& rhs) {
    return lhs.flags == rhs.flags;
}

inline bool operator!=(const Castling& lhs, const Castling& rhs) {
    return lhs.flags != rhs.flags;
}


class DETAIL {
public:
    Square   enpassant_target{SQUARE_INVALID};
    Square   wking_square{e1};
    Square   bking_square{e8};
    Castling castling;

    bool wking()  const { return castling.wking(); }
    bool wqueen() const { return castling.wqueen(); }
    bool bking()  const { return castling.bking(); }
    bool bqueen() const { return castling.bqueen(); }

    void wking(bool val)  { castling.wking(val); }
    void wqueen(bool val) { castling.wqueen(val); }
    void bking(bool val)  { castling.bking(val); }
    void bqueen(bool val) { castling.bqueen(val); }
};

static_assert(sizeof(DETAIL) == sizeof(unsigned int));

inline bool eq_castling(const DETAIL& lhs, const DETAIL& rhs) {
    return lhs.castling == rhs.castling;
}

inline bool operator==(const DETAIL& lhs, const DETAIL& rhs) {
    return reinterpret_cast<const unsigned int&>(lhs) == reinterpret_cast<const unsigned int&>(rhs);
}

inline bool operator!=(const DETAIL& lhs, const DETAIL& rhs) {
    return reinterpret_cast<const unsigned int&>(lhs) != reinterpret_cast<const unsigned int&>(rhs);
}


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

    // Groomed enpassant target is enpassant target qualified by the possibility to
    //  take enpassant. For example any double square pawn push creates an
    //  enpassant target, but a groomed enpassant target will still be SQUARE_INVALID
    //  unless there is an opposition pawn in position to make the capture
    Square groomed_enpassant_target() const;

    char at(Square sq) const { return squares[sq]; }

    // Castling allowed ?
    bool wking_allowed()  const { return d.wking()  && at(e1)=='K' && at(h1)=='R'; }
    bool wqueen_allowed() const { return d.wqueen() && at(e1)=='K' && at(a1)=='R'; }
    bool bking_allowed()  const { return d.bking()  && at(e8)=='k' && at(h8)=='r'; }
    bool bqueen_allowed() const { return d.bqueen() && at(e8)=='k' && at(a8)=='r'; }

    // Set up position on board from Forsyth string with extensions
    //  return bool okay
    bool Forsyth(const char* txt);

    // Publish chess position and supplementary info in forsyth notation
    std::string fen() const;

    // Who's turn is it anyway
    inline bool WhiteToPlay() const { return  white; }
    inline bool BlackToPlay() const { return !white; }
    void Toggle() { white = !white; }
};

}

#endif
