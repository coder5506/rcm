/****************************************************************************
 * Chessdefs.h Chess classes - Common definitions
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/
#pragma once

#ifndef CHESSDEFS_H
#define CHESSDEFS_H

#include <cctype>
#include <cstdint>

// Simple definition to aid platform portability (only remains of former Portability.h)
int strcmp_ignore(const char* s, const char* t); // return 0 if case-insensitive match

namespace thc {

// Use the most natural square convention possible; Define Square to
//  correspond to a conventionally oriented chess diagram; Top left corner
//  (square a8) is 0, bottom right corner (square h1) is 63.
// Note that instead of defining a special piece type, we use the built-in
//  char type, with 'N'=white knight, 'b'=black bishop etc. and ' '=an
//  empty square.
enum Square {
    a8=0,
        b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
    SQUARE_INVALID
};

// Allow easy iteration through squares
inline Square operator++(Square& sq, int) {
    auto result = sq;
    sq = static_cast<Square>(sq+1);
    return result;
}

inline Square operator++(Square& sq) {
    sq = static_cast<Square>(sq+1);
    return sq;
}

// Check whether a piece is black, white or an empty square, should really make
//  these and most other macros into inline functions
inline bool IsEmptySquare(char p) { return p == ' '; }
inline bool IsBlack(char p) { return std::islower(p); }
inline bool IsWhite(char p) { return std::isupper(p); }

//  eg SQ('c','5') -> c5
//  (We didn't always have such a sensible Square convention. SQ() remains
//  useful for cases like SQ(file,rank), but you may actually see examples
//  like the hardwired SQ('c','5') which can safely be changed to simply
//  c5).
inline Square SQ(char f, char r) { return static_cast<Square>(('8'-r)*8 + (f-'a')); }

inline int IFILE(Square sq) { return sq % 8; }                          // eg c5->2
inline int IRANK(Square sq) { return 7 - sq / 8; }                      // eg c5->4

inline char FILE(Square sq) { return 'a' + IFILE(sq); }                 // eg c5->'c'
inline char RANK(Square sq) { return '1' + IRANK(sq); }                 // eg c5->'5'

inline Square SOUTH(Square sq) { return static_cast<Square>(sq + 8); }  // eg c5->c4
inline Square NORTH(Square sq) { return static_cast<Square>(sq - 8); }  // eg c5->c6

inline Square SW(Square sq) { return static_cast<Square>(sq + 7); }     // eg c5->b4
inline Square SE(Square sq) { return static_cast<Square>(sq + 9); }     // eg c5->d4
inline Square NW(Square sq) { return static_cast<Square>(sq - 9); }     // eg c5->b6
inline Square NE(Square sq) { return static_cast<Square>(sq - 7); }     // eg c5->d6

inline int SW(int sq) { return sq + 7; }  // eg c5->b4
inline int SE(int sq) { return sq + 9; }  // eg c5->d4
inline int NW(int sq) { return sq - 9; }  // eg c5->b6
inline int NE(int sq) { return sq - 7; }  // eg c5->d6

// thc::Square utilities
inline char get_file(Square sq) { return FILE(sq); }  // eg c5->'c'
inline char get_rank(Square sq) { return RANK(sq); }  // eg c5->'5'
inline Square make_square(char file, char rank) {     // eg ('c','5') -> c5
    return SQ(file, rank);
}

// Special (i.e. not ordinary) move types
enum SPECIAL {
    NOT_SPECIAL = 0,
    SPECIAL_KING_MOVE,     // special only because it changes wking_square, bking_square
    SPECIAL_WK_CASTLING,
    SPECIAL_BK_CASTLING,
    SPECIAL_WQ_CASTLING,
    SPECIAL_BQ_CASTLING,
    SPECIAL_PROMOTION_QUEEN,
    SPECIAL_PROMOTION_ROOK,
    SPECIAL_PROMOTION_BISHOP,
    SPECIAL_PROMOTION_KNIGHT,
    SPECIAL_WPAWN_2SQUARES,
    SPECIAL_BPAWN_2SQUARES,
    SPECIAL_WEN_PASSANT,
    SPECIAL_BEN_PASSANT,
};

// Results of a test for legal position, note that they are powers
//  of 2, allowing a mask of reasons
enum ILLEGAL_REASON {
    IR_NULL                  =  0,
    IR_PAWN_POSITION         =  1,  //pawns on 1st or 8th rank
    IR_NOT_ONE_KING_EACH     =  2,
    IR_CAN_TAKE_KING         =  4,
    IR_WHITE_TOO_MANY_PIECES =  8,
    IR_WHITE_TOO_MANY_PAWNS  = 16,
    IR_BLACK_TOO_MANY_PIECES = 32,
    IR_BLACK_TOO_MANY_PAWNS  = 64,
};

// Types of draw checked by IsDraw()
enum DRAWTYPE {
    NOT_DRAW,
    DRAWTYPE_50MOVE,
    DRAWTYPE_INSUFFICIENT,      // draw if superior side wants it
                                //  since inferior side has insufficient
                                //  mating material
    DRAWTYPE_INSUFFICIENT_AUTO, // don't wait to be asked, e.g. draw
                                //  immediately if bare kings
    DRAWTYPE_REPITITION,
};

// Stalemate or checkmate game terminations
enum TERMINAL {
    NOT_TERMINAL        =  0,
    TERMINAL_WCHECKMATE = -1,   // White is checkmated
    TERMINAL_WSTALEMATE = -2,   // White is stalemated
    TERMINAL_BCHECKMATE =  1,   // Black is checkmated
    TERMINAL_BSTALEMATE =  2    // Black is stalemated
};

// Calculate an upper limit to the length of a list of moves
#define MAXMOVES (27 + 2*13 + 2*14 + 2*8 + 8 + 8*4  +  3*27)
                //[Q   2*B    2*R    2*N   K   8*P] +  [3*Q]
                //             ^                         ^
                //[calculated practical maximum   ] + [margin]

// Types we'd really rather have in PrivateChessDefs.h, but not possible
//  at the moment, so we reluctantly expose them to users of the chess
//  classes.
using lte = unsigned char;   // lte = lookup table element

}

#endif
