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
