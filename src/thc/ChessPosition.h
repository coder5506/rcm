/****************************************************************************
 * ChessPosition.h Chess classes - Representation of the position on the board
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/
#pragma once

#ifndef CHESSPOSITION_H
#define CHESSPOSITION_H

#include "ChessPositionRaw.h"

#include <cstddef>
#include <cstring>
#include <string>

namespace thc
{
    class Move;

// ChessPosition - A complete representation of the position on the
//  board.
class ChessPosition : public ChessPositionRaw
{
public:

    // Default constructor
    ChessPosition()  { Init(); }
    virtual ~ChessPosition()  {}    // destructor not actually needed now as
                                    //  we don't allocate resources in ctor.
    void Init()
    {
        white = true;
        memcpy(squares,
           "rnbqkbnr"
           "pppppppp"
           "        "
           "        "
           "        "
           "        "
           "PPPPPPPP"
           "RNBQKBNR", sizeof(squares));
        enpassant_target = SQUARE_INVALID;
        wking  = true;
        wqueen = true;
        bking  = true;
        bqueen = true;
        wking_square = e1;
        bking_square = e8;
        half_move_clock = 0;
        full_move_count = 1;
    }

    // Copy constructor and Assignment operator. Defining them this way
    //  generates simple bitwise memory copy, which is exactly what we
    //  want and is better practice than the old memcpy() versions (which
    //  copy the vtable ptr as well - we don't want that). Thanks to GitHub
    //  user metiscus for the pull request that fixed this.
    ChessPosition(const ChessPosition& src) = default;
    ChessPosition& operator=(const ChessPosition& src) = default;

    // Groomed enpassant target is enpassant target qualified by the possibility to
    //  take enpassant. For example any double square pawn push creates an
    //  enpassant target, but a groomed enpassant target will still be SQUARE_INVALID
    //  unless there is an opposition pawn in position to make the capture
    Square groomed_enpassant_target() const
    {
        Square ret = SQUARE_INVALID;
        if( white && a6<=enpassant_target && enpassant_target<=h6 )
        {
            bool zap=true;  // zap unless there is a 'P' in place
            int idx = enpassant_target+8; //idx = SOUTH(enpassant_target)
            if( enpassant_target>a6 && squares[idx-1]=='P' )
                zap = false;    // eg a5xb6 ep, through g5xh6 ep
            if( enpassant_target<h6 && squares[idx+1]=='P' )
                zap = false;    // eg b5xa6 ep, through h5xg6 ep
            if( !zap )
                ret = enpassant_target;
        }
        else if( !white && a3<=enpassant_target && enpassant_target<=h3 )
        {
            bool zap=true;  // zap unless there is a 'p' in place
            int idx = enpassant_target-8; //idx = NORTH(enpassant_target)
            if( enpassant_target>a3 && squares[idx-1]=='p' )
                zap = false;    // eg a4xb3 ep, through g4xh3 ep
            if( enpassant_target<h3 && squares[idx+1]=='p' )
                zap = false;    // eg b4xa3 ep, through h4xg3 ep
            if( !zap )
                ret = enpassant_target;
        }
        return ret;
    }

    // Castling allowed ?
    bool wking_allowed()  const { return wking  && squares[e1]=='K' && squares[h1]=='R'; }
    bool wqueen_allowed() const { return wqueen && squares[e1]=='K' && squares[a1]=='R'; }
    bool bking_allowed()  const { return bking  && squares[e8]=='k' && squares[h8]=='r'; }
    bool bqueen_allowed() const { return bqueen && squares[e8]=='k' && squares[a8]=='r'; }

    // For debug
    std::string ToDebugStr(const char* label = nullptr) const;

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
