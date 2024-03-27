/****************************************************************************
 * Move.cpp Chess classes - Move
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

#include "Move.h"

#include <stdexcept>

using namespace std;
using namespace thc;

Move::Move(Square src, Square dst, SPECIAL special, char capture)
    : src{src}, dst{dst}, special{special}, capture{capture}
{
}

Move::Move(string_view uci_move) {
    if (uci_move.size() < 4 ||
        uci_move[0] < 'a' || 'h' < uci_move[0] ||
        uci_move[1] < '1' || '8' < uci_move[1] ||
        uci_move[2] < 'a' || 'h' < uci_move[2] ||
        uci_move[3] < '1' || '8' < uci_move[3])
    {
        throw std::invalid_argument("Invalid UCI move: " + string(uci_move));
    }

    src = SQ(uci_move[0], uci_move[1]);
    dst = SQ(uci_move[2], uci_move[3]);
    if (uci_move.size() == 4) {
        return;
    }

    switch (uci_move[4]) {
    case 'q':
        special = SPECIAL_PROMOTION_QUEEN;
        break;
    case 'r':
        special = SPECIAL_PROMOTION_ROOK;
        break;
    case 'b':
        special = SPECIAL_PROMOTION_BISHOP;
        break;
    case 'n':
        special = SPECIAL_PROMOTION_KNIGHT;
        break;
    default:
        break;
    }
}

string Move::uci() const {
    char tmove[6];
    if( src == dst )   // null move should be "0000" according to UCI spec
    {
        tmove[0] = '0';
        tmove[1] = '0';
        tmove[2] = '0';
        tmove[3] = '0';
        tmove[4] = '\0';
    }
    else
    {
        tmove[0] = FILE(src);
        tmove[1] = RANK(src);
        tmove[2] = FILE(dst);
        tmove[3] = RANK(dst);
        if( special == SPECIAL_PROMOTION_QUEEN )
            tmove[4] = 'q';
        else if( special == SPECIAL_PROMOTION_ROOK )
            tmove[4] = 'r';
        else if( special == SPECIAL_PROMOTION_BISHOP )
            tmove[4] = 'b';
        else if( special == SPECIAL_PROMOTION_KNIGHT )
            tmove[4] = 'n';
        else
            tmove[4] = '\0';
        tmove[5] = '\0';
    }
    return tmove;
}
