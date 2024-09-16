/****************************************************************************
 * ChessPosition.cpp Chess classes - Representation of the position on the board
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

/* Some project notes */
/*====================*/

/****************************************************************************
 * Class Hierarchy:
 *
 *  The Chess classes are intended to provide a reusable chess subsystem. To
 *  understand the hierarchy of classes, think in terms of the Java concept
 *  of derived classes "extending" base classes.
 *
 *  So
 *    ChessPosition
 *      ;A simple C style structure that holds a chess position
 *    ChessRules      extends ChessPosition
 *      ;ChessRules adds all the rules of standard chess
 ****************************************************************************/

/****************************************************************************
 * Explanation of namespace thc:
 *
 *  The Chess classes use the C++ namespace facility to ensure they can be
 *  used without name conflicts with other 3rd party code. A short, but
 *  highly likely to be unique namespace name is best, we choose "thc" which
 *  is short for TripleHappyChess.
 ****************************************************************************/

/****************************************************************************
 * My original license, now replaced by MIT license full text of which is
 * in file LICENSE in project's root directory.
 *
 * Licensing provisions for all TripleHappy Chess source code;
 *
 * Start Date: 15 February 2003.
 * This software is licensed to be used freely. The following licensing
 * provisions apply;
 *
 * 1) The 'author' is asserted to be the original author of the code, Bill
 *    Forster of Wellington, New Zealand.
 * 2) The 'licensee' is anyone else who wishes to use the software.
 * 3) The 'licensee' is entitled to do anything they wish with the software
 *    EXCEPT for any action that attempts to restrict in any way the rights
 *    granted in 4) below.
 * 4) The 'author' is entitled to do anything he wishes with the software.
 *
 * The intent of this license is to allow the licensees wide freedom to
 * incorporate, modify and sell the software, with the single caveat that
 * they cannot prevent the author from either further development or future
 * commercial use of the software.
 ****************************************************************************/

#include "ChessPosition.h"
#include "Move.h"
#include "MoveGen.h"
#include "PrivateChessDefs.h"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;
using namespace thc;

ChessPosition::ChessPosition()
{
    memcpy(squares,
        "rnbqkbnr"
        "pppppppp"
        "        "
        "        "
        "        "
        "        "
        "PPPPPPPP"
        "RNBQKBNR", sizeof squares);
}

Square ChessPosition::groomed_enpassant_target() const {
    auto ret = SQUARE_INVALID;
    if (white && a6 <= d.enpassant_target && d.enpassant_target <= h6) {
        auto zap = true;  // zap unless there is a 'P' in place
        auto idx = d.enpassant_target + 8; //idx = SOUTH(d.enpassant_target)
        if (d.enpassant_target > a6 && squares[idx-1] == 'P') {
            zap = false;    // eg a5xb6 ep, through g5xh6 ep
        }
        if (d.enpassant_target < h6 && squares[idx+1] == 'P') {
            zap = false;    // eg b5xa6 ep, through h5xg6 ep
        }
        if (!zap) {
            ret = d.enpassant_target;
        }
    }
    else if (!white && a3 <= d.enpassant_target && d.enpassant_target <= h3) {
        bool zap = true;  // zap unless there is a 'p' in place
        auto idx = d.enpassant_target - 8; //idx = NORTH(d.enpassant_target)
        if (d.enpassant_target > a3 && squares[idx-1] == 'p') {
            zap = false;    // eg a4xb3 ep, through g4xh3 ep
        }
        if (d.enpassant_target < h3 && squares[idx+1] == 'p') {
            zap = false;    // eg b4xa3 ep, through h4xg3 ep
        }
        if (!zap) {
            ret = d.enpassant_target;
        }
    }
    return ret;
}


vector<Move> ChessPosition::GenLegalMoveList() const {
    return thc::GenLegalMoveList(*this);
}


bool ChessPosition::Evaluate() const {
    // Enemy king is attacked and our move, position is illegal
    const auto enemy_king = white ? d.bking_square : d.wking_square;
    return !thc::AttackedPiece(*this, enemy_king);
}


bool ChessPosition::Evaluate(const Move& move) const {
    return play_move(move).Evaluate();
}


ChessPosition ChessPosition::play_move(const Move& move) const {
    ChessPosition result{*this};
    result.apply_move(move);
    return result;
}


void ChessPosition::apply_move(const Move& move) {
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

    // Update castling prohibited flags for destination square, eg h8 -> bking
    switch (move.dst) {
    case a8: d.bqueen(false); break;
    case e8: d.bqueen(false);
    case h8: d.bking(false);  break;
    case a1: d.wqueen(false); break;
    case e1: d.wqueen(false);
    case h1: d.wking(false);  break;
    default:    // IMPORTANT - only dst is required since we also qualify
        break;  // castling with presence of rook and king on right squares.
    }           // (i.e., If a rook or king leaves its original square, the
                // castling prohibited flag is unaffected, but it doesn't matter
                // since we won't castle unless rook and king are present on the
                // right squares. If subsequently a king or rook returns, that's
                // okay too because the castling flag is cleared by its arrival
                // on the move.dst square, so castling remains prohibited).
    d.enpassant_target = SQUARE_INVALID;

    // Special handling might be required
    switch (move.special) {
    default:
        squares[move.dst] = squares[move.src];
        squares[move.src] = ' ';
        break;

    // King move updates king position in details field
    case SPECIAL_KING_MOVE:
        squares[move.dst] = squares[move.src];
        squares[move.src] = ' ';
        if (white) {
            d.wking_square = move.dst;
        }
        else {
            d.bking_square = move.dst;
        }
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_QUEEN:
        squares[move.src] = ' ';
        squares[move.dst] = (white?'Q':'q');
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_ROOK:
        squares[move.src] = ' ';
        squares[move.dst] = (white?'R':'r');
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_BISHOP:
        squares[move.src] = ' ';
        squares[move.dst] = (white?'B':'b');
        break;

    // In promotion case, dst piece doesn't equal src piece
    case SPECIAL_PROMOTION_KNIGHT:
        squares[move.src] = ' ';
        squares[move.dst] = (white?'N':'n');
        break;

    // White enpassant removes pawn south of destination
    case SPECIAL_WEN_PASSANT:
        squares[move.src] = ' ';
        squares[move.dst] = 'P';
        squares[SOUTH(move.dst)] = ' ';
        break;

    // Black enpassant removes pawn north of destination
    case SPECIAL_BEN_PASSANT:
        squares[move.src] = ' ';
        squares[move.dst] = 'p';
        squares[NORTH(move.dst)] = ' ';
        break;

    // White pawn advances 2 squares sets an enpassant target
    case SPECIAL_WPAWN_2SQUARES:
        squares[move.src] = ' ';
        squares[move.dst] = 'P';
        d.enpassant_target = SOUTH(move.dst);
        break;

    // Black pawn advances 2 squares sets an enpassant target
    case SPECIAL_BPAWN_2SQUARES:
        squares[move.src] = ' ';
        squares[move.dst] = 'p';
        d.enpassant_target = NORTH(move.dst);
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
    white = !white;
}


/****************************************************************************
 * Set up position on board from Forsyth string with extensions
 *   return bool okay
 ****************************************************************************/
bool ChessPosition::Forsyth(const char* txt) {
    int   file, rank, skip, store, temp;
    int   count_wking=0, count_bking=0;
    char  c, cross;
    char  p;
    bool okay, done;
    const char *reset = txt;

    // When store==0 validate txt without storing results
    for( store=0, okay=true; store<2 && okay; store++ )
    {
        txt = reset;

        // Clear the board
        if( store )
        {
            for( Square square=a8; square<=h1; ++square )
            {
                squares[square]   = ' ';
            }

            // Clear the extension fields
            d.wking(false);
            d.wqueen(false);
            d.bking(false);
            d.bqueen(false);
            d.enpassant_target = SQUARE_INVALID;
            half_move_clock = 0;
            full_move_count = 1;
        }

        // Loop through the main Forsyth field
        for( file=0, rank=7, done=false; *txt && okay && !done; )
        {
            skip = 1;
            c = *txt++;
            p = ' ';
            cross = ' ';
            switch(c)
            {
                case 'x':   cross = 'x';
                            skip = 1;
                            break;
                case ' ':
                case '\t':  done = true;            break;
                case 'k':   p = 'k';
                            count_bking++;          break;
                case 'K':   p = 'K';
                            count_wking++;          break;
                case 'p':
                case 'r':
                case 'n':
                case 'b':
                case 'q':
                case 'P':
                case 'R':
                case 'N':
                case 'B':
                case 'Q':   p = c;   break;
                case '1':   case '2':   case '3':   case '4':
                case '5':   case '6':   case '7':   case '8':
                            skip = c-'0';       break;
                case '/':   // official separator
                case '|':   //  .. we'll allow this
                case '\\':  //  .. or this
                {
                    if( file == 0 )
                        skip = 0;        // eg after "rn6/", '/' does nothing
                    else
                        skip = (8-file); // eg after "rn/", '/' skips 6 squares
                    while( *txt == '/'   // allow consecutive '/' characters
                      ||   *txt == '|'   //     (or '|' or '\')
                      ||   *txt == '\\' )
                    {
                        txt++;
                        skip += 8;
                    }
                    break;
                }
                default:    okay=false;
            }

            // Store single piece or loop over multiple empty squares
            for( int i=0; i<skip && okay && !done; i++ )
            {
                Square sq = SQ('a'+file, '1'+rank);
                if( store )
                {
                    squares[sq] = p;
                    if( p == 'K' )
                        d.wking_square = sq;
                    else if( p == 'k' )
                        d.bking_square = sq;
                }
                file++;
                if( file == 8 )
                {
                    file = 0;
                    rank--;
                }
                if( sq == h1 )
                    done = true;        // last square, we're done !
            }
        }

        // In validation pass make sure there's 1 white king and 1 black king
   /*   if( store==0 )  // disabled to allow ILLEGAL_NOT_ONE_KING_EACH message
        {
            if( count_wking!=1 || count_bking!=1 )
                okay = false;
        }  */

        // Now support standard FEN notation with extensions. See appendix to
        // .PGN standard on the Internet (and in project documentation)
        //
        // Briefly there are a total of 6 fields, the normal Forsyth encoding
        // (as above) plus 5 extension fields. Format of the 5 extension
        // fields is illustrated by example.
        //
        // Example extension fields; "w KQq e6 0 2"
        //
        // White to move,
        // All types of castling available, except black king side (so black's
        //  king rook has moved, other king and rook pieces unmoved). This
        //  whole field is replaced by '-' character if no castling available.
        // The last move played was a double pawn advance, and hence the
        //  'en-passant target square' field is NOT empty. The field is the
        //  square a potential capturing pawn would end up on (can deduce last
        //  move was e7e5)
        // Number of half-moves or ply since a pawn move or capture (for 50
        //  move rule)
        // Total (full-move) count (starts at 1, increments after black moves)

        // Who to move
        if( okay )
        {
            if( *txt == '/'
             || *txt == '|'
             || *txt == '\\' )
                txt++;
            while( *txt==' ' || *txt=='\t' )
                txt++;
            if( *txt=='W' || *txt=='w' )
            {
                if( store )
                    white = true;
                txt++;
            }
            else if( *txt=='B' || *txt=='b' )
            {
                if( store )
                    white = false;
                txt++;
            }
            else
                okay = false;
        }

        // Castling flags
        if( okay )
        {
            while( *txt==' ' || *txt=='\t' )
                txt++;
            if( *txt == '-' )
                txt++;
            else
            {
                for( int i=0; i<4 && okay; i++, txt++ )
                {
                    if( *txt == 'K' )
                    {
                        if( store )
                            d.wking(true);
                    }
                    else if( *txt == 'Q' )
                    {
                        if( store )
                            d.wqueen(true);
                    }
                    else if( *txt == 'k' )
                    {
                        if( store )
                            d.bking(true);
                    }
                    else if( *txt == 'q' )
                    {
                        if( store )
                            d.bqueen(true);
                    }
                    else if( *txt == '-' )               // allow say "KQ-q "
                        ;
                    else if( *txt == ' ' || *txt=='\t' ) // or "KQq "
                        break;
                    else
                        okay = false;
                }
            }
        }

        // Enpassant target
        if( okay )
        {
            while( *txt==' ' || *txt=='\t' )
                txt++;
            if( *txt == '-' )
                txt++;
            else
            {
                char f='a', r='1';
                if( 'a'<=*txt && *txt<='h' )
                    f = *txt++;
                else if( 'A'<=*txt && *txt<='H' )
                {
                    f = *txt++;
                    f = f-'A'+'a';
                }
                else
                    okay = false;
                if( okay )
                {
                    if( '1'<=*txt && *txt<='8' )
                        r = *txt++;
                    else
                        okay = false;
                }
                if( okay && store )
                    d.enpassant_target = SQ(f,r);
            }
        }

        // Half move clock
        if( okay )
        {
            okay=false;
            while( *txt==' ' || *txt=='\t' )
                txt++;
            temp = atoi(txt);
            if( temp >= 0 )
            {
                while( isascii(*txt) && isdigit(*txt) )
                {
                    okay = true;
                    txt++;
                }
                if( okay && store )
                    half_move_clock = temp;
            }
        }

        // Full move count
        if( okay )
        {
            while( *txt==' ' || *txt=='\t' )
                txt++;
            temp = atoi(txt);
            if( temp < 0 )
                okay = false;
            else if( store )
                full_move_count = temp;
        }
    }
    return( okay );
}

// Publish chess position and supplementary info in forsyth notation
std::string ChessPosition::fen() const {
    int i, empty=0, file=0, rank=7, save_file=0, save_rank=0;
    Square sq;
    char p;
    std::string str;

    // Squares
    for( i=0; i<64; i++ )
    {
        sq = SQ('a'+file,'1'+rank);
        if( sq == d.enpassant_target )
        {
            save_file = file;
            save_rank = rank;
        }
        p = squares[sq];
        if( p == ' ' )
            empty++;
        else
        {
            if( empty )
            {
                char count = '0' + (char)empty;
                str += count;
                empty = 0;
            }
            str += p;
        }
        file++;
        if( file == 8 )
        {
            if( empty )
            {
                char count = '0'+(char)empty;
                str += count;
            }
            if( rank )
                str += '/';
            empty = 0;
            file = 0;
            rank--;
        }
    }

    // Who to move
    str += ' ';
    str += (white?'w':'b');

    // Castling flags
    str += ' ';
    if( !wking_allowed() && !wqueen_allowed() && !bking_allowed() && !bqueen_allowed() )
        str += '-';
    else
    {
        if( wking_allowed() )
            str += 'K';
        if( wqueen_allowed() )
            str += 'Q';
        if( bking_allowed() )
            str += 'k';
        if( bqueen_allowed() )
            str += 'q';
    }

    // Enpassant target square
    str += ' ';
    if( d.enpassant_target==SQUARE_INVALID || save_rank==0 )
        str += '-';
    else
    {
        char file2 = 'a'+(char)save_file;
        str += file2;
        char rank2 = '1'+(char)save_rank;
        str += rank2;
    }

    // Counts
    char buf[40];
    sprintf( buf, " %d %d", half_move_clock, full_move_count );
    str += buf;
    return( str );
}
