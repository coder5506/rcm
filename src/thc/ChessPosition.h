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

/* DETAIL is shorthand for the section of type ChessPosition that looks
   like this;

    Square enpassant_target : 8;
    Square wking_square     : 8;
    Square bking_square     : 8;
    int  wking              : 1;
    int  wqueen             : 1;
    int  bking              : 1;
    int  bqueen             : 1;

  We assume it is located in the last 4 bytes of ChessPosition,
  hence the definition of typedef DETAIL as unsigned long, and
  of DETAIL_ADDR below. We assume that ANDing the unsigned
  character at this address + 3, with ~WKING, where WKING
  is defined as unsigned char 0x01, will clear wking. See the
  definition of DETAIL_CASTLING and castling_prohibited_table[].
  These assumptions are likely not portable and are tested in
  TestInternals(). If porting this code, step through that code
  first and make any adjustments necessary */
using DETAIL = std::uint32_t;

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
        DETAIL detail;
    };

    ChessPosition();
    virtual ~ChessPosition() = default;

    // Groomed enpassant target is enpassant target qualified by the possibility to
    //  take enpassant. For example any double square pawn push creates an
    //  enpassant target, but a groomed enpassant target will still be SQUARE_INVALID
    //  unless there is an opposition pawn in position to make the capture
    Square groomed_enpassant_target() const;

    // Castling allowed ?
    bool wking_allowed()  const { return wking  && squares[e1]=='K' && squares[h1]=='R'; }
    bool wqueen_allowed() const { return wqueen && squares[e1]=='K' && squares[a1]=='R'; }
    bool bking_allowed()  const { return bking  && squares[e8]=='k' && squares[h8]=='r'; }
    bool bqueen_allowed() const { return bqueen && squares[e8]=='k' && squares[a8]=='r'; }

    // Set up position on board from Forsyth string with extensions
    //  return bool okay
    virtual bool Forsyth(const char* txt);

    // Publish chess position and supplementary info in forsyth notation
    std::string ForsythPublish();

    // Who's turn is it anyway
    inline bool WhiteToPlay() const { return white; }
    void Toggle() { white = !white; }
};

}

#endif
