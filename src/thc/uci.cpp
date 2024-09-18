/****************************************************************************
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

#include "uci.h"
#include "ChessPosition.h"

#include <stdexcept>

using namespace std;
using namespace thc;


string uci::from_move(const Move& move) {
    // null move should be "0000" according to UCI spec
    char result[6] = {0};
    if (move.src != move.dst) {
        result[0] = FILE(move.src);
        result[1] = RANK(move.src);
        result[2] = FILE(move.dst);
        result[3] = RANK(move.dst);
        switch (move.special) {
        case SPECIAL_PROMOTION_QUEEN:  result[4] = 'q'; break;
        case SPECIAL_PROMOTION_ROOK:   result[4] = 'r'; break;
        case SPECIAL_PROMOTION_BISHOP: result[4] = 'b'; break;
        case SPECIAL_PROMOTION_KNIGHT: result[4] = 'n'; break;
        default:
            break;
        }
    }
    return result;
}


Move uci::to_move(const ChessPosition& position, string_view uci_move) {
    if (uci_move.size() < 4 ||
        uci_move[0] < 'a' || 'h' < uci_move[0] ||
        uci_move[1] < '1' || '8' < uci_move[1] ||
        uci_move[2] < 'a' || 'h' < uci_move[2] ||
        uci_move[3] < '1' || '8' < uci_move[3])
    {
        throw domain_error("Invalid UCI move: " + string(uci_move));
    }

    SPECIAL special = NOT_SPECIAL;
    switch (uci_move[4]) {
    case 'q': special = SPECIAL_PROMOTION_QUEEN;  break;
    case 'r': special = SPECIAL_PROMOTION_ROOK;   break;
    case 'b': special = SPECIAL_PROMOTION_BISHOP; break;
    case 'n': special = SPECIAL_PROMOTION_KNIGHT; break;
    default:
        break;
    }

    const Move expected{SQ(uci_move[0], uci_move[1]), SQ(uci_move[2], uci_move[3]), special};
    for (const auto candidate : position.legal_moves()) {
        if (candidate.src != expected.src || candidate.dst != expected.dst) {
            continue;
        }
        if (candidate.is_promotion() || expected.is_promotion()) {
            if (candidate.special != expected.special) {
                continue;
            }
        }
        return candidate;
    }
    throw domain_error("Invalid UCI move: " + string(uci_move));
}
