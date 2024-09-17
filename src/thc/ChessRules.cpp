/****************************************************************************
 * ChessRules.cpp Chess classes - Rules of chess
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

#include "ChessRules.h"
#include "PrivateChessDefs.h"
#include "gen.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

using namespace std;
using namespace thc;

// Undo a move
void ChessRules::PopMove(Move m) {
    // Previous detail field
    d = detail_stack.back();
    detail_stack.pop_back();

    // Toggle who-to-move
    Toggle();

    // Special handling might be required
    switch (m.special) {
    default:
        squares[m.src] = squares[m.dst];
        squares[m.dst] = m.capture;
        break;

    // For promotion, src piece was a pawn
    case SPECIAL_PROMOTION_QUEEN:
    case SPECIAL_PROMOTION_ROOK:
    case SPECIAL_PROMOTION_BISHOP:
    case SPECIAL_PROMOTION_KNIGHT:
        if (white) {
            squares[m.src] = 'P';
        }
        else {
            squares[m.src] = 'p';
        }
        squares[m.dst] = m.capture;
        break;

    // White enpassant re-insert black pawn south of destination
    case SPECIAL_WEN_PASSANT:
        squares[m.src] = 'P';
        squares[m.dst] = ' ';
        squares[SOUTH(m.dst)] = 'p';
        break;

    // Black enpassant re-insert white pawn north of destination
    case SPECIAL_BEN_PASSANT:
        squares[m.src] = 'p';
        squares[m.dst] = ' ';
        squares[NORTH(m.dst)] = 'P';
        break;

    // Castling moves update 4 squares each
    case SPECIAL_WK_CASTLING:
        squares[e1] = 'K';
        squares[f1] = ' ';
        squares[g1] = ' ';
        squares[h1] = 'R';
        break;
    case SPECIAL_WQ_CASTLING:
        squares[e1] = 'K';
        squares[d1] = ' ';
        squares[c1] = ' ';
        squares[a1] = 'R';
        break;
    case SPECIAL_BK_CASTLING:
        squares[e8] = 'k';
        squares[f8] = ' ';
        squares[g8] = ' ';
        squares[h8] = 'r';
        break;
    case SPECIAL_BQ_CASTLING:
        squares[e8] = 'k';
        squares[d8] = ' ';
        squares[c8] = ' ';
        squares[a8] = 'r';
        break;
    }
}

// Make a move (with the potential to undo)
void ChessRules::PushMove(Move m) {
    // Push old details onto stack
    detail_stack.push_back(d);

    // Update castling prohibited flags for destination square, eg h8 -> bking
    switch (m.dst) {
    case a8: d.bqueen(false); break;
    case e8: d.bqueen(false);
    case h8: d.bking(false);  break;
    case a1: d.wqueen(false); break;
    case e1: d.wqueen(false);
    case h1: d.wking(false);  break;
    default:        // IMPORTANT - only dst is required since we also qualify
        break;      //  castling with presence of rook and king on right squares.
    }               //  (I.E. if a rook or king leaves its original square, the
                    //  castling prohibited flag is unaffected, but it doesn't
                    //  matter since we won't castle unless rook and king are
                    //  present on the right squares. If subsequently a king or
                    //  rook returns, that's okay too because the  castling flag
                    //  is cleared by its arrival on the m.dst square, so
                    //  castling remains prohibited).
    d.enpassant_target = SQUARE_INVALID;

    // Special handling might be required
    switch (m.special) {
    default:
        squares[m.dst] = squares[m.src];
        squares[m.src] = ' ';
        break;

    // King move updates king position in details field
    case SPECIAL_KING_MOVE:
        squares[m.dst] = squares[m.src];
        squares[m.src] = ' ';
        if (white) {
            d.wking_square = m.dst;
        }
        else {
            d.bking_square = m.dst;
        }
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_QUEEN:
        squares[m.src] = ' ';
        squares[m.dst] = (white?'Q':'q');
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_ROOK:
        squares[m.src] = ' ';
        squares[m.dst] = (white?'R':'r');
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_BISHOP:
        squares[m.src] = ' ';
        squares[m.dst] = (white?'B':'b');
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_KNIGHT:
        squares[m.src] = ' ';
        squares[m.dst] = (white?'N':'n');
        break;

    // White enpassant removes pawn south of destination
    case SPECIAL_WEN_PASSANT:
        squares[m.src] = ' ';
        squares[m.dst] = 'P';
        squares[ SOUTH(m.dst) ] = ' ';
        break;

    // Black enpassant removes pawn north of destination
    case SPECIAL_BEN_PASSANT:
        squares[m.src] = ' ';
        squares[m.dst] = 'p';
        squares[ NORTH(m.dst) ] = ' ';
        break;

    // White pawn advances 2 squares sets an enpassant target
    case SPECIAL_WPAWN_2SQUARES:
        squares[m.src] = ' ';
        squares[m.dst] = 'P';
        d.enpassant_target = SOUTH(m.dst);
        break;

    // Black pawn advances 2 squares sets an enpassant target
    case SPECIAL_BPAWN_2SQUARES:
        squares[m.src] = ' ';
        squares[m.dst] = 'p';
        d.enpassant_target = NORTH(m.dst);
        break;

    // Castling moves update 4 squares each
    case SPECIAL_WK_CASTLING:
        squares[e1] = ' ';
        squares[f1] = 'R';
        squares[g1] = 'K';
        squares[h1] = ' ';
        d.wking_square = g1;
        break;
    case SPECIAL_WQ_CASTLING:
        squares[e1] = ' ';
        squares[d1] = 'R';
        squares[c1] = 'K';
        squares[a1] = ' ';
        d.wking_square = c1;
        break;
    case SPECIAL_BK_CASTLING:
        squares[e8] = ' ';
        squares[f8] = 'r';
        squares[g8] = 'k';
        squares[h8] = ' ';
        d.bking_square = g8;
        break;
    case SPECIAL_BQ_CASTLING:
        squares[e8] = ' ';
        squares[d8] = 'r';
        squares[c8] = 'k';
        squares[a8] = ' ';
        d.bking_square = c8;
        break;
    }

    // Toggle who-to-move
    Toggle();
}

// Play a move
void ChessRules::PlayMove(Move move) {
    // Legal move - save it in history
    history.push_back(move);

    // Update full move count
    if (!white) {
        ++full_move_count;
    }

    // Update half move clock
    if (squares[move.src] == 'P' || squares[move.src] == 'p') {
        half_move_clock = 0;   // pawn move
    }
    else if (!IsEmptySquare(move.capture)) {
        half_move_clock = 0;   // capture
    }
    else {
        ++half_move_clock;    // neither pawn move nor capture
    }

    // Actually play the move
    PushMove(move);
}
// Is a square is attacked by enemy ?
bool ChessRules::AttackedSquare(Square square, bool enemy_is_white) const {
    return gen::AttackedSquare(*this, square, enemy_is_white);
}

// Determine if an occupied square is attacked
bool ChessRules::AttackedPiece(Square square) const {
    return gen::AttackedPiece(*this, square);
}

// Generate list of king moves
void ChessRules::KingMoves(vector<Move>& moves, Square square) const {
    gen::KingMoves(*this, square, moves);
}

void ChessRules::GenLegalMoveList(vector<Move>& moves) const {
    moves = GenLegalMoveList();
}

bool ChessRules::Evaluate(TERMINAL &score_terminal) const {
    return gen::Evaluate(*this, score_terminal);
}

// Create a list of all legal moves in this position, with extra info
void ChessRules::GenLegalMoveList(vector<Move>& moves,
                                  vector<bool>& check,
                                  vector<bool>& mate,
                                  vector<bool>& stalemate) const
{
    gen::GenLegalMoveList(*this, moves, check, mate, stalemate);
}

// Check insufficient material draw rule
bool ChessRules::IsInsufficientDraw(bool white_asks, DRAWTYPE& result) const {
    char   piece;
    int    piece_count=0;
    bool   bishop_or_knight=false, lone_wking=true, lone_bking=true;
    bool   draw=false;

    // Loop through the board
    for( Square square=a8; square<=h1; ++square )
    {
        piece = squares[square];
        switch( piece )
        {
            case 'B': case 'b':
            case 'N': case 'n': bishop_or_knight = true;  // and fall through
            case 'Q': case 'q':
            case 'R': case 'r':
            case 'P': case 'p':
                piece_count++;
                if( isupper(piece) )
                    lone_wking = false;
                else
                    lone_bking = false;
                break;
        }
        if( !lone_wking && !lone_bking )
            break;  // quit early for performance
    }

    // Automatic draw if K v K or K v K+N or K v K+B
    //  (note that K+B v K+N etc. is not auto granted due to
    //   selfmates in the corner)
    if( piece_count==0 ||
        (piece_count==1 && bishop_or_knight)
      )
    {
        draw = true;
        result = DRAWTYPE_INSUFFICIENT_AUTO;
    }
    else {
        // Otherwise side playing against lone K can claim a draw
        if( white_asks && lone_bking )
        {
            draw   = true;
            result = DRAWTYPE_INSUFFICIENT;
        }
        else if( !white_asks && lone_wking )
        {
            draw   = true;
            result = DRAWTYPE_INSUFFICIENT;
        }
    }
    return( draw );
}

// Get number of times position has been repeated
int ChessRules::GetRepetitionCount() {
    auto matches = 0;

    //  Save those aspects of current position that are changed by multiple
    //  PopMove() calls as we search backwards (i.e. squares, white,
    //  detail, detail_idx)
    char save_squares[sizeof(squares)];
    memcpy(save_squares, squares, sizeof save_squares);
    const auto save_detail_stack = detail_stack;
    bool          save_white     = white;
    DETAIL tmp{d};

    // Search backwards ....
    size_t nbr_half_moves = (full_move_count - 1) * 2 + (!white ? 1 : 0);
    nbr_half_moves = std::min(nbr_half_moves, history.size());
    nbr_half_moves = std::min(nbr_half_moves, detail_stack.size());
    auto idx = history.size();
    for (auto i = 0; i < nbr_half_moves; i++) {
        Move m = history[--idx];
        PopMove(m);

        // ... looking for matching positions
        if (white          == save_white       &&  // quick ones first!
            d.wking_square == tmp.wking_square &&
            d.bking_square == tmp.bking_square &&
            memcmp(squares, save_squares, sizeof squares) == 0)
        {
            matches++;
            if (d == tmp) {  // Castling flags and/or enpassant target different?
                continue;
            }

            // It might not be a match (but it could be - we have to unpack what the differences
            //  really mean)
            auto revoke_match = false;

            // Revoke match if different value of en-passant target square means different
            //  en passant possibilities
            if (d.enpassant_target != tmp.enpassant_target) {
                int ep_saved = tmp.enpassant_target;
                int ep_now   = d.enpassant_target;

                // Work out whether each en_passant is a real one, i.e. is there an opposition
                //  pawn in place to capture (if not it's just a double pawn advance with no
                //  actual enpassant consequences)
                auto real=false;
                auto ep = ep_saved;
                char const *squ = save_squares;
                for (auto j = 0; j < 2; j++) {
                    if (ep == a6) {
                            real = (squ[SE(ep)] == 'P');
                    }
                    else if (b6 <= ep && ep <= g6) {
                            real = (squ[SW(ep)] == 'P' || squ[SE(ep)] == 'P');
                    }
                    else if (ep == h6) {
                            real = (squ[SW(ep)] == 'P');
                    }
                    else if (ep == a3) {
                            real = (squ[NE(ep)] == 'p');
                    }
                    else if (b3 <= ep && ep <= g3) {
                            real = (squ[NE(ep)] == 'p' || squ[NW(ep)] == 'p');
                    }
                    else if (ep == h3) {
                            real = (squ[NW(ep)] == 'p' );
                    }
                    if (j > 0) {
                        ep_now = real ? ep : SQUARE_INVALID;    // evaluate second time through
                    }
                    else {
                        ep_saved = real ? ep : SQUARE_INVALID;  // evaluate first time through
                        ep = ep_now;                            // setup second time through
                        squ = squares;
                        real = false;
                    }
                }

                // If for example one en_passant is real and the other not, it's not a real match
                if (ep_saved != ep_now) {
                    revoke_match = true;
                }
            }

            // Revoke match if different value of castling flags means different
            //  castling possibilities
            if (!revoke_match && !eq_castling(d, tmp)) {
                bool wking_saved  = save_squares[e1]=='K' && save_squares[h1]=='R' && tmp.wking();
                bool wking_now    = squares[e1]=='K' && squares[h1]=='R' && d.wking();
                bool bking_saved  = save_squares[e8]=='k' && save_squares[h8]=='r' && tmp.bking();
                bool bking_now    = squares[e8]=='k' && squares[h8]=='r' && d.bking();
                bool wqueen_saved = save_squares[e1]=='K' && save_squares[a1]=='R' && tmp.wqueen();
                bool wqueen_now   = squares[e1]=='K' && squares[a1]=='R' && d.wqueen();
                bool bqueen_saved = save_squares[e8]=='k' && save_squares[a8]=='r' && tmp.bqueen();
                bool bqueen_now   = squares[e8]=='k' && squares[a8]=='r' && d.bqueen();
                revoke_match = (
                    wking_saved  != wking_now  ||
                    bking_saved  != bking_now  ||
                    wqueen_saved != wqueen_now ||
                    bqueen_saved != bqueen_now
                );
            }

            // If the real castling or enpassant possibilities differ, it's not a match
            //  At one stage we just did a naive binary match of the details - not good enough. For example
            //  a rook moving away from h1 doesn't affect the WKING flag, but does disallow white king side
            //  castling
            if (revoke_match) {
                matches--;
            }
        }

        // For performance reasons, abandon search early if pawn move
        //  or capture
        if( squares[m.src]=='P' || squares[m.src]=='p' || !IsEmptySquare(m.capture) )
            break;
    }

    // Restore current position
    memcpy(squares, save_squares, sizeof squares);
    white      = save_white;
    detail_stack = save_detail_stack;
    d = tmp;
    return matches+1;  // +1 counts original position
}

// Check draw rules (50 move rule etc.)
bool ChessRules::IsDraw(bool white_asks, DRAWTYPE& result) const {
    bool   draw=false;

    // Insufficient mating material
    draw =  IsInsufficientDraw( white_asks, result );

    // 50 move rule
    if( !draw && half_move_clock>=100 )
    {
        result = DRAWTYPE_50MOVE;
        draw = true;
    }

    // 3 times repetition,
    if( !draw && const_cast<ChessRules&>(*this).GetRepetitionCount()>=3 )
    {
        result = DRAWTYPE_REPITITION;
        draw = true;
    }

    if( !draw )
        result = NOT_DRAW;
    return( draw );
}

// Test for legal position, sets reason to a mask of possibly multiple reasons
bool ChessRules::IsLegal(ILLEGAL_REASON& reason) const {
    int  ireason = 0;
    int  wkings=0, bkings=0, wpawns=0, bpawns=0, wpieces=0, bpieces=0;
    bool legal = true;
    int  file, rank;
    char p;
    Square opposition_king_location = SQUARE_INVALID;

    // Loop through the board
    file=0;     // go from a8,b8..h8,a7,b7..h1
    rank=7;
    for (;;) {
        Square sq = SQ('a'+file, '1'+rank);
        p = squares[sq];
        if ((p == 'P' || p == 'p') && (rank == 0 || rank == 7)) {
            legal = false;
            ireason |= IR_PAWN_POSITION;
        }
        if (IsWhite(p)) {
            if (p == 'P') {
                wpawns++;
            }
            else {
                wpieces++;
                if (p == 'K') {
                    wkings++;
                    if (!white) {
                        opposition_king_location = sq;
                    }
                }
            }
        }
        else if (IsBlack(p)) {
            if (p == 'p') {
                bpawns++;
            }
            else {
                bpieces++;
                if (p == 'k') {
                    bkings++;
                    if (white) {
                        opposition_king_location = sq;
                    }
                }
            }
        }
        if( sq == h1 ) {
            break;
        }
        else {
            file++;
            if (file == 8) {
                file = 0;
                rank--;
            }
        }
    }
    if (wkings!=1 || bkings!=1) {
        legal = false;
        ireason |= IR_NOT_ONE_KING_EACH;
    }
    if (opposition_king_location!=SQUARE_INVALID && AttackedPiece(opposition_king_location)) {
        legal = false;
        ireason |= IR_CAN_TAKE_KING;
    }
    if (wpieces>8 && (wpieces+wpawns)>16) {
        legal = false;
        ireason |= IR_WHITE_TOO_MANY_PIECES;
    }
    if (bpieces>8 && (bpieces+bpawns)>16) {
        legal = false;
        ireason |= IR_BLACK_TOO_MANY_PIECES;
    }
    if (wpawns > 8) {
        legal = false;
        ireason |= IR_WHITE_TOO_MANY_PAWNS;
    }
    if (bpawns > 8) {
        legal = false;
        ireason |= IR_BLACK_TOO_MANY_PAWNS;
    }
    reason = static_cast<ILLEGAL_REASON>(ireason);
    return legal;
}
