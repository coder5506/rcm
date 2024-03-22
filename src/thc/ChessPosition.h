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

#include <cstddef>
#include <cstring>
#include <string>

namespace thc {

class Move;

struct DETAIL {
    union {
        struct {
            Square enpassant_target : 8;
            Square wking_square     : 8;
            Square bking_square     : 8;
            unsigned int  wking     : 1;    // Castling still allowed flags
            unsigned int  wqueen    : 1;    //  unfortunately if the castling
            unsigned int  bking     : 1;    //  flags are declared as bool,
            unsigned int  bqueen    : 1;    //  with Visual C++ at least,
                                            //  the details blow out and use
                                            //  another 32 bits (??!!)
            // Note that for say white king side castling to be allowed in
            //  the same sense as the Forsyth representation, not only
            //  must wking be true, but the  white king and king rook must
            //  be present and in position, see the wking_allowed() etc.
            //  methods in class ChessPosition, these are used for the ChessPosition
            //  == operator.
        };
        std::uint32_t detail;
    };

    DETAIL();

    bool eq_all(const DETAIL& other) const {
        return detail == other.detail;
    }

    bool eq_castling(const DETAIL& other) const {
        return (detail & 0xff000000) == (other.detail & 0xff000000);
    }

    bool eq_king_positions(const DETAIL& other) const {
        return (detail & 0x00ffff00) == (other.detail & 0x00ffff00);
    }

    bool eq_enpassant(const DETAIL& other) const {
        return enpassant_target == other.enpassant_target;
    }
};

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
    char squares[64 +1]; // +1 allows a trailing '\0'
    // note indexed according to Square convention, a8=0 etc.

    // Half moves since pawn move or capture (for 50 move rule)
    //  e.g. after 1.e4 it's 0
    int  half_move_clock{0};

    // Full move count. Initially 1 and increments after black moves
    //  e.g. after 1.e4 it's 1
    //  e.g. after 1... d6 it's 2
    int  full_move_count{1};

    // The following are deemed "details", and must be stored at the
    //  end of the structure. Search for DETAIL for, ahem, details.
    //  For performance reasons we want the details to be able to fit
    //  into 32 bits.
    DETAIL d;

    ChessPosition();

    // Groomed enpassant target is enpassant target qualified by the possibility to
    //  take enpassant. For example any double square pawn push creates an
    //  enpassant target, but a groomed enpassant target will still be SQUARE_INVALID
    //  unless there is an opposition pawn in position to make the capture
    Square groomed_enpassant_target() const;

    char at( Square sq ) const { return squares[sq]; }

    // Castling allowed ?
    bool wking_allowed()  const { return d.wking  && at(e1)=='K' && at(h1)=='R'; }
    bool wqueen_allowed() const { return d.wqueen && at(e1)=='K' && at(a1)=='R'; }
    bool bking_allowed()  const { return d.bking  && at(e8)=='k' && at(h8)=='r'; }
    bool bqueen_allowed() const { return d.bqueen && at(e8)=='k' && at(a8)=='r'; }

    // Set up position on board from Forsyth string with extensions
    //  return bool okay
    bool Forsyth(const char* txt);

    // Publish chess position and supplementary info in forsyth notation
    std::string fen() const;

    // Who's turn is it anyway
    inline bool WhiteToPlay() const { return white; }
    void Toggle() { white = !white; }
};

}

#endif
