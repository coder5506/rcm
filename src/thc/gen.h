/****************************************************************************
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/
#pragma once

#ifndef GEN_H
#define GEN_H

#include "ChessDefs.h"
#include "Move.h"

#include <vector>

namespace thc {

class ChessPosition;


namespace gen {

std::vector<Move> GenLegalMoveList(const ChessPosition& position);
std::vector<Move> GenMoveList(const ChessPosition& position);

void GenLegalMoveList(
    const ChessPosition& position,
    std::vector<Move>& moves,
    std::vector<bool>& check,
    std::vector<bool>& mate,
    std::vector<bool>& stalemate);

bool AttackedPiece(const ChessPosition& position, Square square);
bool AttackedSquare(const ChessPosition& position, Square square, bool enemy_is_white);
bool Evaluate(const ChessPosition& position, TERMINAL& score_terminal);
void KingMoves(const ChessPosition& position, Square square, std::vector<Move>& moves);

}

}

#endif
