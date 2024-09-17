/****************************************************************************
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

#include "fen.h"

#include <stdexcept>

using namespace std;
using namespace thc;


string fen::from_position(const ChessPosition& position) {
    int i, empty=0, file=0, rank=7, save_file=0, save_rank=0;
    Square sq;
    char p;
    std::string str;

    // Squares
    for( i=0; i<64; i++ )
    {
        sq = SQ('a'+file,'1'+rank);
        if( sq == position.d.enpassant_target )
        {
            save_file = file;
            save_rank = rank;
        }
        p = position.squares[sq];
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
    str += (position.white?'w':'b');

    // Castling flags
    str += ' ';
    if( !position.wking_allowed() && !position.wqueen_allowed() && !position.bking_allowed() && !position.bqueen_allowed() )
        str += '-';
    else
    {
        if( position.wking_allowed() )
            str += 'K';
        if( position.wqueen_allowed() )
            str += 'Q';
        if( position.bking_allowed() )
            str += 'k';
        if( position.bqueen_allowed() )
            str += 'q';
    }

    // Enpassant target square
    str += ' ';
    if (position.d.enpassant_target == SQUARE_INVALID || save_rank == 0) {
        str += '-';
    }
    else {
        char file2 = 'a'+(char)save_file;
        str += file2;
        char rank2 = '1'+(char)save_rank;
        str += rank2;
    }

    // Counts
    char buf[40];
    sprintf(buf, " %d %d", position.half_move_clock, position.full_move_count);
    str += buf;
    return str;
}


ChessPosition fen::to_position(const char* txt) {
    int   file, rank, skip, store, temp;
    int   count_wking=0, count_bking=0;
    char  c, cross;
    char  p;
    bool done;
    const char *reset = txt;

    // When store==0 validate txt without storing results
    ChessPosition result;
    for( store=0; store<2; store++ )
    {
        txt = reset;

        // Clear the board
        if( store )
        {
            for( Square square=a8; square<=h1; ++square )
            {
                result.squares[square]   = ' ';
            }

            // Clear the extension fields
            result.d.wking(false);
            result.d.wqueen(false);
            result.d.bking(false);
            result.d.bqueen(false);
            result.d.enpassant_target = SQUARE_INVALID;
            result.half_move_clock = 0;
            result.full_move_count = 1;
        }

        // Loop through the main Forsyth field
        for( file=0, rank=7, done=false; *txt && !done; )
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
                default:
                    throw domain_error("Invalid FEN string: " + string(txt));
            }

            // Store single piece or loop over multiple empty squares
            for( int i=0; i<skip && !done; i++ )
            {
                Square sq = SQ('a'+file, '1'+rank);
                if( store )
                {
                    result.squares[sq] = p;
                    if( p == 'K' )
                        result.d.wking_square = sq;
                    else if( p == 'k' )
                        result.d.bking_square = sq;
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
        if( *txt == '/'
            || *txt == '|'
            || *txt == '\\' )
            txt++;
        while( *txt==' ' || *txt=='\t' )
            txt++;
        if( *txt=='W' || *txt=='w' )
        {
            if( store )
                result.white = true;
            txt++;
        }
        else if( *txt=='B' || *txt=='b' )
        {
            if( store )
                result.white = false;
            txt++;
        }
        else {
            throw domain_error("Invalid FEN string: " + string(txt));
        }

        // Castling flags
        while( *txt==' ' || *txt=='\t' )
            txt++;
        if( *txt == '-' )
            txt++;
        else
        {
            for( int i=0; i<4; i++, txt++ )
            {
                if( *txt == 'K' )
                {
                    if( store )
                        result.d.wking(true);
                }
                else if( *txt == 'Q' )
                {
                    if( store )
                        result.d.wqueen(true);
                }
                else if( *txt == 'k' )
                {
                    if( store )
                        result.d.bking(true);
                }
                else if( *txt == 'q' )
                {
                    if( store )
                        result.d.bqueen(true);
                }
                else if( *txt == '-' )               // allow say "KQ-q "
                    ;
                else if( *txt == ' ' || *txt=='\t' ) // or "KQq "
                    break;
                else {
                    throw domain_error("Invalid FEN string: " + string(txt));
                }
            }
        }

        // Enpassant target
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
            else {
                throw domain_error("Invalid FEN string: " + string(txt));
            }
            if( '1'<=*txt && *txt<='8' )
                r = *txt++;
            else {
                throw domain_error("Invalid FEN string: " + string(txt));
            }
            if( store )
                result.d.enpassant_target = SQ(f,r);
        }

        // Half move clock
        while( *txt==' ' || *txt=='\t' )
            txt++;
        temp = atoi(txt);
        if( temp >= 0 )
        {
            auto okay = false;
            while( isascii(*txt) && isdigit(*txt) )
            {
                okay = true;
                txt++;
            }
            if (!okay) {
                throw domain_error("Invalid FEN string: " + string(txt));
            }
            if( store )
                result.half_move_clock = temp;
        }

        // Full move count
        while( *txt==' ' || *txt=='\t' )
            txt++;
        temp = atoi(txt);
        if( temp < 0 ) {
            throw domain_error("Invalid FEN string: " + string(txt));
        }
        else if( store )
            result.full_move_count = temp;
    }

    return result;
}
