/****************************************************************************
 * Move.cpp Chess classes - Move
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

#include "Move.h"
#include "ChessRules.h"
#include "PrivateChessDefs.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;
using namespace thc;

Move::Move(Square src, Square dst, SPECIAL special, int capture)
    : src{src}, dst{dst}, special{special}, capture{capture}
{
}

Move::Move(string_view uci_move)
    : special{NOT_SPECIAL}, capture{' '}
{
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

// Convert to natural string, eg "Nf3"
std::string Move::NaturalOut(ChessRules* cr) {
// Improved algorithm

    /* basic procedure is run the following algorithms in turn:
        pawn move     ?
        castling      ?
        Nd2 or Nxd2   ? (loop through all legal moves check if unique)
        Nbd2 or Nbxd2 ? (loop through all legal moves check if unique)
        N1d2 or N1xd2 ? (loop through all legal moves check if unique)
        Nb1d2 or Nb1xd2 (fallback if nothing else works)
    */

    char nmove[10];
    nmove[0] = '-';
    nmove[1] = '-';
    nmove[2] = '\0';
    vector<Move> list;
    vector<bool> check;
    vector<bool> mate;
    vector<bool> stalemate;
    enum
    {
        ALG_PAWN_MOVE,
        ALG_CASTLING,
        ALG_ND2,
        ALG_NBD2,
        ALG_N1D2,
        ALG_NB1D2
    };
    bool done=false;
    bool found = false;
    char append='\0';
    cr->GenLegalMoveList(list, check, mate, stalemate);
    for (int i = 0; i != list.size(); ++i) {
        Move mfound = list[i];
        if( mfound == *this )
        {
            found = true;
            if( mate[i] )
                append = '#';
            else if( check[i] )
                append = '+';
        }
    }

    // Loop through algorithms
    for( int alg=ALG_PAWN_MOVE; found && !done && alg<=ALG_NB1D2; alg++ )
    {
        bool do_loop = (alg==ALG_ND2 || alg==ALG_NBD2 || alg==ALG_N1D2);
        int matches=0;

        // Run the algorithm on the input move (i=-1) AND on all legal moves
        //  in a loop if do_loop set for this algorithm (i=0 to i=count-1)
        for (auto i = -1; !done && i < (do_loop ? int(list.size()) : 0); i++) {
            char compare[10];
            char *str_dst = (i == -1) ? nmove : compare;
            Move m = (i == -1) ? *this : list[i];
            Square src_ = m.src;
            Square dst_ = m.dst;
            char t, p = cr->squares[src_];
            if( islower(p) )
                p = (char)toupper(p);
            if( !IsEmptySquare(m.capture) ) // until we did it this way, enpassant was '-' instead of 'x'
                t = 'x';
            else
                t = '-';
            switch( alg )
            {
                // pawn move ? "e4" or "exf6", plus "=Q" etc if promotion
                case ALG_PAWN_MOVE:
                {
                    if( p == 'P' )
                    {
                        done = true;
                        if( t == 'x' )
                            sprintf( nmove, "%cx%c%c", FILE(src_),FILE(dst_),RANK(dst_) );
                        else
                            sprintf( nmove, "%c%c",FILE(dst_),RANK(dst_) );
                        char *s = strchr(nmove,'\0');
                        switch( m.special )
                        {
                            case SPECIAL_PROMOTION_QUEEN:
                                strcpy( s, "=Q" );  break;
                            case SPECIAL_PROMOTION_ROOK:
                                strcpy( s, "=R" );  break;
                            case SPECIAL_PROMOTION_BISHOP:
                                strcpy( s, "=B" );  break;
                            case SPECIAL_PROMOTION_KNIGHT:
                                strcpy( s, "=N" );  break;
                            default:
                                break;
                        }
                    }
                    break;
                }

                // castling ?
                case ALG_CASTLING:
                {
                    if( m.special==SPECIAL_WK_CASTLING || m.special==SPECIAL_BK_CASTLING )
                    {
                        strcpy( nmove, "O-O" );
                        done = true;
                    }
                    else if( m.special==SPECIAL_WQ_CASTLING || m.special==SPECIAL_BQ_CASTLING )
                    {
                        strcpy( nmove, "O-O-O" );
                        done = true;
                    }
                    break;
                }

                // Nd2 or Nxd2
                case ALG_ND2:
                {
                    if( t == 'x' )
                        sprintf( str_dst, "%cx%c%c", p, FILE(dst_), RANK(dst_) );
                    else
                        sprintf( str_dst, "%c%c%c", p, FILE(dst_), RANK(dst_) );
                    if( i >= 0 )
                    {
                        if( 0 == strcmp(nmove,compare) )
                            matches++;
                    }
                    break;
                }

                // Nbd2 or Nbxd2
                case ALG_NBD2:
                {
                    if( t == 'x' )
                        sprintf( str_dst, "%c%cx%c%c", p, FILE(src_), FILE(dst_), RANK(dst_) );
                    else
                        sprintf( str_dst, "%c%c%c%c", p, FILE(src_), FILE(dst_), RANK(dst_) );
                    if( i >= 0 )
                    {
                        if( 0 == strcmp(nmove,compare) )
                            matches++;
                    }
                    break;
                }

                // N1d2 or N1xd2
                case ALG_N1D2:
                {
                    if( t == 'x' )
                        sprintf( str_dst, "%c%cx%c%c", p, RANK(src_), FILE(dst_), RANK(dst_) );
                    else
                        sprintf( str_dst, "%c%c%c%c", p, RANK(src_), FILE(dst_), RANK(dst_) );
                    if( i >= 0 )
                    {
                        if( 0 == strcmp(nmove,compare) )
                            matches++;
                    }
                    break;
                }

                //  Nb1d2 or Nb1xd2
                case ALG_NB1D2:
                {
                    done = true;
                    if( t == 'x' )
                        sprintf( nmove, "%c%c%cx%c%c", p, FILE(src_), RANK(src_), FILE(dst_), RANK(dst_) );
                    else
                        sprintf( nmove, "%c%c%c%c%c", p, FILE(src_), RANK(src_), FILE(dst_), RANK(dst_) );
                    break;
                }
            }
        }   // end loop for all legal moves with given algorithm

        // If it's a looping algorithm and only one move matches nmove, we're done
        if( do_loop && matches==1 )
            done = true;
    }   // end loop for all algorithms
    if( append )
    {
        char *s = strchr(nmove,'\0');
        *s++ = append;
        *s = '\0';
    }
    return nmove;
}

// Convert to terse string eg "e7e8q"
std::string Move::TerseOut() {
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
