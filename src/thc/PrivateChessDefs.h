/****************************************************************************
 * PrivateChessDefs.h Chess classes - Internal implementation details
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/
#pragma once

#ifndef PRIVATE_CHESS_DEFS_H_INCLUDED
#define PRIVATE_CHESS_DEFS_H_INCLUDED

#include "ChessDefs.h"

namespace thc {

// Macro to convert chess notation to Square convention,
//  eg SQ('c','5') -> c5
//  (We didn't always have such a sensible Square convention. SQ() remains
//  useful for cases like SQ(file,rank), but you may actually see examples
//  like the hardwired SQ('c','5') which can safely be changed to simply
//  c5).
#define SQ(f,r)  ( (Square) ( ('8'-(r))*8 + ((f)-'a') )   )

// More Square macros
#define FILE(sq)    ( (char) (  ((sq)&0x07) + 'a' ) )           // eg c5->'c'
#define RANK(sq)    ( (char) (  '8' - (((sq)>>3) & 0x07) ) )    // eg c5->'5'
#define IFILE(sq)   (  (int)(sq) & 0x07 )                       // eg c5->2
#define IRANK(sq)   (  7 - ((((int)(sq)) >>3) & 0x07) )         // eg c5->4
#define SOUTH(sq)   (  (Square)((sq) + 8) )                     // eg c5->c4
#define NORTH(sq)   (  (Square)((sq) - 8) )                     // eg c5->c6
#define SW(sq)      (  (Square)((sq) + 7) )                     // eg c5->b4
#define SE(sq)      (  (Square)((sq) + 9) )                     // eg c5->d4
#define NW(sq)      (  (Square)((sq) - 9) )                     // eg c5->b6
#define NE(sq)      (  (Square)((sq) - 7) )                     // eg c5->d6

// Utility macro
#ifndef nbrof
    #define nbrof(array) (sizeof((array))/sizeof((array)[0]))
#endif

// Convert piece, e.g. 'N' to bitmask in lookup tables. See automatically
//  PrivateChessDefs.cpp and GeneratedLookupTables.h for format of
//  lookup tables
extern lte to_mask[];

// Lookup squares a queen can move to
extern const lte* queen_lookup[];

// Lookup squares a rook can move to
extern const lte* rook_lookup[];

// Lookup squares a bishop can move to
extern const lte* bishop_lookup[];

// Lookup squares a knight can move to
extern const lte* knight_lookup[];

// Lookup squares a king can move to
extern const lte* king_lookup[];

// Lookup squares a white pawn can move to
extern const lte* pawn_white_lookup[];

// Lookup squares a black pawn can move to
extern const lte* pawn_black_lookup[];

// Lookup good squares for enemy king when a king is on a square in an endgame
extern const lte* good_king_position_lookup[];

// Lookup squares from which an enemy pawn attacks white
extern const lte* pawn_attacks_white_lookup[];

// Lookup squares from which an enemy pawn attacks black
extern const lte* pawn_attacks_black_lookup[];

// Lookup squares from which enemy pieces attack white
extern const lte* attacks_white_lookup[];

// Lookup squares from which enemy pieces attack black
extern const lte* attacks_black_lookup[];

}

#endif
