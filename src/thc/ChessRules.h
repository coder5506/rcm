/****************************************************************************
 * ChessRules.h Chess classes - Rules of chess
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/
#pragma once

#ifndef CHESSRULES_H
#define CHESSRULES_H

#include "ChessPosition.h"
#include "Move.h"

#include <array>
#include <vector>

namespace thc {

// Class encapsulates state of game and operations available
class ChessRules: public ChessPosition {
private:
    // Move history is a ring array
    std::array<Move, 256> history;        // must be 256 ..
    unsigned char history_idx{1};         // .. so this loops around naturally
    // 1 => prevent bogus repetition draws

    // Detail stack is a ring array
    std::array<DETAIL, 256> detail_stack;  // must be 256 ..
    unsigned char detail_idx{0};           // .. so this loops around naturally

public:
    //  Test for legal position, sets reason to a mask of possibly multiple reasons
    bool IsLegal(ILLEGAL_REASON& reason);

    // Play a move
    void PlayMove(Move imove);

    // Check draw rules (50 move rule etc.)
    bool IsDraw(bool white_asks, DRAWTYPE& result);

    // Get number of times position has been repeated
    int GetRepetitionCount();

    // Check insufficient material draw rule
    bool IsInsufficientDraw(bool white_asks, DRAWTYPE& result);

    // Evaluate a position, returns bool okay (not okay means illegal position)
    bool Evaluate();
    bool Evaluate(TERMINAL& score_terminal);

    // Is a square is attacked by enemy ?
    bool AttackedSquare(Square square, bool enemy_is_white);

    // Determine if an occupied square is attacked
    bool AttackedPiece(Square square);

    // Create a list of all legal moves in this position
    void GenLegalMoveList(std::vector<Move>& moves);

    //  Create a list of all legal moves in this position, with extra info
    void GenLegalMoveList(std::vector<Move>& moves,
                          std::vector<bool>& check,
                          std::vector<bool>& mate,
                          std::vector<bool>& stalemate);

    // Make a move (with the potential to undo)
    void PushMove(Move m);

    // Undo a move
    void PopMove(Move m);

private:
    // Generate a list of all possible moves in a position (including
    //  illegally "moving into check")
    void GenMoveList(std::vector<Move>& moves);

    // Generate moves for pieces that move along multi-move rays (B,R,Q)
    void LongMoves(std::vector<Move>& moves, Square square, const lte* ptr);

    // Generate moves for pieces that move along single-move rays (K,N,P)
    void ShortMoves(std::vector<Move>& moves, Square square, const lte* ptr, SPECIAL special);

    // Generate list of king moves
    void KingMoves(std::vector<Move>& moves, Square square);

    // Generate list of white pawn moves
    void WhitePawnMoves(std::vector<Move>& moves, Square square);

    // Generate list of black pawn moves
    void BlackPawnMoves(std::vector<Move>& moves, Square square);

    // Evaluate a position, returns bool okay (not okay means illegal position)
    bool Evaluate(std::vector<Move> *p, TERMINAL& score_terminal);
};

}

#endif
