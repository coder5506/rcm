/****************************************************************************
 * GeneratedLookupTables.h These lookup tables are machine generated
 *  They require prior definitions of;
 *   squares (a1,a2..h8)
 *   pieces (P,N,B,N,R,Q,K)
 *   lte (=lookup table element, a type for the lookup tables, eg int or unsigned char)
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/

// Queen, up to 8 rays
static const lte queen_a1[] = {
(lte)3
    ,(lte)7 ,(lte)b1 ,(lte)c1 ,(lte)d1 ,(lte)e1 ,(lte)f1 ,(lte)g1 ,(lte)h1
    ,(lte)7 ,(lte)a2 ,(lte)a3 ,(lte)a4 ,(lte)a5 ,(lte)a6 ,(lte)a7 ,(lte)a8
    ,(lte)7 ,(lte)b2 ,(lte)c3 ,(lte)d4 ,(lte)e5 ,(lte)f6 ,(lte)g7 ,(lte)h8
};
static const lte queen_a2[] = {
(lte)5
    ,(lte)7 ,(lte)b2 ,(lte)c2 ,(lte)d2 ,(lte)e2 ,(lte)f2 ,(lte)g2 ,(lte)h2
    ,(lte)1 ,(lte)a1
    ,(lte)6 ,(lte)a3 ,(lte)a4 ,(lte)a5 ,(lte)a6 ,(lte)a7 ,(lte)a8
    ,(lte)6 ,(lte)b3 ,(lte)c4 ,(lte)d5 ,(lte)e6 ,(lte)f7 ,(lte)g8
    ,(lte)1 ,(lte)b1
};
static const lte queen_a3[] = {
(lte)5
    ,(lte)7 ,(lte)b3 ,(lte)c3 ,(lte)d3 ,(lte)e3 ,(lte)f3 ,(lte)g3 ,(lte)h3
    ,(lte)2 ,(lte)a2 ,(lte)a1
    ,(lte)5 ,(lte)a4 ,(lte)a5 ,(lte)a6 ,(lte)a7 ,(lte)a8
    ,(lte)5 ,(lte)b4 ,(lte)c5 ,(lte)d6 ,(lte)e7 ,(lte)f8
    ,(lte)2 ,(lte)b2 ,(lte)c1
};
static const lte queen_a4[] = {
(lte)5
    ,(lte)7 ,(lte)b4 ,(lte)c4 ,(lte)d4 ,(lte)e4 ,(lte)f4 ,(lte)g4 ,(lte)h4
    ,(lte)3 ,(lte)a3 ,(lte)a2 ,(lte)a1
    ,(lte)4 ,(lte)a5 ,(lte)a6 ,(lte)a7 ,(lte)a8
    ,(lte)4 ,(lte)b5 ,(lte)c6 ,(lte)d7 ,(lte)e8
    ,(lte)3 ,(lte)b3 ,(lte)c2 ,(lte)d1
};
static const lte queen_a5[] = {
(lte)5
    ,(lte)7 ,(lte)b5 ,(lte)c5 ,(lte)d5 ,(lte)e5 ,(lte)f5 ,(lte)g5 ,(lte)h5
    ,(lte)4 ,(lte)a4 ,(lte)a3 ,(lte)a2 ,(lte)a1
    ,(lte)3 ,(lte)a6 ,(lte)a7 ,(lte)a8
    ,(lte)3 ,(lte)b6 ,(lte)c7 ,(lte)d8
    ,(lte)4 ,(lte)b4 ,(lte)c3 ,(lte)d2 ,(lte)e1
};
static const lte queen_a6[] = {
(lte)5
    ,(lte)7 ,(lte)b6 ,(lte)c6 ,(lte)d6 ,(lte)e6 ,(lte)f6 ,(lte)g6 ,(lte)h6
    ,(lte)5 ,(lte)a5 ,(lte)a4 ,(lte)a3 ,(lte)a2 ,(lte)a1
    ,(lte)2 ,(lte)a7 ,(lte)a8
    ,(lte)2 ,(lte)b7 ,(lte)c8
    ,(lte)5 ,(lte)b5 ,(lte)c4 ,(lte)d3 ,(lte)e2 ,(lte)f1
};
static const lte queen_a7[] = {
(lte)5
    ,(lte)7 ,(lte)b7 ,(lte)c7 ,(lte)d7 ,(lte)e7 ,(lte)f7 ,(lte)g7 ,(lte)h7
    ,(lte)6 ,(lte)a6 ,(lte)a5 ,(lte)a4 ,(lte)a3 ,(lte)a2 ,(lte)a1
    ,(lte)1 ,(lte)a8
    ,(lte)1 ,(lte)b8
    ,(lte)6 ,(lte)b6 ,(lte)c5 ,(lte)d4 ,(lte)e3 ,(lte)f2 ,(lte)g1
};
static const lte queen_a8[] = {
(lte)3
    ,(lte)7 ,(lte)b8 ,(lte)c8 ,(lte)d8 ,(lte)e8 ,(lte)f8 ,(lte)g8 ,(lte)h8
    ,(lte)7 ,(lte)a7 ,(lte)a6 ,(lte)a5 ,(lte)a4 ,(lte)a3 ,(lte)a2 ,(lte)a1
    ,(lte)7 ,(lte)b7 ,(lte)c6 ,(lte)d5 ,(lte)e4 ,(lte)f3 ,(lte)g2 ,(lte)h1
};
static const lte queen_b1[] = {
(lte)5
    ,(lte)1 ,(lte)a1
    ,(lte)6 ,(lte)c1 ,(lte)d1 ,(lte)e1 ,(lte)f1 ,(lte)g1 ,(lte)h1
    ,(lte)7 ,(lte)b2 ,(lte)b3 ,(lte)b4 ,(lte)b5 ,(lte)b6 ,(lte)b7 ,(lte)b8
    ,(lte)1 ,(lte)a2
    ,(lte)6 ,(lte)c2 ,(lte)d3 ,(lte)e4 ,(lte)f5 ,(lte)g6 ,(lte)h7
};
static const lte queen_b2[] = {
(lte)8
    ,(lte)1 ,(lte)a2
    ,(lte)6 ,(lte)c2 ,(lte)d2 ,(lte)e2 ,(lte)f2 ,(lte)g2 ,(lte)h2
    ,(lte)1 ,(lte)b1
    ,(lte)6 ,(lte)b3 ,(lte)b4 ,(lte)b5 ,(lte)b6 ,(lte)b7 ,(lte)b8
    ,(lte)1 ,(lte)a1
    ,(lte)1 ,(lte)a3
    ,(lte)6 ,(lte)c3 ,(lte)d4 ,(lte)e5 ,(lte)f6 ,(lte)g7 ,(lte)h8
    ,(lte)1 ,(lte)c1
};
static const lte queen_b3[] = {
(lte)8
    ,(lte)1 ,(lte)a3
    ,(lte)6 ,(lte)c3 ,(lte)d3 ,(lte)e3 ,(lte)f3 ,(lte)g3 ,(lte)h3
    ,(lte)2 ,(lte)b2 ,(lte)b1
    ,(lte)5 ,(lte)b4 ,(lte)b5 ,(lte)b6 ,(lte)b7 ,(lte)b8
    ,(lte)1 ,(lte)a2
    ,(lte)1 ,(lte)a4
    ,(lte)5 ,(lte)c4 ,(lte)d5 ,(lte)e6 ,(lte)f7 ,(lte)g8
    ,(lte)2 ,(lte)c2 ,(lte)d1
};
static const lte queen_b4[] = {
(lte)8
    ,(lte)1 ,(lte)a4
    ,(lte)6 ,(lte)c4 ,(lte)d4 ,(lte)e4 ,(lte)f4 ,(lte)g4 ,(lte)h4
    ,(lte)3 ,(lte)b3 ,(lte)b2 ,(lte)b1
    ,(lte)4 ,(lte)b5 ,(lte)b6 ,(lte)b7 ,(lte)b8
    ,(lte)1 ,(lte)a3
    ,(lte)1 ,(lte)a5
    ,(lte)4 ,(lte)c5 ,(lte)d6 ,(lte)e7 ,(lte)f8
    ,(lte)3 ,(lte)c3 ,(lte)d2 ,(lte)e1
};
static const lte queen_b5[] = {
(lte)8
    ,(lte)1 ,(lte)a5
    ,(lte)6 ,(lte)c5 ,(lte)d5 ,(lte)e5 ,(lte)f5 ,(lte)g5 ,(lte)h5
    ,(lte)4 ,(lte)b4 ,(lte)b3 ,(lte)b2 ,(lte)b1
    ,(lte)3 ,(lte)b6 ,(lte)b7 ,(lte)b8
    ,(lte)1 ,(lte)a4
    ,(lte)1 ,(lte)a6
    ,(lte)3 ,(lte)c6 ,(lte)d7 ,(lte)e8
    ,(lte)4 ,(lte)c4 ,(lte)d3 ,(lte)e2 ,(lte)f1
};
static const lte queen_b6[] = {
(lte)8
    ,(lte)1 ,(lte)a6
    ,(lte)6 ,(lte)c6 ,(lte)d6 ,(lte)e6 ,(lte)f6 ,(lte)g6 ,(lte)h6
    ,(lte)5 ,(lte)b5 ,(lte)b4 ,(lte)b3 ,(lte)b2 ,(lte)b1
    ,(lte)2 ,(lte)b7 ,(lte)b8
    ,(lte)1 ,(lte)a5
    ,(lte)1 ,(lte)a7
    ,(lte)2 ,(lte)c7 ,(lte)d8
    ,(lte)5 ,(lte)c5 ,(lte)d4 ,(lte)e3 ,(lte)f2 ,(lte)g1
};
static const lte queen_b7[] = {
(lte)8
    ,(lte)1 ,(lte)a7
    ,(lte)6 ,(lte)c7 ,(lte)d7 ,(lte)e7 ,(lte)f7 ,(lte)g7 ,(lte)h7
    ,(lte)6 ,(lte)b6 ,(lte)b5 ,(lte)b4 ,(lte)b3 ,(lte)b2 ,(lte)b1
    ,(lte)1 ,(lte)b8
    ,(lte)1 ,(lte)a6
    ,(lte)1 ,(lte)a8
    ,(lte)1 ,(lte)c8
    ,(lte)6 ,(lte)c6 ,(lte)d5 ,(lte)e4 ,(lte)f3 ,(lte)g2 ,(lte)h1
};
static const lte queen_b8[] = {
(lte)5
    ,(lte)1 ,(lte)a8
    ,(lte)6 ,(lte)c8 ,(lte)d8 ,(lte)e8 ,(lte)f8 ,(lte)g8 ,(lte)h8
    ,(lte)7 ,(lte)b7 ,(lte)b6 ,(lte)b5 ,(lte)b4 ,(lte)b3 ,(lte)b2 ,(lte)b1
    ,(lte)1 ,(lte)a7
    ,(lte)6 ,(lte)c7 ,(lte)d6 ,(lte)e5 ,(lte)f4 ,(lte)g3 ,(lte)h2
};
static const lte queen_c1[] = {
(lte)5
    ,(lte)2 ,(lte)b1 ,(lte)a1
    ,(lte)5 ,(lte)d1 ,(lte)e1 ,(lte)f1 ,(lte)g1 ,(lte)h1
    ,(lte)7 ,(lte)c2 ,(lte)c3 ,(lte)c4 ,(lte)c5 ,(lte)c6 ,(lte)c7 ,(lte)c8
    ,(lte)2 ,(lte)b2 ,(lte)a3
    ,(lte)5 ,(lte)d2 ,(lte)e3 ,(lte)f4 ,(lte)g5 ,(lte)h6
};
static const lte queen_c2[] = {
(lte)8
    ,(lte)2 ,(lte)b2 ,(lte)a2
    ,(lte)5 ,(lte)d2 ,(lte)e2 ,(lte)f2 ,(lte)g2 ,(lte)h2
    ,(lte)1 ,(lte)c1
    ,(lte)6 ,(lte)c3 ,(lte)c4 ,(lte)c5 ,(lte)c6 ,(lte)c7 ,(lte)c8
    ,(lte)1 ,(lte)b1
    ,(lte)2 ,(lte)b3 ,(lte)a4
    ,(lte)5 ,(lte)d3 ,(lte)e4 ,(lte)f5 ,(lte)g6 ,(lte)h7
    ,(lte)1 ,(lte)d1
};
static const lte queen_c3[] = {
(lte)8
    ,(lte)2 ,(lte)b3 ,(lte)a3
    ,(lte)5 ,(lte)d3 ,(lte)e3 ,(lte)f3 ,(lte)g3 ,(lte)h3
    ,(lte)2 ,(lte)c2 ,(lte)c1
    ,(lte)5 ,(lte)c4 ,(lte)c5 ,(lte)c6 ,(lte)c7 ,(lte)c8
    ,(lte)2 ,(lte)b2 ,(lte)a1
    ,(lte)2 ,(lte)b4 ,(lte)a5
    ,(lte)5 ,(lte)d4 ,(lte)e5 ,(lte)f6 ,(lte)g7 ,(lte)h8
    ,(lte)2 ,(lte)d2 ,(lte)e1
};
static const lte queen_c4[] = {
(lte)8
    ,(lte)2 ,(lte)b4 ,(lte)a4
    ,(lte)5 ,(lte)d4 ,(lte)e4 ,(lte)f4 ,(lte)g4 ,(lte)h4
    ,(lte)3 ,(lte)c3 ,(lte)c2 ,(lte)c1
    ,(lte)4 ,(lte)c5 ,(lte)c6 ,(lte)c7 ,(lte)c8
    ,(lte)2 ,(lte)b3 ,(lte)a2
    ,(lte)2 ,(lte)b5 ,(lte)a6
    ,(lte)4 ,(lte)d5 ,(lte)e6 ,(lte)f7 ,(lte)g8
    ,(lte)3 ,(lte)d3 ,(lte)e2 ,(lte)f1
};
static const lte queen_c5[] = {
(lte)8
    ,(lte)2 ,(lte)b5 ,(lte)a5
    ,(lte)5 ,(lte)d5 ,(lte)e5 ,(lte)f5 ,(lte)g5 ,(lte)h5
    ,(lte)4 ,(lte)c4 ,(lte)c3 ,(lte)c2 ,(lte)c1
    ,(lte)3 ,(lte)c6 ,(lte)c7 ,(lte)c8
    ,(lte)2 ,(lte)b4 ,(lte)a3
    ,(lte)2 ,(lte)b6 ,(lte)a7
    ,(lte)3 ,(lte)d6 ,(lte)e7 ,(lte)f8
    ,(lte)4 ,(lte)d4 ,(lte)e3 ,(lte)f2 ,(lte)g1
};
static const lte queen_c6[] = {
(lte)8
    ,(lte)2 ,(lte)b6 ,(lte)a6
    ,(lte)5 ,(lte)d6 ,(lte)e6 ,(lte)f6 ,(lte)g6 ,(lte)h6
    ,(lte)5 ,(lte)c5 ,(lte)c4 ,(lte)c3 ,(lte)c2 ,(lte)c1
    ,(lte)2 ,(lte)c7 ,(lte)c8
    ,(lte)2 ,(lte)b5 ,(lte)a4
    ,(lte)2 ,(lte)b7 ,(lte)a8
    ,(lte)2 ,(lte)d7 ,(lte)e8
    ,(lte)5 ,(lte)d5 ,(lte)e4 ,(lte)f3 ,(lte)g2 ,(lte)h1
};
static const lte queen_c7[] = {
(lte)8
    ,(lte)2 ,(lte)b7 ,(lte)a7
    ,(lte)5 ,(lte)d7 ,(lte)e7 ,(lte)f7 ,(lte)g7 ,(lte)h7
    ,(lte)6 ,(lte)c6 ,(lte)c5 ,(lte)c4 ,(lte)c3 ,(lte)c2 ,(lte)c1
    ,(lte)1 ,(lte)c8
    ,(lte)2 ,(lte)b6 ,(lte)a5
    ,(lte)1 ,(lte)b8
    ,(lte)1 ,(lte)d8
    ,(lte)5 ,(lte)d6 ,(lte)e5 ,(lte)f4 ,(lte)g3 ,(lte)h2
};
static const lte queen_c8[] = {
(lte)5
    ,(lte)2 ,(lte)b8 ,(lte)a8
    ,(lte)5 ,(lte)d8 ,(lte)e8 ,(lte)f8 ,(lte)g8 ,(lte)h8
    ,(lte)7 ,(lte)c7 ,(lte)c6 ,(lte)c5 ,(lte)c4 ,(lte)c3 ,(lte)c2 ,(lte)c1
    ,(lte)2 ,(lte)b7 ,(lte)a6
    ,(lte)5 ,(lte)d7 ,(lte)e6 ,(lte)f5 ,(lte)g4 ,(lte)h3
};
static const lte queen_d1[] = {
(lte)5
    ,(lte)3 ,(lte)c1 ,(lte)b1 ,(lte)a1
    ,(lte)4 ,(lte)e1 ,(lte)f1 ,(lte)g1 ,(lte)h1
    ,(lte)7 ,(lte)d2 ,(lte)d3 ,(lte)d4 ,(lte)d5 ,(lte)d6 ,(lte)d7 ,(lte)d8
    ,(lte)3 ,(lte)c2 ,(lte)b3 ,(lte)a4
    ,(lte)4 ,(lte)e2 ,(lte)f3 ,(lte)g4 ,(lte)h5
};
static const lte queen_d2[] = {
(lte)8
    ,(lte)3 ,(lte)c2 ,(lte)b2 ,(lte)a2
    ,(lte)4 ,(lte)e2 ,(lte)f2 ,(lte)g2 ,(lte)h2
    ,(lte)1 ,(lte)d1
    ,(lte)6 ,(lte)d3 ,(lte)d4 ,(lte)d5 ,(lte)d6 ,(lte)d7 ,(lte)d8
    ,(lte)1 ,(lte)c1
    ,(lte)3 ,(lte)c3 ,(lte)b4 ,(lte)a5
    ,(lte)4 ,(lte)e3 ,(lte)f4 ,(lte)g5 ,(lte)h6
    ,(lte)1 ,(lte)e1
};
static const lte queen_d3[] = {
(lte)8
    ,(lte)3 ,(lte)c3 ,(lte)b3 ,(lte)a3
    ,(lte)4 ,(lte)e3 ,(lte)f3 ,(lte)g3 ,(lte)h3
    ,(lte)2 ,(lte)d2 ,(lte)d1
    ,(lte)5 ,(lte)d4 ,(lte)d5 ,(lte)d6 ,(lte)d7 ,(lte)d8
    ,(lte)2 ,(lte)c2 ,(lte)b1
    ,(lte)3 ,(lte)c4 ,(lte)b5 ,(lte)a6
    ,(lte)4 ,(lte)e4 ,(lte)f5 ,(lte)g6 ,(lte)h7
    ,(lte)2 ,(lte)e2 ,(lte)f1
};
static const lte queen_d4[] = {
(lte)8
    ,(lte)3 ,(lte)c4 ,(lte)b4 ,(lte)a4
    ,(lte)4 ,(lte)e4 ,(lte)f4 ,(lte)g4 ,(lte)h4
    ,(lte)3 ,(lte)d3 ,(lte)d2 ,(lte)d1
    ,(lte)4 ,(lte)d5 ,(lte)d6 ,(lte)d7 ,(lte)d8
    ,(lte)3 ,(lte)c3 ,(lte)b2 ,(lte)a1
    ,(lte)3 ,(lte)c5 ,(lte)b6 ,(lte)a7
    ,(lte)4 ,(lte)e5 ,(lte)f6 ,(lte)g7 ,(lte)h8
    ,(lte)3 ,(lte)e3 ,(lte)f2 ,(lte)g1
};
static const lte queen_d5[] = {
(lte)8
    ,(lte)3 ,(lte)c5 ,(lte)b5 ,(lte)a5
    ,(lte)4 ,(lte)e5 ,(lte)f5 ,(lte)g5 ,(lte)h5
    ,(lte)4 ,(lte)d4 ,(lte)d3 ,(lte)d2 ,(lte)d1
    ,(lte)3 ,(lte)d6 ,(lte)d7 ,(lte)d8
    ,(lte)3 ,(lte)c4 ,(lte)b3 ,(lte)a2
    ,(lte)3 ,(lte)c6 ,(lte)b7 ,(lte)a8
    ,(lte)3 ,(lte)e6 ,(lte)f7 ,(lte)g8
    ,(lte)4 ,(lte)e4 ,(lte)f3 ,(lte)g2 ,(lte)h1
};
static const lte queen_d6[] = {
(lte)8
    ,(lte)3 ,(lte)c6 ,(lte)b6 ,(lte)a6
    ,(lte)4 ,(lte)e6 ,(lte)f6 ,(lte)g6 ,(lte)h6
    ,(lte)5 ,(lte)d5 ,(lte)d4 ,(lte)d3 ,(lte)d2 ,(lte)d1
    ,(lte)2 ,(lte)d7 ,(lte)d8
    ,(lte)3 ,(lte)c5 ,(lte)b4 ,(lte)a3
    ,(lte)2 ,(lte)c7 ,(lte)b8
    ,(lte)2 ,(lte)e7 ,(lte)f8
    ,(lte)4 ,(lte)e5 ,(lte)f4 ,(lte)g3 ,(lte)h2
};
static const lte queen_d7[] = {
(lte)8
    ,(lte)3 ,(lte)c7 ,(lte)b7 ,(lte)a7
    ,(lte)4 ,(lte)e7 ,(lte)f7 ,(lte)g7 ,(lte)h7
    ,(lte)6 ,(lte)d6 ,(lte)d5 ,(lte)d4 ,(lte)d3 ,(lte)d2 ,(lte)d1
    ,(lte)1 ,(lte)d8
    ,(lte)3 ,(lte)c6 ,(lte)b5 ,(lte)a4
    ,(lte)1 ,(lte)c8
    ,(lte)1 ,(lte)e8
    ,(lte)4 ,(lte)e6 ,(lte)f5 ,(lte)g4 ,(lte)h3
};
static const lte queen_d8[] = {
(lte)5
    ,(lte)3 ,(lte)c8 ,(lte)b8 ,(lte)a8
    ,(lte)4 ,(lte)e8 ,(lte)f8 ,(lte)g8 ,(lte)h8
    ,(lte)7 ,(lte)d7 ,(lte)d6 ,(lte)d5 ,(lte)d4 ,(lte)d3 ,(lte)d2 ,(lte)d1
    ,(lte)3 ,(lte)c7 ,(lte)b6 ,(lte)a5
    ,(lte)4 ,(lte)e7 ,(lte)f6 ,(lte)g5 ,(lte)h4
};
static const lte queen_e1[] = {
(lte)5
    ,(lte)4 ,(lte)d1 ,(lte)c1 ,(lte)b1 ,(lte)a1
    ,(lte)3 ,(lte)f1 ,(lte)g1 ,(lte)h1
    ,(lte)7 ,(lte)e2 ,(lte)e3 ,(lte)e4 ,(lte)e5 ,(lte)e6 ,(lte)e7 ,(lte)e8
    ,(lte)4 ,(lte)d2 ,(lte)c3 ,(lte)b4 ,(lte)a5
    ,(lte)3 ,(lte)f2 ,(lte)g3 ,(lte)h4
};
static const lte queen_e2[] = {
(lte)8
    ,(lte)4 ,(lte)d2 ,(lte)c2 ,(lte)b2 ,(lte)a2
    ,(lte)3 ,(lte)f2 ,(lte)g2 ,(lte)h2
    ,(lte)1 ,(lte)e1
    ,(lte)6 ,(lte)e3 ,(lte)e4 ,(lte)e5 ,(lte)e6 ,(lte)e7 ,(lte)e8
    ,(lte)1 ,(lte)d1
    ,(lte)4 ,(lte)d3 ,(lte)c4 ,(lte)b5 ,(lte)a6
    ,(lte)3 ,(lte)f3 ,(lte)g4 ,(lte)h5
    ,(lte)1 ,(lte)f1
};
static const lte queen_e3[] = {
(lte)8
    ,(lte)4 ,(lte)d3 ,(lte)c3 ,(lte)b3 ,(lte)a3
    ,(lte)3 ,(lte)f3 ,(lte)g3 ,(lte)h3
    ,(lte)2 ,(lte)e2 ,(lte)e1
    ,(lte)5 ,(lte)e4 ,(lte)e5 ,(lte)e6 ,(lte)e7 ,(lte)e8
    ,(lte)2 ,(lte)d2 ,(lte)c1
    ,(lte)4 ,(lte)d4 ,(lte)c5 ,(lte)b6 ,(lte)a7
    ,(lte)3 ,(lte)f4 ,(lte)g5 ,(lte)h6
    ,(lte)2 ,(lte)f2 ,(lte)g1
};
static const lte queen_e4[] = {
(lte)8
    ,(lte)4 ,(lte)d4 ,(lte)c4 ,(lte)b4 ,(lte)a4
    ,(lte)3 ,(lte)f4 ,(lte)g4 ,(lte)h4
    ,(lte)3 ,(lte)e3 ,(lte)e2 ,(lte)e1
    ,(lte)4 ,(lte)e5 ,(lte)e6 ,(lte)e7 ,(lte)e8
    ,(lte)3 ,(lte)d3 ,(lte)c2 ,(lte)b1
    ,(lte)4 ,(lte)d5 ,(lte)c6 ,(lte)b7 ,(lte)a8
    ,(lte)3 ,(lte)f5 ,(lte)g6 ,(lte)h7
    ,(lte)3 ,(lte)f3 ,(lte)g2 ,(lte)h1
};
static const lte queen_e5[] = {
(lte)8
    ,(lte)4 ,(lte)d5 ,(lte)c5 ,(lte)b5 ,(lte)a5
    ,(lte)3 ,(lte)f5 ,(lte)g5 ,(lte)h5
    ,(lte)4 ,(lte)e4 ,(lte)e3 ,(lte)e2 ,(lte)e1
    ,(lte)3 ,(lte)e6 ,(lte)e7 ,(lte)e8
    ,(lte)4 ,(lte)d4 ,(lte)c3 ,(lte)b2 ,(lte)a1
    ,(lte)3 ,(lte)d6 ,(lte)c7 ,(lte)b8
    ,(lte)3 ,(lte)f6 ,(lte)g7 ,(lte)h8
    ,(lte)3 ,(lte)f4 ,(lte)g3 ,(lte)h2
};
static const lte queen_e6[] = {
(lte)8
    ,(lte)4 ,(lte)d6 ,(lte)c6 ,(lte)b6 ,(lte)a6
    ,(lte)3 ,(lte)f6 ,(lte)g6 ,(lte)h6
    ,(lte)5 ,(lte)e5 ,(lte)e4 ,(lte)e3 ,(lte)e2 ,(lte)e1
    ,(lte)2 ,(lte)e7 ,(lte)e8
    ,(lte)4 ,(lte)d5 ,(lte)c4 ,(lte)b3 ,(lte)a2
    ,(lte)2 ,(lte)d7 ,(lte)c8
    ,(lte)2 ,(lte)f7 ,(lte)g8
    ,(lte)3 ,(lte)f5 ,(lte)g4 ,(lte)h3
};
static const lte queen_e7[] = {
(lte)8
    ,(lte)4 ,(lte)d7 ,(lte)c7 ,(lte)b7 ,(lte)a7
    ,(lte)3 ,(lte)f7 ,(lte)g7 ,(lte)h7
    ,(lte)6 ,(lte)e6 ,(lte)e5 ,(lte)e4 ,(lte)e3 ,(lte)e2 ,(lte)e1
    ,(lte)1 ,(lte)e8
    ,(lte)4 ,(lte)d6 ,(lte)c5 ,(lte)b4 ,(lte)a3
    ,(lte)1 ,(lte)d8
    ,(lte)1 ,(lte)f8
    ,(lte)3 ,(lte)f6 ,(lte)g5 ,(lte)h4
};
static const lte queen_e8[] = {
(lte)5
    ,(lte)4 ,(lte)d8 ,(lte)c8 ,(lte)b8 ,(lte)a8
    ,(lte)3 ,(lte)f8 ,(lte)g8 ,(lte)h8
    ,(lte)7 ,(lte)e7 ,(lte)e6 ,(lte)e5 ,(lte)e4 ,(lte)e3 ,(lte)e2 ,(lte)e1
    ,(lte)4 ,(lte)d7 ,(lte)c6 ,(lte)b5 ,(lte)a4
    ,(lte)3 ,(lte)f7 ,(lte)g6 ,(lte)h5
};
static const lte queen_f1[] = {
(lte)5
    ,(lte)5 ,(lte)e1 ,(lte)d1 ,(lte)c1 ,(lte)b1 ,(lte)a1
    ,(lte)2 ,(lte)g1 ,(lte)h1
    ,(lte)7 ,(lte)f2 ,(lte)f3 ,(lte)f4 ,(lte)f5 ,(lte)f6 ,(lte)f7 ,(lte)f8
    ,(lte)5 ,(lte)e2 ,(lte)d3 ,(lte)c4 ,(lte)b5 ,(lte)a6
    ,(lte)2 ,(lte)g2 ,(lte)h3
};
static const lte queen_f2[] = {
(lte)8
    ,(lte)5 ,(lte)e2 ,(lte)d2 ,(lte)c2 ,(lte)b2 ,(lte)a2
    ,(lte)2 ,(lte)g2 ,(lte)h2
    ,(lte)1 ,(lte)f1
    ,(lte)6 ,(lte)f3 ,(lte)f4 ,(lte)f5 ,(lte)f6 ,(lte)f7 ,(lte)f8
    ,(lte)1 ,(lte)e1
    ,(lte)5 ,(lte)e3 ,(lte)d4 ,(lte)c5 ,(lte)b6 ,(lte)a7
    ,(lte)2 ,(lte)g3 ,(lte)h4
    ,(lte)1 ,(lte)g1
};
static const lte queen_f3[] = {
(lte)8
    ,(lte)5 ,(lte)e3 ,(lte)d3 ,(lte)c3 ,(lte)b3 ,(lte)a3
    ,(lte)2 ,(lte)g3 ,(lte)h3
    ,(lte)2 ,(lte)f2 ,(lte)f1
    ,(lte)5 ,(lte)f4 ,(lte)f5 ,(lte)f6 ,(lte)f7 ,(lte)f8
    ,(lte)2 ,(lte)e2 ,(lte)d1
    ,(lte)5 ,(lte)e4 ,(lte)d5 ,(lte)c6 ,(lte)b7 ,(lte)a8
    ,(lte)2 ,(lte)g4 ,(lte)h5
    ,(lte)2 ,(lte)g2 ,(lte)h1
};
static const lte queen_f4[] = {
(lte)8
    ,(lte)5 ,(lte)e4 ,(lte)d4 ,(lte)c4 ,(lte)b4 ,(lte)a4
    ,(lte)2 ,(lte)g4 ,(lte)h4
    ,(lte)3 ,(lte)f3 ,(lte)f2 ,(lte)f1
    ,(lte)4 ,(lte)f5 ,(lte)f6 ,(lte)f7 ,(lte)f8
    ,(lte)3 ,(lte)e3 ,(lte)d2 ,(lte)c1
    ,(lte)4 ,(lte)e5 ,(lte)d6 ,(lte)c7 ,(lte)b8
    ,(lte)2 ,(lte)g5 ,(lte)h6
    ,(lte)2 ,(lte)g3 ,(lte)h2
};
static const lte queen_f5[] = {
(lte)8
    ,(lte)5 ,(lte)e5 ,(lte)d5 ,(lte)c5 ,(lte)b5 ,(lte)a5
    ,(lte)2 ,(lte)g5 ,(lte)h5
    ,(lte)4 ,(lte)f4 ,(lte)f3 ,(lte)f2 ,(lte)f1
    ,(lte)3 ,(lte)f6 ,(lte)f7 ,(lte)f8
    ,(lte)4 ,(lte)e4 ,(lte)d3 ,(lte)c2 ,(lte)b1
    ,(lte)3 ,(lte)e6 ,(lte)d7 ,(lte)c8
    ,(lte)2 ,(lte)g6 ,(lte)h7
    ,(lte)2 ,(lte)g4 ,(lte)h3
};
static const lte queen_f6[] = {
(lte)8
    ,(lte)5 ,(lte)e6 ,(lte)d6 ,(lte)c6 ,(lte)b6 ,(lte)a6
    ,(lte)2 ,(lte)g6 ,(lte)h6
    ,(lte)5 ,(lte)f5 ,(lte)f4 ,(lte)f3 ,(lte)f2 ,(lte)f1
    ,(lte)2 ,(lte)f7 ,(lte)f8
    ,(lte)5 ,(lte)e5 ,(lte)d4 ,(lte)c3 ,(lte)b2 ,(lte)a1
    ,(lte)2 ,(lte)e7 ,(lte)d8
    ,(lte)2 ,(lte)g7 ,(lte)h8
    ,(lte)2 ,(lte)g5 ,(lte)h4
};
static const lte queen_f7[] = {
(lte)8
    ,(lte)5 ,(lte)e7 ,(lte)d7 ,(lte)c7 ,(lte)b7 ,(lte)a7
    ,(lte)2 ,(lte)g7 ,(lte)h7
    ,(lte)6 ,(lte)f6 ,(lte)f5 ,(lte)f4 ,(lte)f3 ,(lte)f2 ,(lte)f1
    ,(lte)1 ,(lte)f8
    ,(lte)5 ,(lte)e6 ,(lte)d5 ,(lte)c4 ,(lte)b3 ,(lte)a2
    ,(lte)1 ,(lte)e8
    ,(lte)1 ,(lte)g8
    ,(lte)2 ,(lte)g6 ,(lte)h5
};
static const lte queen_f8[] = {
(lte)5
    ,(lte)5 ,(lte)e8 ,(lte)d8 ,(lte)c8 ,(lte)b8 ,(lte)a8
    ,(lte)2 ,(lte)g8 ,(lte)h8
    ,(lte)7 ,(lte)f7 ,(lte)f6 ,(lte)f5 ,(lte)f4 ,(lte)f3 ,(lte)f2 ,(lte)f1
    ,(lte)5 ,(lte)e7 ,(lte)d6 ,(lte)c5 ,(lte)b4 ,(lte)a3
    ,(lte)2 ,(lte)g7 ,(lte)h6
};
static const lte queen_g1[] = {
(lte)5
    ,(lte)6 ,(lte)f1 ,(lte)e1 ,(lte)d1 ,(lte)c1 ,(lte)b1 ,(lte)a1
    ,(lte)1 ,(lte)h1
    ,(lte)7 ,(lte)g2 ,(lte)g3 ,(lte)g4 ,(lte)g5 ,(lte)g6 ,(lte)g7 ,(lte)g8
    ,(lte)6 ,(lte)f2 ,(lte)e3 ,(lte)d4 ,(lte)c5 ,(lte)b6 ,(lte)a7
    ,(lte)1 ,(lte)h2
};
static const lte queen_g2[] = {
(lte)8
    ,(lte)6 ,(lte)f2 ,(lte)e2 ,(lte)d2 ,(lte)c2 ,(lte)b2 ,(lte)a2
    ,(lte)1 ,(lte)h2
    ,(lte)1 ,(lte)g1
    ,(lte)6 ,(lte)g3 ,(lte)g4 ,(lte)g5 ,(lte)g6 ,(lte)g7 ,(lte)g8
    ,(lte)1 ,(lte)f1
    ,(lte)6 ,(lte)f3 ,(lte)e4 ,(lte)d5 ,(lte)c6 ,(lte)b7 ,(lte)a8
    ,(lte)1 ,(lte)h3
    ,(lte)1 ,(lte)h1
};
static const lte queen_g3[] = {
(lte)8
    ,(lte)6 ,(lte)f3 ,(lte)e3 ,(lte)d3 ,(lte)c3 ,(lte)b3 ,(lte)a3
    ,(lte)1 ,(lte)h3
    ,(lte)2 ,(lte)g2 ,(lte)g1
    ,(lte)5 ,(lte)g4 ,(lte)g5 ,(lte)g6 ,(lte)g7 ,(lte)g8
    ,(lte)2 ,(lte)f2 ,(lte)e1
    ,(lte)5 ,(lte)f4 ,(lte)e5 ,(lte)d6 ,(lte)c7 ,(lte)b8
    ,(lte)1 ,(lte)h4
    ,(lte)1 ,(lte)h2
};
static const lte queen_g4[] = {
(lte)8
    ,(lte)6 ,(lte)f4 ,(lte)e4 ,(lte)d4 ,(lte)c4 ,(lte)b4 ,(lte)a4
    ,(lte)1 ,(lte)h4
    ,(lte)3 ,(lte)g3 ,(lte)g2 ,(lte)g1
    ,(lte)4 ,(lte)g5 ,(lte)g6 ,(lte)g7 ,(lte)g8
    ,(lte)3 ,(lte)f3 ,(lte)e2 ,(lte)d1
    ,(lte)4 ,(lte)f5 ,(lte)e6 ,(lte)d7 ,(lte)c8
    ,(lte)1 ,(lte)h5
    ,(lte)1 ,(lte)h3
};
static const lte queen_g5[] = {
(lte)8
    ,(lte)6 ,(lte)f5 ,(lte)e5 ,(lte)d5 ,(lte)c5 ,(lte)b5 ,(lte)a5
    ,(lte)1 ,(lte)h5
    ,(lte)4 ,(lte)g4 ,(lte)g3 ,(lte)g2 ,(lte)g1
    ,(lte)3 ,(lte)g6 ,(lte)g7 ,(lte)g8
    ,(lte)4 ,(lte)f4 ,(lte)e3 ,(lte)d2 ,(lte)c1
    ,(lte)3 ,(lte)f6 ,(lte)e7 ,(lte)d8
    ,(lte)1 ,(lte)h6
    ,(lte)1 ,(lte)h4
};
static const lte queen_g6[] = {
(lte)8
    ,(lte)6 ,(lte)f6 ,(lte)e6 ,(lte)d6 ,(lte)c6 ,(lte)b6 ,(lte)a6
    ,(lte)1 ,(lte)h6
    ,(lte)5 ,(lte)g5 ,(lte)g4 ,(lte)g3 ,(lte)g2 ,(lte)g1
    ,(lte)2 ,(lte)g7 ,(lte)g8
    ,(lte)5 ,(lte)f5 ,(lte)e4 ,(lte)d3 ,(lte)c2 ,(lte)b1
    ,(lte)2 ,(lte)f7 ,(lte)e8
    ,(lte)1 ,(lte)h7
    ,(lte)1 ,(lte)h5
};
static const lte queen_g7[] = {
(lte)8
    ,(lte)6 ,(lte)f7 ,(lte)e7 ,(lte)d7 ,(lte)c7 ,(lte)b7 ,(lte)a7
    ,(lte)1 ,(lte)h7
    ,(lte)6 ,(lte)g6 ,(lte)g5 ,(lte)g4 ,(lte)g3 ,(lte)g2 ,(lte)g1
    ,(lte)1 ,(lte)g8
    ,(lte)6 ,(lte)f6 ,(lte)e5 ,(lte)d4 ,(lte)c3 ,(lte)b2 ,(lte)a1
    ,(lte)1 ,(lte)f8
    ,(lte)1 ,(lte)h8
    ,(lte)1 ,(lte)h6
};
static const lte queen_g8[] = {
(lte)5
    ,(lte)6 ,(lte)f8 ,(lte)e8 ,(lte)d8 ,(lte)c8 ,(lte)b8 ,(lte)a8
    ,(lte)1 ,(lte)h8
    ,(lte)7 ,(lte)g7 ,(lte)g6 ,(lte)g5 ,(lte)g4 ,(lte)g3 ,(lte)g2 ,(lte)g1
    ,(lte)6 ,(lte)f7 ,(lte)e6 ,(lte)d5 ,(lte)c4 ,(lte)b3 ,(lte)a2
    ,(lte)1 ,(lte)h7
};
static const lte queen_h1[] = {
(lte)3
    ,(lte)7 ,(lte)g1 ,(lte)f1 ,(lte)e1 ,(lte)d1 ,(lte)c1 ,(lte)b1 ,(lte)a1
    ,(lte)7 ,(lte)h2 ,(lte)h3 ,(lte)h4 ,(lte)h5 ,(lte)h6 ,(lte)h7 ,(lte)h8
    ,(lte)7 ,(lte)g2 ,(lte)f3 ,(lte)e4 ,(lte)d5 ,(lte)c6 ,(lte)b7 ,(lte)a8
};
static const lte queen_h2[] = {
(lte)5
    ,(lte)7 ,(lte)g2 ,(lte)f2 ,(lte)e2 ,(lte)d2 ,(lte)c2 ,(lte)b2 ,(lte)a2
    ,(lte)1 ,(lte)h1
    ,(lte)6 ,(lte)h3 ,(lte)h4 ,(lte)h5 ,(lte)h6 ,(lte)h7 ,(lte)h8
    ,(lte)1 ,(lte)g1
    ,(lte)6 ,(lte)g3 ,(lte)f4 ,(lte)e5 ,(lte)d6 ,(lte)c7 ,(lte)b8
};
static const lte queen_h3[] = {
(lte)5
    ,(lte)7 ,(lte)g3 ,(lte)f3 ,(lte)e3 ,(lte)d3 ,(lte)c3 ,(lte)b3 ,(lte)a3
    ,(lte)2 ,(lte)h2 ,(lte)h1
    ,(lte)5 ,(lte)h4 ,(lte)h5 ,(lte)h6 ,(lte)h7 ,(lte)h8
    ,(lte)2 ,(lte)g2 ,(lte)f1
    ,(lte)5 ,(lte)g4 ,(lte)f5 ,(lte)e6 ,(lte)d7 ,(lte)c8
};
static const lte queen_h4[] = {
(lte)5
    ,(lte)7 ,(lte)g4 ,(lte)f4 ,(lte)e4 ,(lte)d4 ,(lte)c4 ,(lte)b4 ,(lte)a4
    ,(lte)3 ,(lte)h3 ,(lte)h2 ,(lte)h1
    ,(lte)4 ,(lte)h5 ,(lte)h6 ,(lte)h7 ,(lte)h8
    ,(lte)3 ,(lte)g3 ,(lte)f2 ,(lte)e1
    ,(lte)4 ,(lte)g5 ,(lte)f6 ,(lte)e7 ,(lte)d8
};
static const lte queen_h5[] = {
(lte)5
    ,(lte)7 ,(lte)g5 ,(lte)f5 ,(lte)e5 ,(lte)d5 ,(lte)c5 ,(lte)b5 ,(lte)a5
    ,(lte)4 ,(lte)h4 ,(lte)h3 ,(lte)h2 ,(lte)h1
    ,(lte)3 ,(lte)h6 ,(lte)h7 ,(lte)h8
    ,(lte)4 ,(lte)g4 ,(lte)f3 ,(lte)e2 ,(lte)d1
    ,(lte)3 ,(lte)g6 ,(lte)f7 ,(lte)e8
};
static const lte queen_h6[] = {
(lte)5
    ,(lte)7 ,(lte)g6 ,(lte)f6 ,(lte)e6 ,(lte)d6 ,(lte)c6 ,(lte)b6 ,(lte)a6
    ,(lte)5 ,(lte)h5 ,(lte)h4 ,(lte)h3 ,(lte)h2 ,(lte)h1
    ,(lte)2 ,(lte)h7 ,(lte)h8
    ,(lte)5 ,(lte)g5 ,(lte)f4 ,(lte)e3 ,(lte)d2 ,(lte)c1
    ,(lte)2 ,(lte)g7 ,(lte)f8
};
static const lte queen_h7[] = {
(lte)5
    ,(lte)7 ,(lte)g7 ,(lte)f7 ,(lte)e7 ,(lte)d7 ,(lte)c7 ,(lte)b7 ,(lte)a7
    ,(lte)6 ,(lte)h6 ,(lte)h5 ,(lte)h4 ,(lte)h3 ,(lte)h2 ,(lte)h1
    ,(lte)1 ,(lte)h8
    ,(lte)6 ,(lte)g6 ,(lte)f5 ,(lte)e4 ,(lte)d3 ,(lte)c2 ,(lte)b1
    ,(lte)1 ,(lte)g8
};
static const lte queen_h8[] = {
(lte)3
    ,(lte)7 ,(lte)g8 ,(lte)f8 ,(lte)e8 ,(lte)d8 ,(lte)c8 ,(lte)b8 ,(lte)a8
    ,(lte)7 ,(lte)h7 ,(lte)h6 ,(lte)h5 ,(lte)h4 ,(lte)h3 ,(lte)h2 ,(lte)h1
    ,(lte)7 ,(lte)g7 ,(lte)f6 ,(lte)e5 ,(lte)d4 ,(lte)c3 ,(lte)b2 ,(lte)a1
};

// queen_lookup
const lte *queen_lookup[] = {
    queen_a8, queen_b8, queen_c8, queen_d8, queen_e8, queen_f8, queen_g8, queen_h8,
    queen_a7, queen_b7, queen_c7, queen_d7, queen_e7, queen_f7, queen_g7, queen_h7,
    queen_a6, queen_b6, queen_c6, queen_d6, queen_e6, queen_f6, queen_g6, queen_h6,
    queen_a5, queen_b5, queen_c5, queen_d5, queen_e5, queen_f5, queen_g5, queen_h5,
    queen_a4, queen_b4, queen_c4, queen_d4, queen_e4, queen_f4, queen_g4, queen_h4,
    queen_a3, queen_b3, queen_c3, queen_d3, queen_e3, queen_f3, queen_g3, queen_h3,
    queen_a2, queen_b2, queen_c2, queen_d2, queen_e2, queen_f2, queen_g2, queen_h2,
    queen_a1, queen_b1, queen_c1, queen_d1, queen_e1, queen_f1, queen_g1, queen_h1,
};

// Rook, up to 4 rays
static const lte rook_a1[] = {
(lte)2
    ,(lte)7 ,(lte)b1 ,(lte)c1 ,(lte)d1 ,(lte)e1 ,(lte)f1 ,(lte)g1 ,(lte)h1
    ,(lte)7 ,(lte)a2 ,(lte)a3 ,(lte)a4 ,(lte)a5 ,(lte)a6 ,(lte)a7 ,(lte)a8
};
static const lte rook_a2[] = {
(lte)3
    ,(lte)7 ,(lte)b2 ,(lte)c2 ,(lte)d2 ,(lte)e2 ,(lte)f2 ,(lte)g2 ,(lte)h2
    ,(lte)1 ,(lte)a1
    ,(lte)6 ,(lte)a3 ,(lte)a4 ,(lte)a5 ,(lte)a6 ,(lte)a7 ,(lte)a8
};
static const lte rook_a3[] = {
(lte)3
    ,(lte)7 ,(lte)b3 ,(lte)c3 ,(lte)d3 ,(lte)e3 ,(lte)f3 ,(lte)g3 ,(lte)h3
    ,(lte)2 ,(lte)a2 ,(lte)a1
    ,(lte)5 ,(lte)a4 ,(lte)a5 ,(lte)a6 ,(lte)a7 ,(lte)a8
};
static const lte rook_a4[] = {
(lte)3
    ,(lte)7 ,(lte)b4 ,(lte)c4 ,(lte)d4 ,(lte)e4 ,(lte)f4 ,(lte)g4 ,(lte)h4
    ,(lte)3 ,(lte)a3 ,(lte)a2 ,(lte)a1
    ,(lte)4 ,(lte)a5 ,(lte)a6 ,(lte)a7 ,(lte)a8
};
static const lte rook_a5[] = {
(lte)3
    ,(lte)7 ,(lte)b5 ,(lte)c5 ,(lte)d5 ,(lte)e5 ,(lte)f5 ,(lte)g5 ,(lte)h5
    ,(lte)4 ,(lte)a4 ,(lte)a3 ,(lte)a2 ,(lte)a1
    ,(lte)3 ,(lte)a6 ,(lte)a7 ,(lte)a8
};
static const lte rook_a6[] = {
(lte)3
    ,(lte)7 ,(lte)b6 ,(lte)c6 ,(lte)d6 ,(lte)e6 ,(lte)f6 ,(lte)g6 ,(lte)h6
    ,(lte)5 ,(lte)a5 ,(lte)a4 ,(lte)a3 ,(lte)a2 ,(lte)a1
    ,(lte)2 ,(lte)a7 ,(lte)a8
};
static const lte rook_a7[] = {
(lte)3
    ,(lte)7 ,(lte)b7 ,(lte)c7 ,(lte)d7 ,(lte)e7 ,(lte)f7 ,(lte)g7 ,(lte)h7
    ,(lte)6 ,(lte)a6 ,(lte)a5 ,(lte)a4 ,(lte)a3 ,(lte)a2 ,(lte)a1
    ,(lte)1 ,(lte)a8
};
static const lte rook_a8[] = {
(lte)2
    ,(lte)7 ,(lte)b8 ,(lte)c8 ,(lte)d8 ,(lte)e8 ,(lte)f8 ,(lte)g8 ,(lte)h8
    ,(lte)7 ,(lte)a7 ,(lte)a6 ,(lte)a5 ,(lte)a4 ,(lte)a3 ,(lte)a2 ,(lte)a1
};
static const lte rook_b1[] = {
(lte)3
    ,(lte)1 ,(lte)a1
    ,(lte)6 ,(lte)c1 ,(lte)d1 ,(lte)e1 ,(lte)f1 ,(lte)g1 ,(lte)h1
    ,(lte)7 ,(lte)b2 ,(lte)b3 ,(lte)b4 ,(lte)b5 ,(lte)b6 ,(lte)b7 ,(lte)b8
};
static const lte rook_b2[] = {
(lte)4
    ,(lte)1 ,(lte)a2
    ,(lte)6 ,(lte)c2 ,(lte)d2 ,(lte)e2 ,(lte)f2 ,(lte)g2 ,(lte)h2
    ,(lte)1 ,(lte)b1
    ,(lte)6 ,(lte)b3 ,(lte)b4 ,(lte)b5 ,(lte)b6 ,(lte)b7 ,(lte)b8
};
static const lte rook_b3[] = {
(lte)4
    ,(lte)1 ,(lte)a3
    ,(lte)6 ,(lte)c3 ,(lte)d3 ,(lte)e3 ,(lte)f3 ,(lte)g3 ,(lte)h3
    ,(lte)2 ,(lte)b2 ,(lte)b1
    ,(lte)5 ,(lte)b4 ,(lte)b5 ,(lte)b6 ,(lte)b7 ,(lte)b8
};
static const lte rook_b4[] = {
(lte)4
    ,(lte)1 ,(lte)a4
    ,(lte)6 ,(lte)c4 ,(lte)d4 ,(lte)e4 ,(lte)f4 ,(lte)g4 ,(lte)h4
    ,(lte)3 ,(lte)b3 ,(lte)b2 ,(lte)b1
    ,(lte)4 ,(lte)b5 ,(lte)b6 ,(lte)b7 ,(lte)b8
};
static const lte rook_b5[] = {
(lte)4
    ,(lte)1 ,(lte)a5
    ,(lte)6 ,(lte)c5 ,(lte)d5 ,(lte)e5 ,(lte)f5 ,(lte)g5 ,(lte)h5
    ,(lte)4 ,(lte)b4 ,(lte)b3 ,(lte)b2 ,(lte)b1
    ,(lte)3 ,(lte)b6 ,(lte)b7 ,(lte)b8
};
static const lte rook_b6[] = {
(lte)4
    ,(lte)1 ,(lte)a6
    ,(lte)6 ,(lte)c6 ,(lte)d6 ,(lte)e6 ,(lte)f6 ,(lte)g6 ,(lte)h6
    ,(lte)5 ,(lte)b5 ,(lte)b4 ,(lte)b3 ,(lte)b2 ,(lte)b1
    ,(lte)2 ,(lte)b7 ,(lte)b8
};
static const lte rook_b7[] = {
(lte)4
    ,(lte)1 ,(lte)a7
    ,(lte)6 ,(lte)c7 ,(lte)d7 ,(lte)e7 ,(lte)f7 ,(lte)g7 ,(lte)h7
    ,(lte)6 ,(lte)b6 ,(lte)b5 ,(lte)b4 ,(lte)b3 ,(lte)b2 ,(lte)b1
    ,(lte)1 ,(lte)b8
};
static const lte rook_b8[] = {
(lte)3
    ,(lte)1 ,(lte)a8
    ,(lte)6 ,(lte)c8 ,(lte)d8 ,(lte)e8 ,(lte)f8 ,(lte)g8 ,(lte)h8
    ,(lte)7 ,(lte)b7 ,(lte)b6 ,(lte)b5 ,(lte)b4 ,(lte)b3 ,(lte)b2 ,(lte)b1
};
static const lte rook_c1[] = {
(lte)3
    ,(lte)2 ,(lte)b1 ,(lte)a1
    ,(lte)5 ,(lte)d1 ,(lte)e1 ,(lte)f1 ,(lte)g1 ,(lte)h1
    ,(lte)7 ,(lte)c2 ,(lte)c3 ,(lte)c4 ,(lte)c5 ,(lte)c6 ,(lte)c7 ,(lte)c8
};
static const lte rook_c2[] = {
(lte)4
    ,(lte)2 ,(lte)b2 ,(lte)a2
    ,(lte)5 ,(lte)d2 ,(lte)e2 ,(lte)f2 ,(lte)g2 ,(lte)h2
    ,(lte)1 ,(lte)c1
    ,(lte)6 ,(lte)c3 ,(lte)c4 ,(lte)c5 ,(lte)c6 ,(lte)c7 ,(lte)c8
};
static const lte rook_c3[] = {
(lte)4
    ,(lte)2 ,(lte)b3 ,(lte)a3
    ,(lte)5 ,(lte)d3 ,(lte)e3 ,(lte)f3 ,(lte)g3 ,(lte)h3
    ,(lte)2 ,(lte)c2 ,(lte)c1
    ,(lte)5 ,(lte)c4 ,(lte)c5 ,(lte)c6 ,(lte)c7 ,(lte)c8
};
static const lte rook_c4[] = {
(lte)4
    ,(lte)2 ,(lte)b4 ,(lte)a4
    ,(lte)5 ,(lte)d4 ,(lte)e4 ,(lte)f4 ,(lte)g4 ,(lte)h4
    ,(lte)3 ,(lte)c3 ,(lte)c2 ,(lte)c1
    ,(lte)4 ,(lte)c5 ,(lte)c6 ,(lte)c7 ,(lte)c8
};
static const lte rook_c5[] = {
(lte)4
    ,(lte)2 ,(lte)b5 ,(lte)a5
    ,(lte)5 ,(lte)d5 ,(lte)e5 ,(lte)f5 ,(lte)g5 ,(lte)h5
    ,(lte)4 ,(lte)c4 ,(lte)c3 ,(lte)c2 ,(lte)c1
    ,(lte)3 ,(lte)c6 ,(lte)c7 ,(lte)c8
};
static const lte rook_c6[] = {
(lte)4
    ,(lte)2 ,(lte)b6 ,(lte)a6
    ,(lte)5 ,(lte)d6 ,(lte)e6 ,(lte)f6 ,(lte)g6 ,(lte)h6
    ,(lte)5 ,(lte)c5 ,(lte)c4 ,(lte)c3 ,(lte)c2 ,(lte)c1
    ,(lte)2 ,(lte)c7 ,(lte)c8
};
static const lte rook_c7[] = {
(lte)4
    ,(lte)2 ,(lte)b7 ,(lte)a7
    ,(lte)5 ,(lte)d7 ,(lte)e7 ,(lte)f7 ,(lte)g7 ,(lte)h7
    ,(lte)6 ,(lte)c6 ,(lte)c5 ,(lte)c4 ,(lte)c3 ,(lte)c2 ,(lte)c1
    ,(lte)1 ,(lte)c8
};
static const lte rook_c8[] = {
(lte)3
    ,(lte)2 ,(lte)b8 ,(lte)a8
    ,(lte)5 ,(lte)d8 ,(lte)e8 ,(lte)f8 ,(lte)g8 ,(lte)h8
    ,(lte)7 ,(lte)c7 ,(lte)c6 ,(lte)c5 ,(lte)c4 ,(lte)c3 ,(lte)c2 ,(lte)c1
};
static const lte rook_d1[] = {
(lte)3
    ,(lte)3 ,(lte)c1 ,(lte)b1 ,(lte)a1
    ,(lte)4 ,(lte)e1 ,(lte)f1 ,(lte)g1 ,(lte)h1
    ,(lte)7 ,(lte)d2 ,(lte)d3 ,(lte)d4 ,(lte)d5 ,(lte)d6 ,(lte)d7 ,(lte)d8
};
static const lte rook_d2[] = {
(lte)4
    ,(lte)3 ,(lte)c2 ,(lte)b2 ,(lte)a2
    ,(lte)4 ,(lte)e2 ,(lte)f2 ,(lte)g2 ,(lte)h2
    ,(lte)1 ,(lte)d1
    ,(lte)6 ,(lte)d3 ,(lte)d4 ,(lte)d5 ,(lte)d6 ,(lte)d7 ,(lte)d8
};
static const lte rook_d3[] = {
(lte)4
    ,(lte)3 ,(lte)c3 ,(lte)b3 ,(lte)a3
    ,(lte)4 ,(lte)e3 ,(lte)f3 ,(lte)g3 ,(lte)h3
    ,(lte)2 ,(lte)d2 ,(lte)d1
    ,(lte)5 ,(lte)d4 ,(lte)d5 ,(lte)d6 ,(lte)d7 ,(lte)d8
};
static const lte rook_d4[] = {
(lte)4
    ,(lte)3 ,(lte)c4 ,(lte)b4 ,(lte)a4
    ,(lte)4 ,(lte)e4 ,(lte)f4 ,(lte)g4 ,(lte)h4
    ,(lte)3 ,(lte)d3 ,(lte)d2 ,(lte)d1
    ,(lte)4 ,(lte)d5 ,(lte)d6 ,(lte)d7 ,(lte)d8
};
static const lte rook_d5[] = {
(lte)4
    ,(lte)3 ,(lte)c5 ,(lte)b5 ,(lte)a5
    ,(lte)4 ,(lte)e5 ,(lte)f5 ,(lte)g5 ,(lte)h5
    ,(lte)4 ,(lte)d4 ,(lte)d3 ,(lte)d2 ,(lte)d1
    ,(lte)3 ,(lte)d6 ,(lte)d7 ,(lte)d8
};
static const lte rook_d6[] = {
(lte)4
    ,(lte)3 ,(lte)c6 ,(lte)b6 ,(lte)a6
    ,(lte)4 ,(lte)e6 ,(lte)f6 ,(lte)g6 ,(lte)h6
    ,(lte)5 ,(lte)d5 ,(lte)d4 ,(lte)d3 ,(lte)d2 ,(lte)d1
    ,(lte)2 ,(lte)d7 ,(lte)d8
};
static const lte rook_d7[] = {
(lte)4
    ,(lte)3 ,(lte)c7 ,(lte)b7 ,(lte)a7
    ,(lte)4 ,(lte)e7 ,(lte)f7 ,(lte)g7 ,(lte)h7
    ,(lte)6 ,(lte)d6 ,(lte)d5 ,(lte)d4 ,(lte)d3 ,(lte)d2 ,(lte)d1
    ,(lte)1 ,(lte)d8
};
static const lte rook_d8[] = {
(lte)3
    ,(lte)3 ,(lte)c8 ,(lte)b8 ,(lte)a8
    ,(lte)4 ,(lte)e8 ,(lte)f8 ,(lte)g8 ,(lte)h8
    ,(lte)7 ,(lte)d7 ,(lte)d6 ,(lte)d5 ,(lte)d4 ,(lte)d3 ,(lte)d2 ,(lte)d1
};
static const lte rook_e1[] = {
(lte)3
    ,(lte)4 ,(lte)d1 ,(lte)c1 ,(lte)b1 ,(lte)a1
    ,(lte)3 ,(lte)f1 ,(lte)g1 ,(lte)h1
    ,(lte)7 ,(lte)e2 ,(lte)e3 ,(lte)e4 ,(lte)e5 ,(lte)e6 ,(lte)e7 ,(lte)e8
};
static const lte rook_e2[] = {
(lte)4
    ,(lte)4 ,(lte)d2 ,(lte)c2 ,(lte)b2 ,(lte)a2
    ,(lte)3 ,(lte)f2 ,(lte)g2 ,(lte)h2
    ,(lte)1 ,(lte)e1
    ,(lte)6 ,(lte)e3 ,(lte)e4 ,(lte)e5 ,(lte)e6 ,(lte)e7 ,(lte)e8
};
static const lte rook_e3[] = {
(lte)4
    ,(lte)4 ,(lte)d3 ,(lte)c3 ,(lte)b3 ,(lte)a3
    ,(lte)3 ,(lte)f3 ,(lte)g3 ,(lte)h3
    ,(lte)2 ,(lte)e2 ,(lte)e1
    ,(lte)5 ,(lte)e4 ,(lte)e5 ,(lte)e6 ,(lte)e7 ,(lte)e8
};
static const lte rook_e4[] = {
(lte)4
    ,(lte)4 ,(lte)d4 ,(lte)c4 ,(lte)b4 ,(lte)a4
    ,(lte)3 ,(lte)f4 ,(lte)g4 ,(lte)h4
    ,(lte)3 ,(lte)e3 ,(lte)e2 ,(lte)e1
    ,(lte)4 ,(lte)e5 ,(lte)e6 ,(lte)e7 ,(lte)e8
};
static const lte rook_e5[] = {
(lte)4
    ,(lte)4 ,(lte)d5 ,(lte)c5 ,(lte)b5 ,(lte)a5
    ,(lte)3 ,(lte)f5 ,(lte)g5 ,(lte)h5
    ,(lte)4 ,(lte)e4 ,(lte)e3 ,(lte)e2 ,(lte)e1
    ,(lte)3 ,(lte)e6 ,(lte)e7 ,(lte)e8
};
static const lte rook_e6[] = {
(lte)4
    ,(lte)4 ,(lte)d6 ,(lte)c6 ,(lte)b6 ,(lte)a6
    ,(lte)3 ,(lte)f6 ,(lte)g6 ,(lte)h6
    ,(lte)5 ,(lte)e5 ,(lte)e4 ,(lte)e3 ,(lte)e2 ,(lte)e1
    ,(lte)2 ,(lte)e7 ,(lte)e8
};
static const lte rook_e7[] = {
(lte)4
    ,(lte)4 ,(lte)d7 ,(lte)c7 ,(lte)b7 ,(lte)a7
    ,(lte)3 ,(lte)f7 ,(lte)g7 ,(lte)h7
    ,(lte)6 ,(lte)e6 ,(lte)e5 ,(lte)e4 ,(lte)e3 ,(lte)e2 ,(lte)e1
    ,(lte)1 ,(lte)e8
};
static const lte rook_e8[] = {
(lte)3
    ,(lte)4 ,(lte)d8 ,(lte)c8 ,(lte)b8 ,(lte)a8
    ,(lte)3 ,(lte)f8 ,(lte)g8 ,(lte)h8
    ,(lte)7 ,(lte)e7 ,(lte)e6 ,(lte)e5 ,(lte)e4 ,(lte)e3 ,(lte)e2 ,(lte)e1
};
static const lte rook_f1[] = {
(lte)3
    ,(lte)5 ,(lte)e1 ,(lte)d1 ,(lte)c1 ,(lte)b1 ,(lte)a1
    ,(lte)2 ,(lte)g1 ,(lte)h1
    ,(lte)7 ,(lte)f2 ,(lte)f3 ,(lte)f4 ,(lte)f5 ,(lte)f6 ,(lte)f7 ,(lte)f8
};
static const lte rook_f2[] = {
(lte)4
    ,(lte)5 ,(lte)e2 ,(lte)d2 ,(lte)c2 ,(lte)b2 ,(lte)a2
    ,(lte)2 ,(lte)g2 ,(lte)h2
    ,(lte)1 ,(lte)f1
    ,(lte)6 ,(lte)f3 ,(lte)f4 ,(lte)f5 ,(lte)f6 ,(lte)f7 ,(lte)f8
};
static const lte rook_f3[] = {
(lte)4
    ,(lte)5 ,(lte)e3 ,(lte)d3 ,(lte)c3 ,(lte)b3 ,(lte)a3
    ,(lte)2 ,(lte)g3 ,(lte)h3
    ,(lte)2 ,(lte)f2 ,(lte)f1
    ,(lte)5 ,(lte)f4 ,(lte)f5 ,(lte)f6 ,(lte)f7 ,(lte)f8
};
static const lte rook_f4[] = {
(lte)4
    ,(lte)5 ,(lte)e4 ,(lte)d4 ,(lte)c4 ,(lte)b4 ,(lte)a4
    ,(lte)2 ,(lte)g4 ,(lte)h4
    ,(lte)3 ,(lte)f3 ,(lte)f2 ,(lte)f1
    ,(lte)4 ,(lte)f5 ,(lte)f6 ,(lte)f7 ,(lte)f8
};
static const lte rook_f5[] = {
(lte)4
    ,(lte)5 ,(lte)e5 ,(lte)d5 ,(lte)c5 ,(lte)b5 ,(lte)a5
    ,(lte)2 ,(lte)g5 ,(lte)h5
    ,(lte)4 ,(lte)f4 ,(lte)f3 ,(lte)f2 ,(lte)f1
    ,(lte)3 ,(lte)f6 ,(lte)f7 ,(lte)f8
};
static const lte rook_f6[] = {
(lte)4
    ,(lte)5 ,(lte)e6 ,(lte)d6 ,(lte)c6 ,(lte)b6 ,(lte)a6
    ,(lte)2 ,(lte)g6 ,(lte)h6
    ,(lte)5 ,(lte)f5 ,(lte)f4 ,(lte)f3 ,(lte)f2 ,(lte)f1
    ,(lte)2 ,(lte)f7 ,(lte)f8
};
static const lte rook_f7[] = {
(lte)4
    ,(lte)5 ,(lte)e7 ,(lte)d7 ,(lte)c7 ,(lte)b7 ,(lte)a7
    ,(lte)2 ,(lte)g7 ,(lte)h7
    ,(lte)6 ,(lte)f6 ,(lte)f5 ,(lte)f4 ,(lte)f3 ,(lte)f2 ,(lte)f1
    ,(lte)1 ,(lte)f8
};
static const lte rook_f8[] = {
(lte)3
    ,(lte)5 ,(lte)e8 ,(lte)d8 ,(lte)c8 ,(lte)b8 ,(lte)a8
    ,(lte)2 ,(lte)g8 ,(lte)h8
    ,(lte)7 ,(lte)f7 ,(lte)f6 ,(lte)f5 ,(lte)f4 ,(lte)f3 ,(lte)f2 ,(lte)f1
};
static const lte rook_g1[] = {
(lte)3
    ,(lte)6 ,(lte)f1 ,(lte)e1 ,(lte)d1 ,(lte)c1 ,(lte)b1 ,(lte)a1
    ,(lte)1 ,(lte)h1
    ,(lte)7 ,(lte)g2 ,(lte)g3 ,(lte)g4 ,(lte)g5 ,(lte)g6 ,(lte)g7 ,(lte)g8
};
static const lte rook_g2[] = {
(lte)4
    ,(lte)6 ,(lte)f2 ,(lte)e2 ,(lte)d2 ,(lte)c2 ,(lte)b2 ,(lte)a2
    ,(lte)1 ,(lte)h2
    ,(lte)1 ,(lte)g1
    ,(lte)6 ,(lte)g3 ,(lte)g4 ,(lte)g5 ,(lte)g6 ,(lte)g7 ,(lte)g8
};
static const lte rook_g3[] = {
(lte)4
    ,(lte)6 ,(lte)f3 ,(lte)e3 ,(lte)d3 ,(lte)c3 ,(lte)b3 ,(lte)a3
    ,(lte)1 ,(lte)h3
    ,(lte)2 ,(lte)g2 ,(lte)g1
    ,(lte)5 ,(lte)g4 ,(lte)g5 ,(lte)g6 ,(lte)g7 ,(lte)g8
};
static const lte rook_g4[] = {
(lte)4
    ,(lte)6 ,(lte)f4 ,(lte)e4 ,(lte)d4 ,(lte)c4 ,(lte)b4 ,(lte)a4
    ,(lte)1 ,(lte)h4
    ,(lte)3 ,(lte)g3 ,(lte)g2 ,(lte)g1
    ,(lte)4 ,(lte)g5 ,(lte)g6 ,(lte)g7 ,(lte)g8
};
static const lte rook_g5[] = {
(lte)4
    ,(lte)6 ,(lte)f5 ,(lte)e5 ,(lte)d5 ,(lte)c5 ,(lte)b5 ,(lte)a5
    ,(lte)1 ,(lte)h5
    ,(lte)4 ,(lte)g4 ,(lte)g3 ,(lte)g2 ,(lte)g1
    ,(lte)3 ,(lte)g6 ,(lte)g7 ,(lte)g8
};
static const lte rook_g6[] = {
(lte)4
    ,(lte)6 ,(lte)f6 ,(lte)e6 ,(lte)d6 ,(lte)c6 ,(lte)b6 ,(lte)a6
    ,(lte)1 ,(lte)h6
    ,(lte)5 ,(lte)g5 ,(lte)g4 ,(lte)g3 ,(lte)g2 ,(lte)g1
    ,(lte)2 ,(lte)g7 ,(lte)g8
};
static const lte rook_g7[] = {
(lte)4
    ,(lte)6 ,(lte)f7 ,(lte)e7 ,(lte)d7 ,(lte)c7 ,(lte)b7 ,(lte)a7
    ,(lte)1 ,(lte)h7
    ,(lte)6 ,(lte)g6 ,(lte)g5 ,(lte)g4 ,(lte)g3 ,(lte)g2 ,(lte)g1
    ,(lte)1 ,(lte)g8
};
static const lte rook_g8[] = {
(lte)3
    ,(lte)6 ,(lte)f8 ,(lte)e8 ,(lte)d8 ,(lte)c8 ,(lte)b8 ,(lte)a8
    ,(lte)1 ,(lte)h8
    ,(lte)7 ,(lte)g7 ,(lte)g6 ,(lte)g5 ,(lte)g4 ,(lte)g3 ,(lte)g2 ,(lte)g1
};
static const lte rook_h1[] = {
(lte)2
    ,(lte)7 ,(lte)g1 ,(lte)f1 ,(lte)e1 ,(lte)d1 ,(lte)c1 ,(lte)b1 ,(lte)a1
    ,(lte)7 ,(lte)h2 ,(lte)h3 ,(lte)h4 ,(lte)h5 ,(lte)h6 ,(lte)h7 ,(lte)h8
};
static const lte rook_h2[] = {
(lte)3
    ,(lte)7 ,(lte)g2 ,(lte)f2 ,(lte)e2 ,(lte)d2 ,(lte)c2 ,(lte)b2 ,(lte)a2
    ,(lte)1 ,(lte)h1
    ,(lte)6 ,(lte)h3 ,(lte)h4 ,(lte)h5 ,(lte)h6 ,(lte)h7 ,(lte)h8
};
static const lte rook_h3[] = {
(lte)3
    ,(lte)7 ,(lte)g3 ,(lte)f3 ,(lte)e3 ,(lte)d3 ,(lte)c3 ,(lte)b3 ,(lte)a3
    ,(lte)2 ,(lte)h2 ,(lte)h1
    ,(lte)5 ,(lte)h4 ,(lte)h5 ,(lte)h6 ,(lte)h7 ,(lte)h8
};
static const lte rook_h4[] = {
(lte)3
    ,(lte)7 ,(lte)g4 ,(lte)f4 ,(lte)e4 ,(lte)d4 ,(lte)c4 ,(lte)b4 ,(lte)a4
    ,(lte)3 ,(lte)h3 ,(lte)h2 ,(lte)h1
    ,(lte)4 ,(lte)h5 ,(lte)h6 ,(lte)h7 ,(lte)h8
};
static const lte rook_h5[] = {
(lte)3
    ,(lte)7 ,(lte)g5 ,(lte)f5 ,(lte)e5 ,(lte)d5 ,(lte)c5 ,(lte)b5 ,(lte)a5
    ,(lte)4 ,(lte)h4 ,(lte)h3 ,(lte)h2 ,(lte)h1
    ,(lte)3 ,(lte)h6 ,(lte)h7 ,(lte)h8
};
static const lte rook_h6[] = {
(lte)3
    ,(lte)7 ,(lte)g6 ,(lte)f6 ,(lte)e6 ,(lte)d6 ,(lte)c6 ,(lte)b6 ,(lte)a6
    ,(lte)5 ,(lte)h5 ,(lte)h4 ,(lte)h3 ,(lte)h2 ,(lte)h1
    ,(lte)2 ,(lte)h7 ,(lte)h8
};
static const lte rook_h7[] = {
(lte)3
    ,(lte)7 ,(lte)g7 ,(lte)f7 ,(lte)e7 ,(lte)d7 ,(lte)c7 ,(lte)b7 ,(lte)a7
    ,(lte)6 ,(lte)h6 ,(lte)h5 ,(lte)h4 ,(lte)h3 ,(lte)h2 ,(lte)h1
    ,(lte)1 ,(lte)h8
};
static const lte rook_h8[] = {
(lte)2
    ,(lte)7 ,(lte)g8 ,(lte)f8 ,(lte)e8 ,(lte)d8 ,(lte)c8 ,(lte)b8 ,(lte)a8
    ,(lte)7 ,(lte)h7 ,(lte)h6 ,(lte)h5 ,(lte)h4 ,(lte)h3 ,(lte)h2 ,(lte)h1
};

const lte* rook_lookup[] = {
    rook_a8, rook_b8, rook_c8, rook_d8, rook_e8, rook_f8, rook_g8, rook_h8,
    rook_a7, rook_b7, rook_c7, rook_d7, rook_e7, rook_f7, rook_g7, rook_h7,
    rook_a6, rook_b6, rook_c6, rook_d6, rook_e6, rook_f6, rook_g6, rook_h6,
    rook_a5, rook_b5, rook_c5, rook_d5, rook_e5, rook_f5, rook_g5, rook_h5,
    rook_a4, rook_b4, rook_c4, rook_d4, rook_e4, rook_f4, rook_g4, rook_h4,
    rook_a3, rook_b3, rook_c3, rook_d3, rook_e3, rook_f3, rook_g3, rook_h3,
    rook_a2, rook_b2, rook_c2, rook_d2, rook_e2, rook_f2, rook_g2, rook_h2,
    rook_a1, rook_b1, rook_c1, rook_d1, rook_e1, rook_f1, rook_g1, rook_h1,
};

// Bishop, up to 4 rays
static const lte bishop_a1[] = {
(lte)1
    ,(lte)7 ,(lte)b2 ,(lte)c3 ,(lte)d4 ,(lte)e5 ,(lte)f6 ,(lte)g7 ,(lte)h8
};
static const lte bishop_a2[] = {
(lte)2
    ,(lte)6 ,(lte)b3 ,(lte)c4 ,(lte)d5 ,(lte)e6 ,(lte)f7 ,(lte)g8
    ,(lte)1 ,(lte)b1
};
static const lte bishop_a3[] = {
(lte)2
    ,(lte)5 ,(lte)b4 ,(lte)c5 ,(lte)d6 ,(lte)e7 ,(lte)f8
    ,(lte)2 ,(lte)b2 ,(lte)c1
};
static const lte bishop_a4[] = {
(lte)2
    ,(lte)4 ,(lte)b5 ,(lte)c6 ,(lte)d7 ,(lte)e8
    ,(lte)3 ,(lte)b3 ,(lte)c2 ,(lte)d1
};
static const lte bishop_a5[] = {
(lte)2
    ,(lte)3 ,(lte)b6 ,(lte)c7 ,(lte)d8
    ,(lte)4 ,(lte)b4 ,(lte)c3 ,(lte)d2 ,(lte)e1
};
static const lte bishop_a6[] = {
(lte)2
    ,(lte)2 ,(lte)b7 ,(lte)c8
    ,(lte)5 ,(lte)b5 ,(lte)c4 ,(lte)d3 ,(lte)e2 ,(lte)f1
};
static const lte bishop_a7[] = {
(lte)2
    ,(lte)1 ,(lte)b8
    ,(lte)6 ,(lte)b6 ,(lte)c5 ,(lte)d4 ,(lte)e3 ,(lte)f2 ,(lte)g1
};
static const lte bishop_a8[] = {
(lte)1
    ,(lte)7 ,(lte)b7 ,(lte)c6 ,(lte)d5 ,(lte)e4 ,(lte)f3 ,(lte)g2 ,(lte)h1
};
static const lte bishop_b1[] = {
(lte)2
    ,(lte)1 ,(lte)a2
    ,(lte)6 ,(lte)c2 ,(lte)d3 ,(lte)e4 ,(lte)f5 ,(lte)g6 ,(lte)h7
};
static const lte bishop_b2[] = {
(lte)4
    ,(lte)1 ,(lte)a1
    ,(lte)1 ,(lte)a3
    ,(lte)6 ,(lte)c3 ,(lte)d4 ,(lte)e5 ,(lte)f6 ,(lte)g7 ,(lte)h8
    ,(lte)1 ,(lte)c1
};
static const lte bishop_b3[] = {
(lte)4
    ,(lte)1 ,(lte)a2
    ,(lte)1 ,(lte)a4
    ,(lte)5 ,(lte)c4 ,(lte)d5 ,(lte)e6 ,(lte)f7 ,(lte)g8
    ,(lte)2 ,(lte)c2 ,(lte)d1
};
static const lte bishop_b4[] = {
(lte)4
    ,(lte)1 ,(lte)a3
    ,(lte)1 ,(lte)a5
    ,(lte)4 ,(lte)c5 ,(lte)d6 ,(lte)e7 ,(lte)f8
    ,(lte)3 ,(lte)c3 ,(lte)d2 ,(lte)e1
};
static const lte bishop_b5[] = {
(lte)4
    ,(lte)1 ,(lte)a4
    ,(lte)1 ,(lte)a6
    ,(lte)3 ,(lte)c6 ,(lte)d7 ,(lte)e8
    ,(lte)4 ,(lte)c4 ,(lte)d3 ,(lte)e2 ,(lte)f1
};
static const lte bishop_b6[] = {
(lte)4
    ,(lte)1 ,(lte)a5
    ,(lte)1 ,(lte)a7
    ,(lte)2 ,(lte)c7 ,(lte)d8
    ,(lte)5 ,(lte)c5 ,(lte)d4 ,(lte)e3 ,(lte)f2 ,(lte)g1
};
static const lte bishop_b7[] = {
(lte)4
    ,(lte)1 ,(lte)a6
    ,(lte)1 ,(lte)a8
    ,(lte)1 ,(lte)c8
    ,(lte)6 ,(lte)c6 ,(lte)d5 ,(lte)e4 ,(lte)f3 ,(lte)g2 ,(lte)h1
};
static const lte bishop_b8[] = {
(lte)2
    ,(lte)1 ,(lte)a7
    ,(lte)6 ,(lte)c7 ,(lte)d6 ,(lte)e5 ,(lte)f4 ,(lte)g3 ,(lte)h2
};
static const lte bishop_c1[] = {
(lte)2
    ,(lte)2 ,(lte)b2 ,(lte)a3
    ,(lte)5 ,(lte)d2 ,(lte)e3 ,(lte)f4 ,(lte)g5 ,(lte)h6
};
static const lte bishop_c2[] = {
(lte)4
    ,(lte)1 ,(lte)b1
    ,(lte)2 ,(lte)b3 ,(lte)a4
    ,(lte)5 ,(lte)d3 ,(lte)e4 ,(lte)f5 ,(lte)g6 ,(lte)h7
    ,(lte)1 ,(lte)d1
};
static const lte bishop_c3[] = {
(lte)4
    ,(lte)2 ,(lte)b2 ,(lte)a1
    ,(lte)2 ,(lte)b4 ,(lte)a5
    ,(lte)5 ,(lte)d4 ,(lte)e5 ,(lte)f6 ,(lte)g7 ,(lte)h8
    ,(lte)2 ,(lte)d2 ,(lte)e1
};
static const lte bishop_c4[] = {
(lte)4
    ,(lte)2 ,(lte)b3 ,(lte)a2
    ,(lte)2 ,(lte)b5 ,(lte)a6
    ,(lte)4 ,(lte)d5 ,(lte)e6 ,(lte)f7 ,(lte)g8
    ,(lte)3 ,(lte)d3 ,(lte)e2 ,(lte)f1
};
static const lte bishop_c5[] = {
(lte)4
    ,(lte)2 ,(lte)b4 ,(lte)a3
    ,(lte)2 ,(lte)b6 ,(lte)a7
    ,(lte)3 ,(lte)d6 ,(lte)e7 ,(lte)f8
    ,(lte)4 ,(lte)d4 ,(lte)e3 ,(lte)f2 ,(lte)g1
};
static const lte bishop_c6[] = {
(lte)4
    ,(lte)2 ,(lte)b5 ,(lte)a4
    ,(lte)2 ,(lte)b7 ,(lte)a8
    ,(lte)2 ,(lte)d7 ,(lte)e8
    ,(lte)5 ,(lte)d5 ,(lte)e4 ,(lte)f3 ,(lte)g2 ,(lte)h1
};
static const lte bishop_c7[] = {
(lte)4
    ,(lte)2 ,(lte)b6 ,(lte)a5
    ,(lte)1 ,(lte)b8
    ,(lte)1 ,(lte)d8
    ,(lte)5 ,(lte)d6 ,(lte)e5 ,(lte)f4 ,(lte)g3 ,(lte)h2
};
static const lte bishop_c8[] = {
(lte)2
    ,(lte)2 ,(lte)b7 ,(lte)a6
    ,(lte)5 ,(lte)d7 ,(lte)e6 ,(lte)f5 ,(lte)g4 ,(lte)h3
};
static const lte bishop_d1[] = {
(lte)2
    ,(lte)3 ,(lte)c2 ,(lte)b3 ,(lte)a4
    ,(lte)4 ,(lte)e2 ,(lte)f3 ,(lte)g4 ,(lte)h5
};
static const lte bishop_d2[] = {
(lte)4
    ,(lte)1 ,(lte)c1
    ,(lte)3 ,(lte)c3 ,(lte)b4 ,(lte)a5
    ,(lte)4 ,(lte)e3 ,(lte)f4 ,(lte)g5 ,(lte)h6
    ,(lte)1 ,(lte)e1
};
static const lte bishop_d3[] = {
(lte)4
    ,(lte)2 ,(lte)c2 ,(lte)b1
    ,(lte)3 ,(lte)c4 ,(lte)b5 ,(lte)a6
    ,(lte)4 ,(lte)e4 ,(lte)f5 ,(lte)g6 ,(lte)h7
    ,(lte)2 ,(lte)e2 ,(lte)f1
};
static const lte bishop_d4[] = {
(lte)4
    ,(lte)3 ,(lte)c3 ,(lte)b2 ,(lte)a1
    ,(lte)3 ,(lte)c5 ,(lte)b6 ,(lte)a7
    ,(lte)4 ,(lte)e5 ,(lte)f6 ,(lte)g7 ,(lte)h8
    ,(lte)3 ,(lte)e3 ,(lte)f2 ,(lte)g1
};
static const lte bishop_d5[] = {
(lte)4
    ,(lte)3 ,(lte)c4 ,(lte)b3 ,(lte)a2
    ,(lte)3 ,(lte)c6 ,(lte)b7 ,(lte)a8
    ,(lte)3 ,(lte)e6 ,(lte)f7 ,(lte)g8
    ,(lte)4 ,(lte)e4 ,(lte)f3 ,(lte)g2 ,(lte)h1
};
static const lte bishop_d6[] = {
(lte)4
    ,(lte)3 ,(lte)c5 ,(lte)b4 ,(lte)a3
    ,(lte)2 ,(lte)c7 ,(lte)b8
    ,(lte)2 ,(lte)e7 ,(lte)f8
    ,(lte)4 ,(lte)e5 ,(lte)f4 ,(lte)g3 ,(lte)h2
};
static const lte bishop_d7[] = {
(lte)4
    ,(lte)3 ,(lte)c6 ,(lte)b5 ,(lte)a4
    ,(lte)1 ,(lte)c8
    ,(lte)1 ,(lte)e8
    ,(lte)4 ,(lte)e6 ,(lte)f5 ,(lte)g4 ,(lte)h3
};
static const lte bishop_d8[] = {
(lte)2
    ,(lte)3 ,(lte)c7 ,(lte)b6 ,(lte)a5
    ,(lte)4 ,(lte)e7 ,(lte)f6 ,(lte)g5 ,(lte)h4
};
static const lte bishop_e1[] = {
(lte)2
    ,(lte)4 ,(lte)d2 ,(lte)c3 ,(lte)b4 ,(lte)a5
    ,(lte)3 ,(lte)f2 ,(lte)g3 ,(lte)h4
};
static const lte bishop_e2[] = {
(lte)4
    ,(lte)1 ,(lte)d1
    ,(lte)4 ,(lte)d3 ,(lte)c4 ,(lte)b5 ,(lte)a6
    ,(lte)3 ,(lte)f3 ,(lte)g4 ,(lte)h5
    ,(lte)1 ,(lte)f1
};
static const lte bishop_e3[] = {
(lte)4
    ,(lte)2 ,(lte)d2 ,(lte)c1
    ,(lte)4 ,(lte)d4 ,(lte)c5 ,(lte)b6 ,(lte)a7
    ,(lte)3 ,(lte)f4 ,(lte)g5 ,(lte)h6
    ,(lte)2 ,(lte)f2 ,(lte)g1
};
static const lte bishop_e4[] = {
(lte)4
    ,(lte)3 ,(lte)d3 ,(lte)c2 ,(lte)b1
    ,(lte)4 ,(lte)d5 ,(lte)c6 ,(lte)b7 ,(lte)a8
    ,(lte)3 ,(lte)f5 ,(lte)g6 ,(lte)h7
    ,(lte)3 ,(lte)f3 ,(lte)g2 ,(lte)h1
};
static const lte bishop_e5[] = {
(lte)4
    ,(lte)4 ,(lte)d4 ,(lte)c3 ,(lte)b2 ,(lte)a1
    ,(lte)3 ,(lte)d6 ,(lte)c7 ,(lte)b8
    ,(lte)3 ,(lte)f6 ,(lte)g7 ,(lte)h8
    ,(lte)3 ,(lte)f4 ,(lte)g3 ,(lte)h2
};
static const lte bishop_e6[] = {
(lte)4
    ,(lte)4 ,(lte)d5 ,(lte)c4 ,(lte)b3 ,(lte)a2
    ,(lte)2 ,(lte)d7 ,(lte)c8
    ,(lte)2 ,(lte)f7 ,(lte)g8
    ,(lte)3 ,(lte)f5 ,(lte)g4 ,(lte)h3
};
static const lte bishop_e7[] = {
(lte)4
    ,(lte)4 ,(lte)d6 ,(lte)c5 ,(lte)b4 ,(lte)a3
    ,(lte)1 ,(lte)d8
    ,(lte)1 ,(lte)f8
    ,(lte)3 ,(lte)f6 ,(lte)g5 ,(lte)h4
};
static const lte bishop_e8[] = {
(lte)2
    ,(lte)4 ,(lte)d7 ,(lte)c6 ,(lte)b5 ,(lte)a4
    ,(lte)3 ,(lte)f7 ,(lte)g6 ,(lte)h5
};
static const lte bishop_f1[] = {
(lte)2
    ,(lte)5 ,(lte)e2 ,(lte)d3 ,(lte)c4 ,(lte)b5 ,(lte)a6
    ,(lte)2 ,(lte)g2 ,(lte)h3
};
static const lte bishop_f2[] = {
(lte)4
    ,(lte)1 ,(lte)e1
    ,(lte)5 ,(lte)e3 ,(lte)d4 ,(lte)c5 ,(lte)b6 ,(lte)a7
    ,(lte)2 ,(lte)g3 ,(lte)h4
    ,(lte)1 ,(lte)g1
};
static const lte bishop_f3[] = {
(lte)4
    ,(lte)2 ,(lte)e2 ,(lte)d1
    ,(lte)5 ,(lte)e4 ,(lte)d5 ,(lte)c6 ,(lte)b7 ,(lte)a8
    ,(lte)2 ,(lte)g4 ,(lte)h5
    ,(lte)2 ,(lte)g2 ,(lte)h1
};
static const lte bishop_f4[] = {
(lte)4
    ,(lte)3 ,(lte)e3 ,(lte)d2 ,(lte)c1
    ,(lte)4 ,(lte)e5 ,(lte)d6 ,(lte)c7 ,(lte)b8
    ,(lte)2 ,(lte)g5 ,(lte)h6
    ,(lte)2 ,(lte)g3 ,(lte)h2
};
static const lte bishop_f5[] = {
(lte)4
    ,(lte)4 ,(lte)e4 ,(lte)d3 ,(lte)c2 ,(lte)b1
    ,(lte)3 ,(lte)e6 ,(lte)d7 ,(lte)c8
    ,(lte)2 ,(lte)g6 ,(lte)h7
    ,(lte)2 ,(lte)g4 ,(lte)h3
};
static const lte bishop_f6[] = {
(lte)4
    ,(lte)5 ,(lte)e5 ,(lte)d4 ,(lte)c3 ,(lte)b2 ,(lte)a1
    ,(lte)2 ,(lte)e7 ,(lte)d8
    ,(lte)2 ,(lte)g7 ,(lte)h8
    ,(lte)2 ,(lte)g5 ,(lte)h4
};
static const lte bishop_f7[] = {
(lte)4
    ,(lte)5 ,(lte)e6 ,(lte)d5 ,(lte)c4 ,(lte)b3 ,(lte)a2
    ,(lte)1 ,(lte)e8
    ,(lte)1 ,(lte)g8
    ,(lte)2 ,(lte)g6 ,(lte)h5
};
static const lte bishop_f8[] = {
(lte)2
    ,(lte)5 ,(lte)e7 ,(lte)d6 ,(lte)c5 ,(lte)b4 ,(lte)a3
    ,(lte)2 ,(lte)g7 ,(lte)h6
};
static const lte bishop_g1[] = {
(lte)2
    ,(lte)6 ,(lte)f2 ,(lte)e3 ,(lte)d4 ,(lte)c5 ,(lte)b6 ,(lte)a7
    ,(lte)1 ,(lte)h2
};
static const lte bishop_g2[] = {
(lte)4
    ,(lte)1 ,(lte)f1
    ,(lte)6 ,(lte)f3 ,(lte)e4 ,(lte)d5 ,(lte)c6 ,(lte)b7 ,(lte)a8
    ,(lte)1 ,(lte)h3
    ,(lte)1 ,(lte)h1
};
static const lte bishop_g3[] = {
(lte)4
    ,(lte)2 ,(lte)f2 ,(lte)e1
    ,(lte)5 ,(lte)f4 ,(lte)e5 ,(lte)d6 ,(lte)c7 ,(lte)b8
    ,(lte)1 ,(lte)h4
    ,(lte)1 ,(lte)h2
};
static const lte bishop_g4[] = {
(lte)4
    ,(lte)3 ,(lte)f3 ,(lte)e2 ,(lte)d1
    ,(lte)4 ,(lte)f5 ,(lte)e6 ,(lte)d7 ,(lte)c8
    ,(lte)1 ,(lte)h5
    ,(lte)1 ,(lte)h3
};
static const lte bishop_g5[] = {
(lte)4
    ,(lte)4 ,(lte)f4 ,(lte)e3 ,(lte)d2 ,(lte)c1
    ,(lte)3 ,(lte)f6 ,(lte)e7 ,(lte)d8
    ,(lte)1 ,(lte)h6
    ,(lte)1 ,(lte)h4
};
static const lte bishop_g6[] = {
(lte)4
    ,(lte)5 ,(lte)f5 ,(lte)e4 ,(lte)d3 ,(lte)c2 ,(lte)b1
    ,(lte)2 ,(lte)f7 ,(lte)e8
    ,(lte)1 ,(lte)h7
    ,(lte)1 ,(lte)h5
};
static const lte bishop_g7[] = {
(lte)4
    ,(lte)6 ,(lte)f6 ,(lte)e5 ,(lte)d4 ,(lte)c3 ,(lte)b2 ,(lte)a1
    ,(lte)1 ,(lte)f8
    ,(lte)1 ,(lte)h8
    ,(lte)1 ,(lte)h6
};
static const lte bishop_g8[] = {
(lte)2
    ,(lte)6 ,(lte)f7 ,(lte)e6 ,(lte)d5 ,(lte)c4 ,(lte)b3 ,(lte)a2
    ,(lte)1 ,(lte)h7
};
static const lte bishop_h1[] = {
(lte)1
    ,(lte)7 ,(lte)g2 ,(lte)f3 ,(lte)e4 ,(lte)d5 ,(lte)c6 ,(lte)b7 ,(lte)a8
};
static const lte bishop_h2[] = {
(lte)2
    ,(lte)1 ,(lte)g1
    ,(lte)6 ,(lte)g3 ,(lte)f4 ,(lte)e5 ,(lte)d6 ,(lte)c7 ,(lte)b8
};
static const lte bishop_h3[] = {
(lte)2
    ,(lte)2 ,(lte)g2 ,(lte)f1
    ,(lte)5 ,(lte)g4 ,(lte)f5 ,(lte)e6 ,(lte)d7 ,(lte)c8
};
static const lte bishop_h4[] = {
(lte)2
    ,(lte)3 ,(lte)g3 ,(lte)f2 ,(lte)e1
    ,(lte)4 ,(lte)g5 ,(lte)f6 ,(lte)e7 ,(lte)d8
};
static const lte bishop_h5[] = {
(lte)2
    ,(lte)4 ,(lte)g4 ,(lte)f3 ,(lte)e2 ,(lte)d1
    ,(lte)3 ,(lte)g6 ,(lte)f7 ,(lte)e8
};
static const lte bishop_h6[] = {
(lte)2
    ,(lte)5 ,(lte)g5 ,(lte)f4 ,(lte)e3 ,(lte)d2 ,(lte)c1
    ,(lte)2 ,(lte)g7 ,(lte)f8
};
static const lte bishop_h7[] = {
(lte)2
    ,(lte)6 ,(lte)g6 ,(lte)f5 ,(lte)e4 ,(lte)d3 ,(lte)c2 ,(lte)b1
    ,(lte)1 ,(lte)g8
};
static const lte bishop_h8[] = {
(lte)1
    ,(lte)7 ,(lte)g7 ,(lte)f6 ,(lte)e5 ,(lte)d4 ,(lte)c3 ,(lte)b2 ,(lte)a1
};

const lte *bishop_lookup[] = {
    bishop_a8, bishop_b8, bishop_c8, bishop_d8, bishop_e8, bishop_f8, bishop_g8, bishop_h8,
    bishop_a7, bishop_b7, bishop_c7, bishop_d7, bishop_e7, bishop_f7, bishop_g7, bishop_h7,
    bishop_a6, bishop_b6, bishop_c6, bishop_d6, bishop_e6, bishop_f6, bishop_g6, bishop_h6,
    bishop_a5, bishop_b5, bishop_c5, bishop_d5, bishop_e5, bishop_f5, bishop_g5, bishop_h5,
    bishop_a4, bishop_b4, bishop_c4, bishop_d4, bishop_e4, bishop_f4, bishop_g4, bishop_h4,
    bishop_a3, bishop_b3, bishop_c3, bishop_d3, bishop_e3, bishop_f3, bishop_g3, bishop_h3,
    bishop_a2, bishop_b2, bishop_c2, bishop_d2, bishop_e2, bishop_f2, bishop_g2, bishop_h2,
    bishop_a1, bishop_b1, bishop_c1, bishop_d1, bishop_e1, bishop_f1, bishop_g1, bishop_h1,
};

// Knight, up to 8 squares
static const lte knight_a1[] = {
    (lte)2, (lte)c2, (lte)b3
};
static const lte knight_a2[] = {
    (lte)3, (lte)c1, (lte)c3, (lte)b4
};
static const lte knight_a3[] = {
    (lte)4, (lte)c2, (lte)c4, (lte)b1, (lte)b5
};
static const lte knight_a4[] = {
    (lte)4, (lte)c3, (lte)c5, (lte)b2, (lte)b6
};
static const lte knight_a5[] = {
    (lte)4, (lte)c4, (lte)c6, (lte)b3, (lte)b7
};
static const lte knight_a6[] = {
    (lte)4, (lte)c5, (lte)c7, (lte)b4, (lte)b8
};
static const lte knight_a7[] = {
    (lte)3, (lte)c6, (lte)c8, (lte)b5
};
static const lte knight_a8[] = {
    (lte)2, (lte)c7, (lte)b6
};
static const lte knight_b1[] = {
    (lte)3, (lte)a3, (lte)d2, (lte)c3
};
static const lte knight_b2[] = {
    (lte)4, (lte)a4, (lte)d1, (lte)d3, (lte)c4
};
static const lte knight_b3[] = {
    (lte)6, (lte)a1, (lte)a5, (lte)d2, (lte)d4, (lte)c1, (lte)c5
};
static const lte knight_b4[] = {
    (lte)6, (lte)a2, (lte)a6, (lte)d3, (lte)d5, (lte)c2, (lte)c6
};
static const lte knight_b5[] = {
    (lte)6, (lte)a3, (lte)a7, (lte)d4, (lte)d6, (lte)c3, (lte)c7
};
static const lte knight_b6[] = {
    (lte)6, (lte)a4, (lte)a8, (lte)d5, (lte)d7, (lte)c4, (lte)c8
};
static const lte knight_b7[] = {
    (lte)4, (lte)a5, (lte)d6, (lte)d8, (lte)c5
};
static const lte knight_b8[] = {
    (lte)3, (lte)a6, (lte)d7, (lte)c6
};
static const lte knight_c1[] = {
    (lte)4, (lte)a2, (lte)b3, (lte)e2, (lte)d3
};
static const lte knight_c2[] = {
    (lte)6, (lte)a1, (lte)a3, (lte)b4, (lte)e1, (lte)e3, (lte)d4
};
static const lte knight_c3[] = {
    (lte)8, (lte)a2, (lte)a4, (lte)b1, (lte)b5, (lte)e2, (lte)e4, (lte)d1, (lte)d5
};
static const lte knight_c4[] = {
    (lte)8, (lte)a3, (lte)a5, (lte)b2, (lte)b6, (lte)e3, (lte)e5, (lte)d2, (lte)d6
};
static const lte knight_c5[] = {
    (lte)8, (lte)a4, (lte)a6, (lte)b3, (lte)b7, (lte)e4, (lte)e6, (lte)d3, (lte)d7
};
static const lte knight_c6[] = {
    (lte)8, (lte)a5, (lte)a7, (lte)b4, (lte)b8, (lte)e5, (lte)e7, (lte)d4, (lte)d8
};
static const lte knight_c7[] = {
    (lte)6, (lte)a6, (lte)a8, (lte)b5, (lte)e6, (lte)e8, (lte)d5
};
static const lte knight_c8[] = {
    (lte)4, (lte)a7, (lte)b6, (lte)e7, (lte)d6
};
static const lte knight_d1[] = {
    (lte)4, (lte)b2, (lte)c3, (lte)f2, (lte)e3
};
static const lte knight_d2[] = {
    (lte)6, (lte)b1, (lte)b3, (lte)c4, (lte)f1, (lte)f3, (lte)e4
};
static const lte knight_d3[] = {
    (lte)8, (lte)b2, (lte)b4, (lte)c1, (lte)c5, (lte)f2, (lte)f4, (lte)e1, (lte)e5
};
static const lte knight_d4[] = {
    (lte)8, (lte)b3, (lte)b5, (lte)c2, (lte)c6, (lte)f3, (lte)f5, (lte)e2, (lte)e6
};
static const lte knight_d5[] = {
    (lte)8, (lte)b4, (lte)b6, (lte)c3, (lte)c7, (lte)f4, (lte)f6, (lte)e3, (lte)e7
};
static const lte knight_d6[] = {
    (lte)8, (lte)b5, (lte)b7, (lte)c4, (lte)c8, (lte)f5, (lte)f7, (lte)e4, (lte)e8
};
static const lte knight_d7[] = {
    (lte)6, (lte)b6, (lte)b8, (lte)c5, (lte)f6, (lte)f8, (lte)e5
};
static const lte knight_d8[] = {
    (lte)4, (lte)b7, (lte)c6, (lte)f7, (lte)e6
};
static const lte knight_e1[] = {
    (lte)4, (lte)c2, (lte)d3, (lte)g2, (lte)f3
};
static const lte knight_e2[] = {
    (lte)6, (lte)c1, (lte)c3, (lte)d4, (lte)g1, (lte)g3, (lte)f4
};
static const lte knight_e3[] = {
    (lte)8, (lte)c2, (lte)c4, (lte)d1, (lte)d5, (lte)g2, (lte)g4, (lte)f1, (lte)f5
};
static const lte knight_e4[] = {
    (lte)8, (lte)c3, (lte)c5, (lte)d2, (lte)d6, (lte)g3, (lte)g5, (lte)f2, (lte)f6
};
static const lte knight_e5[] = {
    (lte)8, (lte)c4, (lte)c6, (lte)d3, (lte)d7, (lte)g4, (lte)g6, (lte)f3, (lte)f7
};
static const lte knight_e6[] = {
    (lte)8, (lte)c5, (lte)c7, (lte)d4, (lte)d8, (lte)g5, (lte)g7, (lte)f4, (lte)f8
};
static const lte knight_e7[] = {
    (lte)6, (lte)c6, (lte)c8, (lte)d5, (lte)g6, (lte)g8, (lte)f5
};
static const lte knight_e8[] = {
    (lte)4, (lte)c7, (lte)d6, (lte)g7, (lte)f6
};
static const lte knight_f1[] = {
    (lte)4, (lte)d2, (lte)e3, (lte)h2, (lte)g3
};
static const lte knight_f2[] = {
    (lte)6, (lte)d1, (lte)d3, (lte)e4, (lte)h1, (lte)h3, (lte)g4
};
static const lte knight_f3[] = {
    (lte)8, (lte)d2, (lte)d4, (lte)e1, (lte)e5, (lte)h2, (lte)h4, (lte)g1, (lte)g5
};
static const lte knight_f4[] = {
    (lte)8, (lte)d3, (lte)d5, (lte)e2, (lte)e6, (lte)h3, (lte)h5, (lte)g2, (lte)g6
};
static const lte knight_f5[] = {
    (lte)8, (lte)d4, (lte)d6, (lte)e3, (lte)e7, (lte)h4, (lte)h6, (lte)g3, (lte)g7
};
static const lte knight_f6[] = {
    (lte)8, (lte)d5, (lte)d7, (lte)e4, (lte)e8, (lte)h5, (lte)h7, (lte)g4, (lte)g8
};
static const lte knight_f7[] = {
    (lte)6, (lte)d6, (lte)d8, (lte)e5, (lte)h6, (lte)h8, (lte)g5
};
static const lte knight_f8[] = {
    (lte)4, (lte)d7, (lte)e6, (lte)h7, (lte)g6
};
static const lte knight_g1[] = {
    (lte)3, (lte)e2, (lte)f3, (lte)h3
};
static const lte knight_g2[] = {
    (lte)4, (lte)e1, (lte)e3, (lte)f4, (lte)h4
};
static const lte knight_g3[] = {
    (lte)6, (lte)e2, (lte)e4, (lte)f1, (lte)f5, (lte)h1, (lte)h5
};
static const lte knight_g4[] = {
    (lte)6, (lte)e3, (lte)e5, (lte)f2, (lte)f6, (lte)h2, (lte)h6
};
static const lte knight_g5[] = {
    (lte)6, (lte)e4, (lte)e6, (lte)f3, (lte)f7, (lte)h3, (lte)h7
};
static const lte knight_g6[] = {
    (lte)6, (lte)e5, (lte)e7, (lte)f4, (lte)f8, (lte)h4, (lte)h8
};
static const lte knight_g7[] = {
    (lte)4, (lte)e6, (lte)e8, (lte)f5, (lte)h5
};
static const lte knight_g8[] = {
    (lte)3, (lte)e7, (lte)f6, (lte)h6
};
static const lte knight_h1[] = {
    (lte)2, (lte)f2, (lte)g3
};
static const lte knight_h2[] = {
    (lte)3, (lte)f1, (lte)f3, (lte)g4
};
static const lte knight_h3[] = {
    (lte)4, (lte)f2, (lte)f4, (lte)g1, (lte)g5
};
static const lte knight_h4[] = {
    (lte)4, (lte)f3, (lte)f5, (lte)g2, (lte)g6
};
static const lte knight_h5[] = {
    (lte)4, (lte)f4, (lte)f6, (lte)g3, (lte)g7
};
static const lte knight_h6[] = {
    (lte)4, (lte)f5, (lte)f7, (lte)g4, (lte)g8
};
static const lte knight_h7[] = {
    (lte)3, (lte)f6, (lte)f8, (lte)g5
};
static const lte knight_h8[] = {
    (lte)2, (lte)f7, (lte)g6
};

// knight_lookup
const lte *knight_lookup[] = {
    knight_a8, knight_b8, knight_c8, knight_d8, knight_e8, knight_f8, knight_g8, knight_h8,
    knight_a7, knight_b7, knight_c7, knight_d7, knight_e7, knight_f7, knight_g7, knight_h7,
    knight_a6, knight_b6, knight_c6, knight_d6, knight_e6, knight_f6, knight_g6, knight_h6,
    knight_a5, knight_b5, knight_c5, knight_d5, knight_e5, knight_f5, knight_g5, knight_h5,
    knight_a4, knight_b4, knight_c4, knight_d4, knight_e4, knight_f4, knight_g4, knight_h4,
    knight_a3, knight_b3, knight_c3, knight_d3, knight_e3, knight_f3, knight_g3, knight_h3,
    knight_a2, knight_b2, knight_c2, knight_d2, knight_e2, knight_f2, knight_g2, knight_h2,
    knight_a1, knight_b1, knight_c1, knight_d1, knight_e1, knight_f1, knight_g1, knight_h1,
};

// King, up to 8 squares
static const lte king_a1[] = {
    (lte)3, (lte)b2, (lte)b1, (lte)a2
};
static const lte king_a2[] = {
    (lte)5, (lte)b1, (lte)b3, (lte)b2, (lte)a1, (lte)a3
};
static const lte king_a3[] = {
    (lte)5, (lte)b2, (lte)b4, (lte)b3, (lte)a2, (lte)a4
};
static const lte king_a4[] = {
    (lte)5, (lte)b3, (lte)b5, (lte)b4, (lte)a3, (lte)a5
};
static const lte king_a5[] = {
    (lte)5, (lte)b4, (lte)b6, (lte)b5, (lte)a4, (lte)a6
};
static const lte king_a6[] = {
    (lte)5, (lte)b5, (lte)b7, (lte)b6, (lte)a5, (lte)a7
};
static const lte king_a7[] = {
    (lte)5, (lte)b6, (lte)b8, (lte)b7, (lte)a6, (lte)a8
};
static const lte king_a8[] = {
    (lte)3, (lte)b7, (lte)b8, (lte)a7
};
static const lte king_b1[] = {
    (lte)5, (lte)a2, (lte)c2, (lte)a1, (lte)c1, (lte)b2
};
static const lte king_b2[] = {
    (lte)8, (lte)a1, (lte)a3, (lte)c1, (lte)c3, (lte)a2, (lte)c2, (lte)b1, (lte)b3
};
static const lte king_b3[] = {
    (lte)8, (lte)a2, (lte)a4, (lte)c2, (lte)c4, (lte)a3, (lte)c3, (lte)b2, (lte)b4
};
static const lte king_b4[] = {
    (lte)8, (lte)a3, (lte)a5, (lte)c3, (lte)c5, (lte)a4, (lte)c4, (lte)b3, (lte)b5
};
static const lte king_b5[] = {
    (lte)8, (lte)a4, (lte)a6, (lte)c4, (lte)c6, (lte)a5, (lte)c5, (lte)b4, (lte)b6
};
static const lte king_b6[] = {
    (lte)8, (lte)a5, (lte)a7, (lte)c5, (lte)c7, (lte)a6, (lte)c6, (lte)b5, (lte)b7
};
static const lte king_b7[] = {
    (lte)8, (lte)a6, (lte)a8, (lte)c6, (lte)c8, (lte)a7, (lte)c7, (lte)b6, (lte)b8
};
static const lte king_b8[] = {
    (lte)5, (lte)a7, (lte)c7, (lte)a8, (lte)c8, (lte)b7
};
static const lte king_c1[] = {
    (lte)5, (lte)b2, (lte)d2, (lte)b1, (lte)d1, (lte)c2
};
static const lte king_c2[] = {
    (lte)8, (lte)b1, (lte)b3, (lte)d1, (lte)d3, (lte)b2, (lte)d2, (lte)c1, (lte)c3
};
static const lte king_c3[] = {
    (lte)8, (lte)b2, (lte)b4, (lte)d2, (lte)d4, (lte)b3, (lte)d3, (lte)c2, (lte)c4
};
static const lte king_c4[] = {
    (lte)8, (lte)b3, (lte)b5, (lte)d3, (lte)d5, (lte)b4, (lte)d4, (lte)c3, (lte)c5
};
static const lte king_c5[] = {
    (lte)8, (lte)b4, (lte)b6, (lte)d4, (lte)d6, (lte)b5, (lte)d5, (lte)c4, (lte)c6
};
static const lte king_c6[] = {
    (lte)8, (lte)b5, (lte)b7, (lte)d5, (lte)d7, (lte)b6, (lte)d6, (lte)c5, (lte)c7
};
static const lte king_c7[] = {
    (lte)8, (lte)b6, (lte)b8, (lte)d6, (lte)d8, (lte)b7, (lte)d7, (lte)c6, (lte)c8
};
static const lte king_c8[] = {
    (lte)5, (lte)b7, (lte)d7, (lte)b8, (lte)d8, (lte)c7
};
static const lte king_d1[] = {
    (lte)5, (lte)c2, (lte)e2, (lte)c1, (lte)e1, (lte)d2
};
static const lte king_d2[] = {
    (lte)8, (lte)c1, (lte)c3, (lte)e1, (lte)e3, (lte)c2, (lte)e2, (lte)d1, (lte)d3
};
static const lte king_d3[] = {
    (lte)8, (lte)c2, (lte)c4, (lte)e2, (lte)e4, (lte)c3, (lte)e3, (lte)d2, (lte)d4
};
static const lte king_d4[] = {
    (lte)8, (lte)c3, (lte)c5, (lte)e3, (lte)e5, (lte)c4, (lte)e4, (lte)d3, (lte)d5
};
static const lte king_d5[] = {
    (lte)8, (lte)c4, (lte)c6, (lte)e4, (lte)e6, (lte)c5, (lte)e5, (lte)d4, (lte)d6
};
static const lte king_d6[] = {
    (lte)8, (lte)c5, (lte)c7, (lte)e5, (lte)e7, (lte)c6, (lte)e6, (lte)d5, (lte)d7
};
static const lte king_d7[] = {
    (lte)8, (lte)c6, (lte)c8, (lte)e6, (lte)e8, (lte)c7, (lte)e7, (lte)d6, (lte)d8
};
static const lte king_d8[] = {
    (lte)5, (lte)c7, (lte)e7, (lte)c8, (lte)e8, (lte)d7
};
static const lte king_e1[] = {
    (lte)5, (lte)d2, (lte)f2, (lte)d1, (lte)f1, (lte)e2
};
static const lte king_e2[] = {
    (lte)8, (lte)d1, (lte)d3, (lte)f1, (lte)f3, (lte)d2, (lte)f2, (lte)e1, (lte)e3
};
static const lte king_e3[] = {
    (lte)8, (lte)d2, (lte)d4, (lte)f2, (lte)f4, (lte)d3, (lte)f3, (lte)e2, (lte)e4
};
static const lte king_e4[] = {
    (lte)8, (lte)d3, (lte)d5, (lte)f3, (lte)f5, (lte)d4, (lte)f4, (lte)e3, (lte)e5
};
static const lte king_e5[] = {
    (lte)8, (lte)d4, (lte)d6, (lte)f4, (lte)f6, (lte)d5, (lte)f5, (lte)e4, (lte)e6
};
static const lte king_e6[] = {
    (lte)8, (lte)d5, (lte)d7, (lte)f5, (lte)f7, (lte)d6, (lte)f6, (lte)e5, (lte)e7
};
static const lte king_e7[] = {
    (lte)8, (lte)d6, (lte)d8, (lte)f6, (lte)f8, (lte)d7, (lte)f7, (lte)e6, (lte)e8
};
static const lte king_e8[] = {
    (lte)5, (lte)d7, (lte)f7, (lte)d8, (lte)f8, (lte)e7
};
static const lte king_f1[] = {
    (lte)5, (lte)e2, (lte)g2, (lte)e1, (lte)g1, (lte)f2
};
static const lte king_f2[] = {
    (lte)8, (lte)e1, (lte)e3, (lte)g1, (lte)g3, (lte)e2, (lte)g2, (lte)f1, (lte)f3
};
static const lte king_f3[] = {
    (lte)8, (lte)e2, (lte)e4, (lte)g2, (lte)g4, (lte)e3, (lte)g3, (lte)f2, (lte)f4
};
static const lte king_f4[] = {
    (lte)8, (lte)e3, (lte)e5, (lte)g3, (lte)g5, (lte)e4, (lte)g4, (lte)f3, (lte)f5
};
static const lte king_f5[] = {
    (lte)8, (lte)e4, (lte)e6, (lte)g4, (lte)g6, (lte)e5, (lte)g5, (lte)f4, (lte)f6
};
static const lte king_f6[] = {
    (lte)8, (lte)e5, (lte)e7, (lte)g5, (lte)g7, (lte)e6, (lte)g6, (lte)f5, (lte)f7
};
static const lte king_f7[] = {
    (lte)8, (lte)e6, (lte)e8, (lte)g6, (lte)g8, (lte)e7, (lte)g7, (lte)f6, (lte)f8
};
static const lte king_f8[] = {
    (lte)5, (lte)e7, (lte)g7, (lte)e8, (lte)g8, (lte)f7
};
static const lte king_g1[] = {
    (lte)5, (lte)f2, (lte)h2, (lte)f1, (lte)h1, (lte)g2
};
static const lte king_g2[] = {
    (lte)8, (lte)f1, (lte)f3, (lte)h1, (lte)h3, (lte)f2, (lte)h2, (lte)g1, (lte)g3
};
static const lte king_g3[] = {
    (lte)8, (lte)f2, (lte)f4, (lte)h2, (lte)h4, (lte)f3, (lte)h3, (lte)g2, (lte)g4
};
static const lte king_g4[] = {
    (lte)8, (lte)f3, (lte)f5, (lte)h3, (lte)h5, (lte)f4, (lte)h4, (lte)g3, (lte)g5
};
static const lte king_g5[] = {
    (lte)8, (lte)f4, (lte)f6, (lte)h4, (lte)h6, (lte)f5, (lte)h5, (lte)g4, (lte)g6
};
static const lte king_g6[] = {
    (lte)8, (lte)f5, (lte)f7, (lte)h5, (lte)h7, (lte)f6, (lte)h6, (lte)g5, (lte)g7
};
static const lte king_g7[] = {
    (lte)8, (lte)f6, (lte)f8, (lte)h6, (lte)h8, (lte)f7, (lte)h7, (lte)g6, (lte)g8
};
static const lte king_g8[] = {
    (lte)5, (lte)f7, (lte)h7, (lte)f8, (lte)h8, (lte)g7
};
static const lte king_h1[] = {
    (lte)3, (lte)g2, (lte)g1, (lte)h2
};
static const lte king_h2[] = {
    (lte)5, (lte)g1, (lte)g3, (lte)g2, (lte)h1, (lte)h3
};
static const lte king_h3[] = {
    (lte)5, (lte)g2, (lte)g4, (lte)g3, (lte)h2, (lte)h4
};
static const lte king_h4[] = {
    (lte)5, (lte)g3, (lte)g5, (lte)g4, (lte)h3, (lte)h5
};
static const lte king_h5[] = {
    (lte)5, (lte)g4, (lte)g6, (lte)g5, (lte)h4, (lte)h6
};
static const lte king_h6[] = {
    (lte)5, (lte)g5, (lte)g7, (lte)g6, (lte)h5, (lte)h7
};
static const lte king_h7[] = {
    (lte)5, (lte)g6, (lte)g8, (lte)g7, (lte)h6, (lte)h8
};
static const lte king_h8[] = {
    (lte)3, (lte)g7, (lte)g8, (lte)h7
};

const lte *king_lookup[] = {
    king_a8, king_b8, king_c8, king_d8, king_e8, king_f8, king_g8, king_h8,
    king_a7, king_b7, king_c7, king_d7, king_e7, king_f7, king_g7, king_h7,
    king_a6, king_b6, king_c6, king_d6, king_e6, king_f6, king_g6, king_h6,
    king_a5, king_b5, king_c5, king_d5, king_e5, king_f5, king_g5, king_h5,
    king_a4, king_b4, king_c4, king_d4, king_e4, king_f4, king_g4, king_h4,
    king_a3, king_b3, king_c3, king_d3, king_e3, king_f3, king_g3, king_h3,
    king_a2, king_b2, king_c2, king_d2, king_e2, king_f2, king_g2, king_h2,
    king_a1, king_b1, king_c1, king_d1, king_e1, king_f1, king_g1, king_h1,
};

// White pawn, capture ray followed by advance ray
static const lte pawn_white_a1[] = {
    (lte)1, (lte)b2,
    (lte)1, (lte)a2
};
static const lte pawn_white_a2[] = {
    (lte)1, (lte)b3,
    (lte)2, (lte)a3, (lte)a4
};
static const lte pawn_white_a3[] = {
    (lte)1, (lte)b4,
    (lte)1, (lte)a4
};
static const lte pawn_white_a4[] = {
    (lte)1, (lte)b5,
    (lte)1, (lte)a5
};
static const lte pawn_white_a5[] = {
    (lte)1, (lte)b6,
    (lte)1, (lte)a6
};
static const lte pawn_white_a6[] = {
    (lte)1, (lte)b7,
    (lte)1, (lte)a7
};
static const lte pawn_white_a7[] = {
    (lte)1, (lte)b8,
    (lte)1, (lte)a8
};
static const lte pawn_white_a8[] = {
    (lte)0,
    (lte)0
};
static const lte pawn_white_b1[] = {
    (lte)2, (lte)a2, (lte)c2,
    (lte)1, (lte)b2
};
static const lte pawn_white_b2[] = {
    (lte)2, (lte)a3, (lte)c3,
    (lte)2, (lte)b3, (lte)b4
};
static const lte pawn_white_b3[] = {
    (lte)2, (lte)a4, (lte)c4,
    (lte)1, (lte)b4
};
static const lte pawn_white_b4[] = {
    (lte)2, (lte)a5, (lte)c5,
    (lte)1, (lte)b5
};
static const lte pawn_white_b5[] = {
    (lte)2, (lte)a6, (lte)c6,
    (lte)1, (lte)b6
};
static const lte pawn_white_b6[] = {
    (lte)2, (lte)a7, (lte)c7,
    (lte)1, (lte)b7
};
static const lte pawn_white_b7[] = {
    (lte)2, (lte)a8, (lte)c8,
    (lte)1, (lte)b8
};
static const lte pawn_white_b8[] = {
    (lte)0,
    (lte)0
};
static const lte pawn_white_c1[] = {
    (lte)2, (lte)b2, (lte)d2,
    (lte)1, (lte)c2
};
static const lte pawn_white_c2[] = {
    (lte)2, (lte)b3, (lte)d3,
    (lte)2, (lte)c3, (lte)c4
};
static const lte pawn_white_c3[] = {
    (lte)2, (lte)b4, (lte)d4,
    (lte)1, (lte)c4
};
static const lte pawn_white_c4[] = {
    (lte)2, (lte)b5, (lte)d5,
    (lte)1, (lte)c5
};
static const lte pawn_white_c5[] = {
    (lte)2, (lte)b6, (lte)d6,
    (lte)1, (lte)c6
};
static const lte pawn_white_c6[] = {
    (lte)2, (lte)b7, (lte)d7,
    (lte)1, (lte)c7
};
static const lte pawn_white_c7[] = {
    (lte)2, (lte)b8, (lte)d8,
    (lte)1, (lte)c8
};
static const lte pawn_white_c8[] = {
    (lte)0,
    (lte)0
};
static const lte pawn_white_d1[] = {
    (lte)2, (lte)c2, (lte)e2,
    (lte)1, (lte)d2
};
static const lte pawn_white_d2[] = {
    (lte)2, (lte)c3, (lte)e3,
    (lte)2, (lte)d3, (lte)d4
};
static const lte pawn_white_d3[] = {
    (lte)2, (lte)c4, (lte)e4,
    (lte)1, (lte)d4
};
static const lte pawn_white_d4[] = {
    (lte)2, (lte)c5, (lte)e5,
    (lte)1, (lte)d5
};
static const lte pawn_white_d5[] = {
    (lte)2, (lte)c6, (lte)e6,
    (lte)1, (lte)d6
};
static const lte pawn_white_d6[] = {
    (lte)2, (lte)c7, (lte)e7,
    (lte)1, (lte)d7
};
static const lte pawn_white_d7[] = {
    (lte)2, (lte)c8, (lte)e8,
    (lte)1, (lte)d8
};
static const lte pawn_white_d8[] = {
    (lte)0,
    (lte)0
};
static const lte pawn_white_e1[] = {
    (lte)2, (lte)d2, (lte)f2,
    (lte)1, (lte)e2
};
static const lte pawn_white_e2[] = {
    (lte)2, (lte)d3, (lte)f3,
    (lte)2, (lte)e3, (lte)e4
};
static const lte pawn_white_e3[] = {
    (lte)2, (lte)d4, (lte)f4,
    (lte)1, (lte)e4
};
static const lte pawn_white_e4[] = {
    (lte)2, (lte)d5, (lte)f5,
    (lte)1, (lte)e5
};
static const lte pawn_white_e5[] = {
    (lte)2, (lte)d6, (lte)f6,
    (lte)1, (lte)e6
};
static const lte pawn_white_e6[] = {
    (lte)2, (lte)d7, (lte)f7,
    (lte)1, (lte)e7
};
static const lte pawn_white_e7[] = {
    (lte)2, (lte)d8, (lte)f8,
    (lte)1, (lte)e8
};
static const lte pawn_white_e8[] = {
    (lte)0,
    (lte)0
};
static const lte pawn_white_f1[] = {
    (lte)2, (lte)e2, (lte)g2,
    (lte)1, (lte)f2
};
static const lte pawn_white_f2[] = {
    (lte)2, (lte)e3, (lte)g3,
    (lte)2, (lte)f3, (lte)f4
};
static const lte pawn_white_f3[] = {
    (lte)2, (lte)e4, (lte)g4,
    (lte)1, (lte)f4
};
static const lte pawn_white_f4[] = {
    (lte)2, (lte)e5, (lte)g5,
    (lte)1, (lte)f5
};
static const lte pawn_white_f5[] = {
    (lte)2, (lte)e6, (lte)g6,
    (lte)1, (lte)f6
};
static const lte pawn_white_f6[] = {
    (lte)2, (lte)e7, (lte)g7,
    (lte)1, (lte)f7
};
static const lte pawn_white_f7[] = {
    (lte)2, (lte)e8, (lte)g8,
    (lte)1, (lte)f8
};
static const lte pawn_white_f8[] = {
    (lte)0,
    (lte)0
};
static const lte pawn_white_g1[] = {
    (lte)2, (lte)f2, (lte)h2,
    (lte)1, (lte)g2
};
static const lte pawn_white_g2[] = {
    (lte)2, (lte)f3, (lte)h3,
    (lte)2, (lte)g3, (lte)g4
};
static const lte pawn_white_g3[] = {
    (lte)2, (lte)f4, (lte)h4,
    (lte)1, (lte)g4
};
static const lte pawn_white_g4[] = {
    (lte)2, (lte)f5, (lte)h5,
    (lte)1, (lte)g5
};
static const lte pawn_white_g5[] = {
    (lte)2, (lte)f6, (lte)h6,
    (lte)1, (lte)g6
};
static const lte pawn_white_g6[] = {
    (lte)2, (lte)f7, (lte)h7,
    (lte)1, (lte)g7
};
static const lte pawn_white_g7[] = {
    (lte)2, (lte)f8, (lte)h8,
    (lte)1, (lte)g8
};
static const lte pawn_white_g8[] = {
    (lte)0,
    (lte)0
};
static const lte pawn_white_h1[] = {
    (lte)1, (lte)g2,
    (lte)1, (lte)h2
};
static const lte pawn_white_h2[] = {
    (lte)1, (lte)g3,
    (lte)2, (lte)h3, (lte)h4
};
static const lte pawn_white_h3[] = {
    (lte)1, (lte)g4,
    (lte)1, (lte)h4
};
static const lte pawn_white_h4[] = {
    (lte)1, (lte)g5,
    (lte)1, (lte)h5
};
static const lte pawn_white_h5[] = {
    (lte)1, (lte)g6,
    (lte)1, (lte)h6
};
static const lte pawn_white_h6[] = {
    (lte)1, (lte)g7,
    (lte)1, (lte)h7
};
static const lte pawn_white_h7[] = {
    (lte)1, (lte)g8,
    (lte)1, (lte)h8
};
static const lte pawn_white_h8[] = {
    (lte)0,
    (lte)0
};

const lte* pawn_white_lookup[] = {
    pawn_white_a8,
    pawn_white_b8,
    pawn_white_c8,
    pawn_white_d8,
    pawn_white_e8,
    pawn_white_f8,
    pawn_white_g8,
    pawn_white_h8,
    pawn_white_a7,
    pawn_white_b7,
    pawn_white_c7,
    pawn_white_d7,
    pawn_white_e7,
    pawn_white_f7,
    pawn_white_g7,
    pawn_white_h7,
    pawn_white_a6,
    pawn_white_b6,
    pawn_white_c6,
    pawn_white_d6,
    pawn_white_e6,
    pawn_white_f6,
    pawn_white_g6,
    pawn_white_h6,
    pawn_white_a5,
    pawn_white_b5,
    pawn_white_c5,
    pawn_white_d5,
    pawn_white_e5,
    pawn_white_f5,
    pawn_white_g5,
    pawn_white_h5,
    pawn_white_a4,
    pawn_white_b4,
    pawn_white_c4,
    pawn_white_d4,
    pawn_white_e4,
    pawn_white_f4,
    pawn_white_g4,
    pawn_white_h4,
    pawn_white_a3,
    pawn_white_b3,
    pawn_white_c3,
    pawn_white_d3,
    pawn_white_e3,
    pawn_white_f3,
    pawn_white_g3,
    pawn_white_h3,
    pawn_white_a2,
    pawn_white_b2,
    pawn_white_c2,
    pawn_white_d2,
    pawn_white_e2,
    pawn_white_f2,
    pawn_white_g2,
    pawn_white_h2,
    pawn_white_a1,
    pawn_white_b1,
    pawn_white_c1,
    pawn_white_d1,
    pawn_white_e1,
    pawn_white_f1,
    pawn_white_g1,
    pawn_white_h1,
};

// Black pawn, capture ray followed by advance ray
static const lte pawn_black_a1[] = {
    (lte)0,
    (lte)0
};
static const lte pawn_black_a2[] = {
    (lte)1, (lte)b1,
    (lte)1, (lte)a1
};
static const lte pawn_black_a3[] = {
    (lte)1, (lte)b2,
    (lte)1, (lte)a2
};
static const lte pawn_black_a4[] = {
    (lte)1, (lte)b3,
    (lte)1, (lte)a3
};
static const lte pawn_black_a5[] = {
    (lte)1, (lte)b4,
    (lte)1, (lte)a4
};
static const lte pawn_black_a6[] = {
    (lte)1, (lte)b5,
    (lte)1, (lte)a5
};
static const lte pawn_black_a7[] = {
    (lte)1, (lte)b6,
    (lte)2, (lte)a6, (lte)a5
};
static const lte pawn_black_a8[] = {
    (lte)1, (lte)b7,
    (lte)1, (lte)a7
};
static const lte pawn_black_b1[] = {
    (lte)0,
    (lte)0
};
static const lte pawn_black_b2[] = {
    (lte)2, (lte)a1, (lte)c1,
    (lte)1, (lte)b1
};
static const lte pawn_black_b3[] = {
    (lte)2, (lte)a2, (lte)c2,
    (lte)1, (lte)b2
};
static const lte pawn_black_b4[] = {
    (lte)2, (lte)a3, (lte)c3,
    (lte)1, (lte)b3
};
static const lte pawn_black_b5[] = {
    (lte)2, (lte)a4, (lte)c4,
    (lte)1, (lte)b4
};
static const lte pawn_black_b6[] = {
    (lte)2, (lte)a5, (lte)c5,
    (lte)1, (lte)b5
};
static const lte pawn_black_b7[] = {
    (lte)2, (lte)a6, (lte)c6,
    (lte)2, (lte)b6, (lte)b5
};
static const lte pawn_black_b8[] = {
    (lte)2, (lte)a7, (lte)c7,
    (lte)1, (lte)b7
};
static const lte pawn_black_c1[] = {
    (lte)0,
    (lte)0
};
static const lte pawn_black_c2[] = {
    (lte)2, (lte)b1, (lte)d1,
    (lte)1, (lte)c1
};
static const lte pawn_black_c3[] = {
    (lte)2, (lte)b2, (lte)d2,
    (lte)1, (lte)c2
};
static const lte pawn_black_c4[] = {
    (lte)2, (lte)b3, (lte)d3,
    (lte)1, (lte)c3
};
static const lte pawn_black_c5[] = {
    (lte)2, (lte)b4, (lte)d4,
    (lte)1, (lte)c4
};
static const lte pawn_black_c6[] = {
    (lte)2, (lte)b5, (lte)d5,
    (lte)1, (lte)c5
};
static const lte pawn_black_c7[] = {
    (lte)2, (lte)b6, (lte)d6,
    (lte)2, (lte)c6, (lte)c5
};
static const lte pawn_black_c8[] = {
    (lte)2, (lte)b7, (lte)d7,
    (lte)1, (lte)c7
};
static const lte pawn_black_d1[] = {
    (lte)0,
    (lte)0
};
static const lte pawn_black_d2[] = {
    (lte)2, (lte)c1, (lte)e1,
    (lte)1, (lte)d1
};
static const lte pawn_black_d3[] = {
    (lte)2, (lte)c2, (lte)e2,
    (lte)1, (lte)d2
};
static const lte pawn_black_d4[] = {
    (lte)2, (lte)c3, (lte)e3,
    (lte)1, (lte)d3
};
static const lte pawn_black_d5[] = {
    (lte)2, (lte)c4, (lte)e4,
    (lte)1, (lte)d4
};
static const lte pawn_black_d6[] = {
    (lte)2, (lte)c5, (lte)e5,
    (lte)1, (lte)d5
};
static const lte pawn_black_d7[] = {
    (lte)2, (lte)c6, (lte)e6,
    (lte)2, (lte)d6, (lte)d5
};
static const lte pawn_black_d8[] = {
    (lte)2, (lte)c7, (lte)e7,
    (lte)1, (lte)d7
};
static const lte pawn_black_e1[] = {
    (lte)0,
    (lte)0
};
static const lte pawn_black_e2[] = {
    (lte)2, (lte)d1, (lte)f1,
    (lte)1, (lte)e1
};
static const lte pawn_black_e3[] = {
    (lte)2, (lte)d2, (lte)f2,
    (lte)1, (lte)e2
};
static const lte pawn_black_e4[] = {
    (lte)2, (lte)d3, (lte)f3,
    (lte)1, (lte)e3
};
static const lte pawn_black_e5[] = {
    (lte)2, (lte)d4, (lte)f4,
    (lte)1, (lte)e4
};
static const lte pawn_black_e6[] = {
    (lte)2, (lte)d5, (lte)f5,
    (lte)1, (lte)e5
};
static const lte pawn_black_e7[] = {
    (lte)2, (lte)d6, (lte)f6,
    (lte)2, (lte)e6, (lte)e5
};
static const lte pawn_black_e8[] = {
    (lte)2, (lte)d7, (lte)f7,
    (lte)1, (lte)e7
};
static const lte pawn_black_f1[] = {
    (lte)0,
    (lte)0
};
static const lte pawn_black_f2[] = {
    (lte)2, (lte)e1, (lte)g1,
    (lte)1, (lte)f1
};
static const lte pawn_black_f3[] = {
    (lte)2, (lte)e2, (lte)g2,
    (lte)1, (lte)f2
};
static const lte pawn_black_f4[] = {
    (lte)2, (lte)e3, (lte)g3,
    (lte)1, (lte)f3
};
static const lte pawn_black_f5[] = {
    (lte)2, (lte)e4, (lte)g4,
    (lte)1, (lte)f4
};
static const lte pawn_black_f6[] = {
    (lte)2, (lte)e5, (lte)g5,
    (lte)1, (lte)f5
};
static const lte pawn_black_f7[] = {
    (lte)2, (lte)e6, (lte)g6,
    (lte)2, (lte)f6, (lte)f5
};
static const lte pawn_black_f8[] = {
    (lte)2, (lte)e7, (lte)g7,
    (lte)1, (lte)f7
};
static const lte pawn_black_g1[] = {
    (lte)0,
    (lte)0
};
static const lte pawn_black_g2[] = {
    (lte)2, (lte)f1, (lte)h1,
    (lte)1, (lte)g1
};
static const lte pawn_black_g3[] = {
    (lte)2, (lte)f2, (lte)h2,
    (lte)1, (lte)g2
};
static const lte pawn_black_g4[] = {
    (lte)2, (lte)f3, (lte)h3,
    (lte)1, (lte)g3
};
static const lte pawn_black_g5[] = {
    (lte)2, (lte)f4, (lte)h4,
    (lte)1, (lte)g4
};
static const lte pawn_black_g6[] = {
    (lte)2, (lte)f5, (lte)h5,
    (lte)1, (lte)g5
};
static const lte pawn_black_g7[] = {
    (lte)2, (lte)f6, (lte)h6,
    (lte)2, (lte)g6, (lte)g5
};
static const lte pawn_black_g8[] = {
    (lte)2, (lte)f7, (lte)h7,
    (lte)1, (lte)g7
};
static const lte pawn_black_h1[] = {
    (lte)0,
    (lte)0
};
static const lte pawn_black_h2[] = {
    (lte)1, (lte)g1,
    (lte)1, (lte)h1
};
static const lte pawn_black_h3[] = {
    (lte)1, (lte)g2,
    (lte)1, (lte)h2
};
static const lte pawn_black_h4[] = {
    (lte)1, (lte)g3,
    (lte)1, (lte)h3
};
static const lte pawn_black_h5[] = {
    (lte)1, (lte)g4,
    (lte)1, (lte)h4
};
static const lte pawn_black_h6[] = {
    (lte)1, (lte)g5,
    (lte)1, (lte)h5
};
static const lte pawn_black_h7[] = {
    (lte)1, (lte)g6,
    (lte)2, (lte)h6, (lte)h5
};
static const lte pawn_black_h8[] = {
    (lte)1, (lte)g7,
    (lte)1, (lte)h7
};

const lte *pawn_black_lookup[] = {
    pawn_black_a8,
    pawn_black_b8,
    pawn_black_c8,
    pawn_black_d8,
    pawn_black_e8,
    pawn_black_f8,
    pawn_black_g8,
    pawn_black_h8,
    pawn_black_a7,
    pawn_black_b7,
    pawn_black_c7,
    pawn_black_d7,
    pawn_black_e7,
    pawn_black_f7,
    pawn_black_g7,
    pawn_black_h7,
    pawn_black_a6,
    pawn_black_b6,
    pawn_black_c6,
    pawn_black_d6,
    pawn_black_e6,
    pawn_black_f6,
    pawn_black_g6,
    pawn_black_h6,
    pawn_black_a5,
    pawn_black_b5,
    pawn_black_c5,
    pawn_black_d5,
    pawn_black_e5,
    pawn_black_f5,
    pawn_black_g5,
    pawn_black_h5,
    pawn_black_a4,
    pawn_black_b4,
    pawn_black_c4,
    pawn_black_d4,
    pawn_black_e4,
    pawn_black_f4,
    pawn_black_g4,
    pawn_black_h4,
    pawn_black_a3,
    pawn_black_b3,
    pawn_black_c3,
    pawn_black_d3,
    pawn_black_e3,
    pawn_black_f3,
    pawn_black_g3,
    pawn_black_h3,
    pawn_black_a2,
    pawn_black_b2,
    pawn_black_c2,
    pawn_black_d2,
    pawn_black_e2,
    pawn_black_f2,
    pawn_black_g2,
    pawn_black_h2,
    pawn_black_a1,
    pawn_black_b1,
    pawn_black_c1,
    pawn_black_d1,
    pawn_black_e1,
    pawn_black_f1,
    pawn_black_g1,
    pawn_black_h1,
};

// Attack from up to 2 black pawns on a white piece
static const lte pawn_attacks_white_a1[] = {
    (lte)1, (lte)b2
};
static const lte pawn_attacks_white_a2[] = {
    (lte)1, (lte)b3
};
static const lte pawn_attacks_white_a3[] = {
    (lte)1, (lte)b4
};
static const lte pawn_attacks_white_a4[] = {
    (lte)1, (lte)b5
};
static const lte pawn_attacks_white_a5[] = {
    (lte)1, (lte)b6
};
static const lte pawn_attacks_white_a6[] = {
    (lte)1, (lte)b7
};
static const lte pawn_attacks_white_a7[] = {
    (lte)1, (lte)b8
};
static const lte pawn_attacks_white_a8[] = {
    (lte)0
};
static const lte pawn_attacks_white_b1[] = {
    (lte)2, (lte)a2, (lte)c2
};
static const lte pawn_attacks_white_b2[] = {
    (lte)2, (lte)a3, (lte)c3
};
static const lte pawn_attacks_white_b3[] = {
    (lte)2, (lte)a4, (lte)c4
};
static const lte pawn_attacks_white_b4[] = {
    (lte)2, (lte)a5, (lte)c5
};
static const lte pawn_attacks_white_b5[] = {
    (lte)2, (lte)a6, (lte)c6
};
static const lte pawn_attacks_white_b6[] = {
    (lte)2, (lte)a7, (lte)c7
};
static const lte pawn_attacks_white_b7[] = {
    (lte)2, (lte)a8, (lte)c8
};
static const lte pawn_attacks_white_b8[] = {
    (lte)0
};
static const lte pawn_attacks_white_c1[] = {
    (lte)2, (lte)b2, (lte)d2
};
static const lte pawn_attacks_white_c2[] = {
    (lte)2, (lte)b3, (lte)d3
};
static const lte pawn_attacks_white_c3[] = {
    (lte)2, (lte)b4, (lte)d4
};
static const lte pawn_attacks_white_c4[] = {
    (lte)2, (lte)b5, (lte)d5
};
static const lte pawn_attacks_white_c5[] = {
    (lte)2, (lte)b6, (lte)d6
};
static const lte pawn_attacks_white_c6[] = {
    (lte)2, (lte)b7, (lte)d7
};
static const lte pawn_attacks_white_c7[] = {
    (lte)2, (lte)b8, (lte)d8
};
static const lte pawn_attacks_white_c8[] = {
    (lte)0
};
static const lte pawn_attacks_white_d1[] = {
    (lte)2, (lte)c2, (lte)e2
};
static const lte pawn_attacks_white_d2[] = {
    (lte)2, (lte)c3, (lte)e3
};
static const lte pawn_attacks_white_d3[] = {
    (lte)2, (lte)c4, (lte)e4
};
static const lte pawn_attacks_white_d4[] = {
    (lte)2, (lte)c5, (lte)e5
};
static const lte pawn_attacks_white_d5[] = {
    (lte)2, (lte)c6, (lte)e6
};
static const lte pawn_attacks_white_d6[] = {
    (lte)2, (lte)c7, (lte)e7
};
static const lte pawn_attacks_white_d7[] = {
    (lte)2, (lte)c8, (lte)e8
};
static const lte pawn_attacks_white_d8[] = {
    (lte)0
};
static const lte pawn_attacks_white_e1[] = {
    (lte)2, (lte)d2, (lte)f2
};
static const lte pawn_attacks_white_e2[] = {
    (lte)2, (lte)d3, (lte)f3
};
static const lte pawn_attacks_white_e3[] = {
    (lte)2, (lte)d4, (lte)f4
};
static const lte pawn_attacks_white_e4[] = {
    (lte)2, (lte)d5, (lte)f5
};
static const lte pawn_attacks_white_e5[] = {
    (lte)2, (lte)d6, (lte)f6
};
static const lte pawn_attacks_white_e6[] = {
    (lte)2, (lte)d7, (lte)f7
};
static const lte pawn_attacks_white_e7[] = {
    (lte)2, (lte)d8, (lte)f8
};
static const lte pawn_attacks_white_e8[] = {
    (lte)0
};
static const lte pawn_attacks_white_f1[] = {
    (lte)2, (lte)e2, (lte)g2
};
static const lte pawn_attacks_white_f2[] = {
    (lte)2, (lte)e3, (lte)g3
};
static const lte pawn_attacks_white_f3[] = {
    (lte)2, (lte)e4, (lte)g4
};
static const lte pawn_attacks_white_f4[] = {
    (lte)2, (lte)e5, (lte)g5
};
static const lte pawn_attacks_white_f5[] = {
    (lte)2, (lte)e6, (lte)g6
};
static const lte pawn_attacks_white_f6[] = {
    (lte)2, (lte)e7, (lte)g7
};
static const lte pawn_attacks_white_f7[] = {
    (lte)2, (lte)e8, (lte)g8
};
static const lte pawn_attacks_white_f8[] = {
    (lte)0
};
static const lte pawn_attacks_white_g1[] = {
    (lte)2, (lte)f2, (lte)h2
};
static const lte pawn_attacks_white_g2[] = {
    (lte)2, (lte)f3, (lte)h3
};
static const lte pawn_attacks_white_g3[] = {
    (lte)2, (lte)f4, (lte)h4
};
static const lte pawn_attacks_white_g4[] = {
    (lte)2, (lte)f5, (lte)h5
};
static const lte pawn_attacks_white_g5[] = {
    (lte)2, (lte)f6, (lte)h6
};
static const lte pawn_attacks_white_g6[] = {
    (lte)2, (lte)f7, (lte)h7
};
static const lte pawn_attacks_white_g7[] = {
    (lte)2, (lte)f8, (lte)h8
};
static const lte pawn_attacks_white_g8[] = {
    (lte)0
};
static const lte pawn_attacks_white_h1[] = {
    (lte)1, (lte)g2
};
static const lte pawn_attacks_white_h2[] = {
    (lte)1, (lte)g3
};
static const lte pawn_attacks_white_h3[] = {
    (lte)1, (lte)g4
};
static const lte pawn_attacks_white_h4[] = {
    (lte)1, (lte)g5
};
static const lte pawn_attacks_white_h5[] = {
    (lte)1, (lte)g6
};
static const lte pawn_attacks_white_h6[] = {
    (lte)1, (lte)g7
};
static const lte pawn_attacks_white_h7[] = {
    (lte)1, (lte)g8
};
static const lte pawn_attacks_white_h8[] = {
    (lte)0
};

// pawn_attacks_white_lookup
const lte *pawn_attacks_white_lookup[] = {
    pawn_attacks_white_a8,
    pawn_attacks_white_b8,
    pawn_attacks_white_c8,
    pawn_attacks_white_d8,
    pawn_attacks_white_e8,
    pawn_attacks_white_f8,
    pawn_attacks_white_g8,
    pawn_attacks_white_h8,
    pawn_attacks_white_a7,
    pawn_attacks_white_b7,
    pawn_attacks_white_c7,
    pawn_attacks_white_d7,
    pawn_attacks_white_e7,
    pawn_attacks_white_f7,
    pawn_attacks_white_g7,
    pawn_attacks_white_h7,
    pawn_attacks_white_a6,
    pawn_attacks_white_b6,
    pawn_attacks_white_c6,
    pawn_attacks_white_d6,
    pawn_attacks_white_e6,
    pawn_attacks_white_f6,
    pawn_attacks_white_g6,
    pawn_attacks_white_h6,
    pawn_attacks_white_a5,
    pawn_attacks_white_b5,
    pawn_attacks_white_c5,
    pawn_attacks_white_d5,
    pawn_attacks_white_e5,
    pawn_attacks_white_f5,
    pawn_attacks_white_g5,
    pawn_attacks_white_h5,
    pawn_attacks_white_a4,
    pawn_attacks_white_b4,
    pawn_attacks_white_c4,
    pawn_attacks_white_d4,
    pawn_attacks_white_e4,
    pawn_attacks_white_f4,
    pawn_attacks_white_g4,
    pawn_attacks_white_h4,
    pawn_attacks_white_a3,
    pawn_attacks_white_b3,
    pawn_attacks_white_c3,
    pawn_attacks_white_d3,
    pawn_attacks_white_e3,
    pawn_attacks_white_f3,
    pawn_attacks_white_g3,
    pawn_attacks_white_h3,
    pawn_attacks_white_a2,
    pawn_attacks_white_b2,
    pawn_attacks_white_c2,
    pawn_attacks_white_d2,
    pawn_attacks_white_e2,
    pawn_attacks_white_f2,
    pawn_attacks_white_g2,
    pawn_attacks_white_h2,
    pawn_attacks_white_a1,
    pawn_attacks_white_b1,
    pawn_attacks_white_c1,
    pawn_attacks_white_d1,
    pawn_attacks_white_e1,
    pawn_attacks_white_f1,
    pawn_attacks_white_g1,
    pawn_attacks_white_h1,
};

// Attack from up to 2 white pawns on a black piece
static const lte pawn_attacks_black_a1[] = {
    (lte)0
};
static const lte pawn_attacks_black_a2[] = {
    (lte)1, (lte)b1
};
static const lte pawn_attacks_black_a3[] = {
    (lte)1, (lte)b2
};
static const lte pawn_attacks_black_a4[] = {
    (lte)1, (lte)b3
};
static const lte pawn_attacks_black_a5[] = {
    (lte)1, (lte)b4
};
static const lte pawn_attacks_black_a6[] = {
    (lte)1, (lte)b5
};
static const lte pawn_attacks_black_a7[] = {
    (lte)1, (lte)b6
};
static const lte pawn_attacks_black_a8[] = {
    (lte)1, (lte)b7
};
static const lte pawn_attacks_black_b1[] = {
    (lte)0
};
static const lte pawn_attacks_black_b2[] = {
    (lte)2, (lte)a1, (lte)c1
};
static const lte pawn_attacks_black_b3[] = {
    (lte)2, (lte)a2, (lte)c2
};
static const lte pawn_attacks_black_b4[] = {
    (lte)2, (lte)a3, (lte)c3
};
static const lte pawn_attacks_black_b5[] = {
    (lte)2, (lte)a4, (lte)c4
};
static const lte pawn_attacks_black_b6[] = {
    (lte)2, (lte)a5, (lte)c5
};
static const lte pawn_attacks_black_b7[] = {
    (lte)2, (lte)a6, (lte)c6
};
static const lte pawn_attacks_black_b8[] = {
    (lte)2, (lte)a7, (lte)c7
};
static const lte pawn_attacks_black_c1[] = {
    (lte)0
};
static const lte pawn_attacks_black_c2[] = {
    (lte)2, (lte)b1, (lte)d1
};
static const lte pawn_attacks_black_c3[] = {
    (lte)2, (lte)b2, (lte)d2
};
static const lte pawn_attacks_black_c4[] = {
    (lte)2, (lte)b3, (lte)d3
};
static const lte pawn_attacks_black_c5[] = {
    (lte)2, (lte)b4, (lte)d4
};
static const lte pawn_attacks_black_c6[] = {
    (lte)2, (lte)b5, (lte)d5
};
static const lte pawn_attacks_black_c7[] = {
    (lte)2, (lte)b6, (lte)d6
};
static const lte pawn_attacks_black_c8[] = {
    (lte)2, (lte)b7, (lte)d7
};
static const lte pawn_attacks_black_d1[] = {
    (lte)0
};
static const lte pawn_attacks_black_d2[] = {
    (lte)2, (lte)c1, (lte)e1
};
static const lte pawn_attacks_black_d3[] = {
    (lte)2, (lte)c2, (lte)e2
};
static const lte pawn_attacks_black_d4[] = {
    (lte)2, (lte)c3, (lte)e3
};
static const lte pawn_attacks_black_d5[] = {
    (lte)2, (lte)c4, (lte)e4
};
static const lte pawn_attacks_black_d6[] = {
    (lte)2, (lte)c5, (lte)e5
};
static const lte pawn_attacks_black_d7[] = {
    (lte)2, (lte)c6, (lte)e6
};
static const lte pawn_attacks_black_d8[] = {
    (lte)2, (lte)c7, (lte)e7
};
static const lte pawn_attacks_black_e1[] = {
    (lte)0
};
static const lte pawn_attacks_black_e2[] = {
    (lte)2, (lte)d1, (lte)f1
};
static const lte pawn_attacks_black_e3[] = {
    (lte)2, (lte)d2, (lte)f2
};
static const lte pawn_attacks_black_e4[] = {
    (lte)2, (lte)d3, (lte)f3
};
static const lte pawn_attacks_black_e5[] = {
    (lte)2, (lte)d4, (lte)f4
};
static const lte pawn_attacks_black_e6[] = {
    (lte)2, (lte)d5, (lte)f5
};
static const lte pawn_attacks_black_e7[] = {
    (lte)2, (lte)d6, (lte)f6
};
static const lte pawn_attacks_black_e8[] = {
    (lte)2, (lte)d7, (lte)f7
};
static const lte pawn_attacks_black_f1[] = {
    (lte)0
};
static const lte pawn_attacks_black_f2[] = {
    (lte)2, (lte)e1, (lte)g1
};
static const lte pawn_attacks_black_f3[] = {
    (lte)2, (lte)e2, (lte)g2
};
static const lte pawn_attacks_black_f4[] = {
    (lte)2, (lte)e3, (lte)g3
};
static const lte pawn_attacks_black_f5[] = {
    (lte)2, (lte)e4, (lte)g4
};
static const lte pawn_attacks_black_f6[] = {
    (lte)2, (lte)e5, (lte)g5
};
static const lte pawn_attacks_black_f7[] = {
    (lte)2, (lte)e6, (lte)g6
};
static const lte pawn_attacks_black_f8[] = {
    (lte)2, (lte)e7, (lte)g7
};
static const lte pawn_attacks_black_g1[] = {
    (lte)0
};
static const lte pawn_attacks_black_g2[] = {
    (lte)2, (lte)f1, (lte)h1
};
static const lte pawn_attacks_black_g3[] = {
    (lte)2, (lte)f2, (lte)h2
};
static const lte pawn_attacks_black_g4[] = {
    (lte)2, (lte)f3, (lte)h3
};
static const lte pawn_attacks_black_g5[] = {
    (lte)2, (lte)f4, (lte)h4
};
static const lte pawn_attacks_black_g6[] = {
    (lte)2, (lte)f5, (lte)h5
};
static const lte pawn_attacks_black_g7[] = {
    (lte)2, (lte)f6, (lte)h6
};
static const lte pawn_attacks_black_g8[] = {
    (lte)2, (lte)f7, (lte)h7
};
static const lte pawn_attacks_black_h1[] = {
    (lte)0
};
static const lte pawn_attacks_black_h2[] = {
    (lte)1, (lte)g1
};
static const lte pawn_attacks_black_h3[] = {
    (lte)1, (lte)g2
};
static const lte pawn_attacks_black_h4[] = {
    (lte)1, (lte)g3
};
static const lte pawn_attacks_black_h5[] = {
    (lte)1, (lte)g4
};
static const lte pawn_attacks_black_h6[] = {
    (lte)1, (lte)g5
};
static const lte pawn_attacks_black_h7[] = {
    (lte)1, (lte)g6
};
static const lte pawn_attacks_black_h8[] = {
    (lte)1, (lte)g7
};

// pawn_attacks_black_lookup
const lte *pawn_attacks_black_lookup[] = {
    pawn_attacks_black_a8,
    pawn_attacks_black_b8,
    pawn_attacks_black_c8,
    pawn_attacks_black_d8,
    pawn_attacks_black_e8,
    pawn_attacks_black_f8,
    pawn_attacks_black_g8,
    pawn_attacks_black_h8,
    pawn_attacks_black_a7,
    pawn_attacks_black_b7,
    pawn_attacks_black_c7,
    pawn_attacks_black_d7,
    pawn_attacks_black_e7,
    pawn_attacks_black_f7,
    pawn_attacks_black_g7,
    pawn_attacks_black_h7,
    pawn_attacks_black_a6,
    pawn_attacks_black_b6,
    pawn_attacks_black_c6,
    pawn_attacks_black_d6,
    pawn_attacks_black_e6,
    pawn_attacks_black_f6,
    pawn_attacks_black_g6,
    pawn_attacks_black_h6,
    pawn_attacks_black_a5,
    pawn_attacks_black_b5,
    pawn_attacks_black_c5,
    pawn_attacks_black_d5,
    pawn_attacks_black_e5,
    pawn_attacks_black_f5,
    pawn_attacks_black_g5,
    pawn_attacks_black_h5,
    pawn_attacks_black_a4,
    pawn_attacks_black_b4,
    pawn_attacks_black_c4,
    pawn_attacks_black_d4,
    pawn_attacks_black_e4,
    pawn_attacks_black_f4,
    pawn_attacks_black_g4,
    pawn_attacks_black_h4,
    pawn_attacks_black_a3,
    pawn_attacks_black_b3,
    pawn_attacks_black_c3,
    pawn_attacks_black_d3,
    pawn_attacks_black_e3,
    pawn_attacks_black_f3,
    pawn_attacks_black_g3,
    pawn_attacks_black_h3,
    pawn_attacks_black_a2,
    pawn_attacks_black_b2,
    pawn_attacks_black_c2,
    pawn_attacks_black_d2,
    pawn_attacks_black_e2,
    pawn_attacks_black_f2,
    pawn_attacks_black_g2,
    pawn_attacks_black_h2,
    pawn_attacks_black_a1,
    pawn_attacks_black_b1,
    pawn_attacks_black_c1,
    pawn_attacks_black_d1,
    pawn_attacks_black_e1,
    pawn_attacks_black_f1,
    pawn_attacks_black_g1,
    pawn_attacks_black_h1,
};

// Attack from up to 8 rays on a white piece
static const lte attacks_white_a1[] = {
(lte)3
    ,(lte)7 ,(lte)b1,(lte)(K|R|Q)   ,(lte)c1,(lte)(R|Q) ,(lte)d1,(lte)(R|Q) ,(lte)e1,(lte)(R|Q) ,(lte)f1,(lte)(R|Q) ,(lte)g1,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)7 ,(lte)a2,(lte)(K|R|Q)   ,(lte)a3,(lte)(R|Q) ,(lte)a4,(lte)(R|Q) ,(lte)a5,(lte)(R|Q) ,(lte)a6,(lte)(R|Q) ,(lte)a7,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)7 ,(lte)b2,(lte)(K|P|B|Q) ,(lte)c3,(lte)(B|Q) ,(lte)d4,(lte)(B|Q) ,(lte)e5,(lte)(B|Q) ,(lte)f6,(lte)(B|Q) ,(lte)g7,(lte)(B|Q) ,(lte)h8,(lte)(B|Q)
};
static const lte attacks_white_a2[] = {
(lte)5
    ,(lte)7 ,(lte)b2,(lte)(K|R|Q)   ,(lte)c2,(lte)(R|Q) ,(lte)d2,(lte)(R|Q) ,(lte)e2,(lte)(R|Q) ,(lte)f2,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)h2,(lte)(R|Q)
    ,(lte)1 ,(lte)a1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)a3,(lte)(K|R|Q)   ,(lte)a4,(lte)(R|Q) ,(lte)a5,(lte)(R|Q) ,(lte)a6,(lte)(R|Q) ,(lte)a7,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)6 ,(lte)b3,(lte)(K|P|B|Q) ,(lte)c4,(lte)(B|Q) ,(lte)d5,(lte)(B|Q) ,(lte)e6,(lte)(B|Q) ,(lte)f7,(lte)(B|Q) ,(lte)g8,(lte)(B|Q)
    ,(lte)1 ,(lte)b1,(lte)(K|B|Q)
};
static const lte attacks_white_a3[] = {
(lte)5
    ,(lte)7 ,(lte)b3,(lte)(K|R|Q)   ,(lte)c3,(lte)(R|Q) ,(lte)d3,(lte)(R|Q) ,(lte)e3,(lte)(R|Q) ,(lte)f3,(lte)(R|Q) ,(lte)g3,(lte)(R|Q) ,(lte)h3,(lte)(R|Q)
    ,(lte)2 ,(lte)a2,(lte)(K|R|Q)   ,(lte)a1,(lte)(R|Q)
    ,(lte)5 ,(lte)a4,(lte)(K|R|Q)   ,(lte)a5,(lte)(R|Q) ,(lte)a6,(lte)(R|Q) ,(lte)a7,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)5 ,(lte)b4,(lte)(K|P|B|Q) ,(lte)c5,(lte)(B|Q) ,(lte)d6,(lte)(B|Q) ,(lte)e7,(lte)(B|Q) ,(lte)f8,(lte)(B|Q)
    ,(lte)2 ,(lte)b2,(lte)(K|B|Q)   ,(lte)c1,(lte)(B|Q)
};
static const lte attacks_white_a4[] = {
(lte)5
    ,(lte)7 ,(lte)b4,(lte)(K|R|Q)   ,(lte)c4,(lte)(R|Q) ,(lte)d4,(lte)(R|Q) ,(lte)e4,(lte)(R|Q) ,(lte)f4,(lte)(R|Q) ,(lte)g4,(lte)(R|Q) ,(lte)h4,(lte)(R|Q)
    ,(lte)3 ,(lte)a3,(lte)(K|R|Q)   ,(lte)a2,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)4 ,(lte)a5,(lte)(K|R|Q)   ,(lte)a6,(lte)(R|Q) ,(lte)a7,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)4 ,(lte)b5,(lte)(K|P|B|Q) ,(lte)c6,(lte)(B|Q) ,(lte)d7,(lte)(B|Q) ,(lte)e8,(lte)(B|Q)
    ,(lte)3 ,(lte)b3,(lte)(K|B|Q)   ,(lte)c2,(lte)(B|Q) ,(lte)d1,(lte)(B|Q)
};
static const lte attacks_white_a5[] = {
(lte)5
    ,(lte)7 ,(lte)b5,(lte)(K|R|Q)   ,(lte)c5,(lte)(R|Q) ,(lte)d5,(lte)(R|Q) ,(lte)e5,(lte)(R|Q) ,(lte)f5,(lte)(R|Q) ,(lte)g5,(lte)(R|Q) ,(lte)h5,(lte)(R|Q)
    ,(lte)4 ,(lte)a4,(lte)(K|R|Q)   ,(lte)a3,(lte)(R|Q) ,(lte)a2,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)3 ,(lte)a6,(lte)(K|R|Q)   ,(lte)a7,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)3 ,(lte)b6,(lte)(K|P|B|Q) ,(lte)c7,(lte)(B|Q) ,(lte)d8,(lte)(B|Q)
    ,(lte)4 ,(lte)b4,(lte)(K|B|Q)   ,(lte)c3,(lte)(B|Q) ,(lte)d2,(lte)(B|Q) ,(lte)e1,(lte)(B|Q)
};
static const lte attacks_white_a6[] = {
(lte)5
    ,(lte)7 ,(lte)b6,(lte)(K|R|Q)   ,(lte)c6,(lte)(R|Q) ,(lte)d6,(lte)(R|Q) ,(lte)e6,(lte)(R|Q) ,(lte)f6,(lte)(R|Q) ,(lte)g6,(lte)(R|Q) ,(lte)h6,(lte)(R|Q)
    ,(lte)5 ,(lte)a5,(lte)(K|R|Q)   ,(lte)a4,(lte)(R|Q) ,(lte)a3,(lte)(R|Q) ,(lte)a2,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)2 ,(lte)a7,(lte)(K|R|Q)   ,(lte)a8,(lte)(R|Q)
    ,(lte)2 ,(lte)b7,(lte)(K|P|B|Q) ,(lte)c8,(lte)(B|Q)
    ,(lte)5 ,(lte)b5,(lte)(K|B|Q)   ,(lte)c4,(lte)(B|Q) ,(lte)d3,(lte)(B|Q) ,(lte)e2,(lte)(B|Q) ,(lte)f1,(lte)(B|Q)
};
static const lte attacks_white_a7[] = {
(lte)5
    ,(lte)7 ,(lte)b7,(lte)(K|R|Q)   ,(lte)c7,(lte)(R|Q) ,(lte)d7,(lte)(R|Q) ,(lte)e7,(lte)(R|Q) ,(lte)f7,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)h7,(lte)(R|Q)
    ,(lte)6 ,(lte)a6,(lte)(K|R|Q)   ,(lte)a5,(lte)(R|Q) ,(lte)a4,(lte)(R|Q) ,(lte)a3,(lte)(R|Q) ,(lte)a2,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)1 ,(lte)a8,(lte)(K|R|Q)
    ,(lte)1 ,(lte)b8,(lte)(K|P|B|Q)
    ,(lte)6 ,(lte)b6,(lte)(K|B|Q)   ,(lte)c5,(lte)(B|Q) ,(lte)d4,(lte)(B|Q) ,(lte)e3,(lte)(B|Q) ,(lte)f2,(lte)(B|Q) ,(lte)g1,(lte)(B|Q)
};
static const lte attacks_white_a8[] = {
(lte)3
    ,(lte)7 ,(lte)b8,(lte)(K|R|Q)   ,(lte)c8,(lte)(R|Q) ,(lte)d8,(lte)(R|Q) ,(lte)e8,(lte)(R|Q) ,(lte)f8,(lte)(R|Q) ,(lte)g8,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)7 ,(lte)a7,(lte)(K|R|Q)   ,(lte)a6,(lte)(R|Q) ,(lte)a5,(lte)(R|Q) ,(lte)a4,(lte)(R|Q) ,(lte)a3,(lte)(R|Q) ,(lte)a2,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)7 ,(lte)b7,(lte)(K|B|Q)   ,(lte)c6,(lte)(B|Q) ,(lte)d5,(lte)(B|Q) ,(lte)e4,(lte)(B|Q) ,(lte)f3,(lte)(B|Q) ,(lte)g2,(lte)(B|Q) ,(lte)h1,(lte)(B|Q)
};
static const lte attacks_white_b1[] = {
(lte)5
    ,(lte)1 ,(lte)a1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c1,(lte)(K|R|Q)   ,(lte)d1,(lte)(R|Q) ,(lte)e1,(lte)(R|Q) ,(lte)f1,(lte)(R|Q) ,(lte)g1,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)7 ,(lte)b2,(lte)(K|R|Q)   ,(lte)b3,(lte)(R|Q) ,(lte)b4,(lte)(R|Q) ,(lte)b5,(lte)(R|Q) ,(lte)b6,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)b8,(lte)(R|Q)
    ,(lte)1 ,(lte)a2,(lte)(K|P|B|Q)
    ,(lte)6 ,(lte)c2,(lte)(K|P|B|Q) ,(lte)d3,(lte)(B|Q) ,(lte)e4,(lte)(B|Q) ,(lte)f5,(lte)(B|Q) ,(lte)g6,(lte)(B|Q) ,(lte)h7,(lte)(B|Q)
};
static const lte attacks_white_b2[] = {
(lte)8
    ,(lte)1 ,(lte)a2,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c2,(lte)(K|R|Q)   ,(lte)d2,(lte)(R|Q) ,(lte)e2,(lte)(R|Q) ,(lte)f2,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)h2,(lte)(R|Q)
    ,(lte)1 ,(lte)b1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)b3,(lte)(K|R|Q)   ,(lte)b4,(lte)(R|Q) ,(lte)b5,(lte)(R|Q) ,(lte)b6,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)b8,(lte)(R|Q)
    ,(lte)1 ,(lte)a1,(lte)(K|B|Q)
    ,(lte)1 ,(lte)a3,(lte)(K|P|B|Q)
    ,(lte)6 ,(lte)c3,(lte)(K|P|B|Q) ,(lte)d4,(lte)(B|Q) ,(lte)e5,(lte)(B|Q) ,(lte)f6,(lte)(B|Q) ,(lte)g7,(lte)(B|Q) ,(lte)h8,(lte)(B|Q)
    ,(lte)1 ,(lte)c1,(lte)(K|B|Q)
};
static const lte attacks_white_b3[] = {
(lte)8
    ,(lte)1 ,(lte)a3,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c3,(lte)(K|R|Q)   ,(lte)d3,(lte)(R|Q) ,(lte)e3,(lte)(R|Q) ,(lte)f3,(lte)(R|Q) ,(lte)g3,(lte)(R|Q) ,(lte)h3,(lte)(R|Q)
    ,(lte)2 ,(lte)b2,(lte)(K|R|Q)   ,(lte)b1,(lte)(R|Q)
    ,(lte)5 ,(lte)b4,(lte)(K|R|Q)   ,(lte)b5,(lte)(R|Q) ,(lte)b6,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)b8,(lte)(R|Q)
    ,(lte)1 ,(lte)a2,(lte)(K|B|Q)
    ,(lte)1 ,(lte)a4,(lte)(K|P|B|Q)
    ,(lte)5 ,(lte)c4,(lte)(K|P|B|Q) ,(lte)d5,(lte)(B|Q) ,(lte)e6,(lte)(B|Q) ,(lte)f7,(lte)(B|Q) ,(lte)g8,(lte)(B|Q)
    ,(lte)2 ,(lte)c2,(lte)(K|B|Q)   ,(lte)d1,(lte)(B|Q)
};
static const lte attacks_white_b4[] = {
(lte)8
    ,(lte)1 ,(lte)a4,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c4,(lte)(K|R|Q)   ,(lte)d4,(lte)(R|Q) ,(lte)e4,(lte)(R|Q) ,(lte)f4,(lte)(R|Q) ,(lte)g4,(lte)(R|Q) ,(lte)h4,(lte)(R|Q)
    ,(lte)3 ,(lte)b3,(lte)(K|R|Q)   ,(lte)b2,(lte)(R|Q) ,(lte)b1,(lte)(R|Q)
    ,(lte)4 ,(lte)b5,(lte)(K|R|Q)   ,(lte)b6,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)b8,(lte)(R|Q)
    ,(lte)1 ,(lte)a3,(lte)(K|B|Q)
    ,(lte)1 ,(lte)a5,(lte)(K|P|B|Q)
    ,(lte)4 ,(lte)c5,(lte)(K|P|B|Q) ,(lte)d6,(lte)(B|Q) ,(lte)e7,(lte)(B|Q) ,(lte)f8,(lte)(B|Q)
    ,(lte)3 ,(lte)c3,(lte)(K|B|Q)   ,(lte)d2,(lte)(B|Q) ,(lte)e1,(lte)(B|Q)
};
static const lte attacks_white_b5[] = {
(lte)8
    ,(lte)1 ,(lte)a5,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c5,(lte)(K|R|Q)   ,(lte)d5,(lte)(R|Q) ,(lte)e5,(lte)(R|Q) ,(lte)f5,(lte)(R|Q) ,(lte)g5,(lte)(R|Q) ,(lte)h5,(lte)(R|Q)
    ,(lte)4 ,(lte)b4,(lte)(K|R|Q)   ,(lte)b3,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)b1,(lte)(R|Q)
    ,(lte)3 ,(lte)b6,(lte)(K|R|Q)   ,(lte)b7,(lte)(R|Q) ,(lte)b8,(lte)(R|Q)
    ,(lte)1 ,(lte)a4,(lte)(K|B|Q)
    ,(lte)1 ,(lte)a6,(lte)(K|P|B|Q)
    ,(lte)3 ,(lte)c6,(lte)(K|P|B|Q) ,(lte)d7,(lte)(B|Q) ,(lte)e8,(lte)(B|Q)
    ,(lte)4 ,(lte)c4,(lte)(K|B|Q)   ,(lte)d3,(lte)(B|Q) ,(lte)e2,(lte)(B|Q) ,(lte)f1,(lte)(B|Q)
};
static const lte attacks_white_b6[] = {
(lte)8
    ,(lte)1 ,(lte)a6,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c6,(lte)(K|R|Q)   ,(lte)d6,(lte)(R|Q) ,(lte)e6,(lte)(R|Q) ,(lte)f6,(lte)(R|Q) ,(lte)g6,(lte)(R|Q) ,(lte)h6,(lte)(R|Q)
    ,(lte)5 ,(lte)b5,(lte)(K|R|Q)   ,(lte)b4,(lte)(R|Q) ,(lte)b3,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)b1,(lte)(R|Q)
    ,(lte)2 ,(lte)b7,(lte)(K|R|Q)   ,(lte)b8,(lte)(R|Q)
    ,(lte)1 ,(lte)a5,(lte)(K|B|Q)
    ,(lte)1 ,(lte)a7,(lte)(K|P|B|Q)
    ,(lte)2 ,(lte)c7,(lte)(K|P|B|Q) ,(lte)d8,(lte)(B|Q)
    ,(lte)5 ,(lte)c5,(lte)(K|B|Q)   ,(lte)d4,(lte)(B|Q) ,(lte)e3,(lte)(B|Q) ,(lte)f2,(lte)(B|Q) ,(lte)g1,(lte)(B|Q)
};
static const lte attacks_white_b7[] = {
(lte)8
    ,(lte)1 ,(lte)a7,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c7,(lte)(K|R|Q)   ,(lte)d7,(lte)(R|Q) ,(lte)e7,(lte)(R|Q) ,(lte)f7,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)h7,(lte)(R|Q)
    ,(lte)6 ,(lte)b6,(lte)(K|R|Q)   ,(lte)b5,(lte)(R|Q) ,(lte)b4,(lte)(R|Q) ,(lte)b3,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)b1,(lte)(R|Q)
    ,(lte)1 ,(lte)b8,(lte)(K|R|Q)
    ,(lte)1 ,(lte)a6,(lte)(K|B|Q)
    ,(lte)1 ,(lte)a8,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)c8,(lte)(K|P|B|Q)
    ,(lte)6 ,(lte)c6,(lte)(K|B|Q)   ,(lte)d5,(lte)(B|Q) ,(lte)e4,(lte)(B|Q) ,(lte)f3,(lte)(B|Q) ,(lte)g2,(lte)(B|Q) ,(lte)h1,(lte)(B|Q)
};
static const lte attacks_white_b8[] = {
(lte)5
    ,(lte)1 ,(lte)a8,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c8,(lte)(K|R|Q)   ,(lte)d8,(lte)(R|Q) ,(lte)e8,(lte)(R|Q) ,(lte)f8,(lte)(R|Q) ,(lte)g8,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)7 ,(lte)b7,(lte)(K|R|Q)   ,(lte)b6,(lte)(R|Q) ,(lte)b5,(lte)(R|Q) ,(lte)b4,(lte)(R|Q) ,(lte)b3,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)b1,(lte)(R|Q)
    ,(lte)1 ,(lte)a7,(lte)(K|B|Q)
    ,(lte)6 ,(lte)c7,(lte)(K|B|Q)   ,(lte)d6,(lte)(B|Q) ,(lte)e5,(lte)(B|Q) ,(lte)f4,(lte)(B|Q) ,(lte)g3,(lte)(B|Q) ,(lte)h2,(lte)(B|Q)
};
static const lte attacks_white_c1[] = {
(lte)5
    ,(lte)2 ,(lte)b1,(lte)(K|R|Q)   ,(lte)a1,(lte)(R|Q)
    ,(lte)5 ,(lte)d1,(lte)(K|R|Q)   ,(lte)e1,(lte)(R|Q) ,(lte)f1,(lte)(R|Q) ,(lte)g1,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)7 ,(lte)c2,(lte)(K|R|Q)   ,(lte)c3,(lte)(R|Q) ,(lte)c4,(lte)(R|Q) ,(lte)c5,(lte)(R|Q) ,(lte)c6,(lte)(R|Q) ,(lte)c7,(lte)(R|Q) ,(lte)c8,(lte)(R|Q)
    ,(lte)2 ,(lte)b2,(lte)(K|P|B|Q) ,(lte)a3,(lte)(B|Q)
    ,(lte)5 ,(lte)d2,(lte)(K|P|B|Q) ,(lte)e3,(lte)(B|Q) ,(lte)f4,(lte)(B|Q) ,(lte)g5,(lte)(B|Q) ,(lte)h6,(lte)(B|Q)
};
static const lte attacks_white_c2[] = {
(lte)8
    ,(lte)2 ,(lte)b2,(lte)(K|R|Q)   ,(lte)a2,(lte)(R|Q)
    ,(lte)5 ,(lte)d2,(lte)(K|R|Q)   ,(lte)e2,(lte)(R|Q) ,(lte)f2,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)h2,(lte)(R|Q)
    ,(lte)1 ,(lte)c1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c3,(lte)(K|R|Q)   ,(lte)c4,(lte)(R|Q) ,(lte)c5,(lte)(R|Q) ,(lte)c6,(lte)(R|Q) ,(lte)c7,(lte)(R|Q) ,(lte)c8,(lte)(R|Q)
    ,(lte)1 ,(lte)b1,(lte)(K|B|Q)
    ,(lte)2 ,(lte)b3,(lte)(K|P|B|Q) ,(lte)a4,(lte)(B|Q)
    ,(lte)5 ,(lte)d3,(lte)(K|P|B|Q) ,(lte)e4,(lte)(B|Q) ,(lte)f5,(lte)(B|Q) ,(lte)g6,(lte)(B|Q) ,(lte)h7,(lte)(B|Q)
    ,(lte)1 ,(lte)d1,(lte)(K|B|Q)
};
static const lte attacks_white_c3[] = {
(lte)8
    ,(lte)2 ,(lte)b3,(lte)(K|R|Q)   ,(lte)a3,(lte)(R|Q)
    ,(lte)5 ,(lte)d3,(lte)(K|R|Q)   ,(lte)e3,(lte)(R|Q) ,(lte)f3,(lte)(R|Q) ,(lte)g3,(lte)(R|Q) ,(lte)h3,(lte)(R|Q)
    ,(lte)2 ,(lte)c2,(lte)(K|R|Q)   ,(lte)c1,(lte)(R|Q)
    ,(lte)5 ,(lte)c4,(lte)(K|R|Q)   ,(lte)c5,(lte)(R|Q) ,(lte)c6,(lte)(R|Q) ,(lte)c7,(lte)(R|Q) ,(lte)c8,(lte)(R|Q)
    ,(lte)2 ,(lte)b2,(lte)(K|B|Q)   ,(lte)a1,(lte)(B|Q)
    ,(lte)2 ,(lte)b4,(lte)(K|P|B|Q) ,(lte)a5,(lte)(B|Q)
    ,(lte)5 ,(lte)d4,(lte)(K|P|B|Q) ,(lte)e5,(lte)(B|Q) ,(lte)f6,(lte)(B|Q) ,(lte)g7,(lte)(B|Q) ,(lte)h8,(lte)(B|Q)
    ,(lte)2 ,(lte)d2,(lte)(K|B|Q)   ,(lte)e1,(lte)(B|Q)
};
static const lte attacks_white_c4[] = {
(lte)8
    ,(lte)2 ,(lte)b4,(lte)(K|R|Q)   ,(lte)a4,(lte)(R|Q)
    ,(lte)5 ,(lte)d4,(lte)(K|R|Q)   ,(lte)e4,(lte)(R|Q) ,(lte)f4,(lte)(R|Q) ,(lte)g4,(lte)(R|Q) ,(lte)h4,(lte)(R|Q)
    ,(lte)3 ,(lte)c3,(lte)(K|R|Q)   ,(lte)c2,(lte)(R|Q) ,(lte)c1,(lte)(R|Q)
    ,(lte)4 ,(lte)c5,(lte)(K|R|Q)   ,(lte)c6,(lte)(R|Q) ,(lte)c7,(lte)(R|Q) ,(lte)c8,(lte)(R|Q)
    ,(lte)2 ,(lte)b3,(lte)(K|B|Q)   ,(lte)a2,(lte)(B|Q)
    ,(lte)2 ,(lte)b5,(lte)(K|P|B|Q) ,(lte)a6,(lte)(B|Q)
    ,(lte)4 ,(lte)d5,(lte)(K|P|B|Q) ,(lte)e6,(lte)(B|Q) ,(lte)f7,(lte)(B|Q) ,(lte)g8,(lte)(B|Q)
    ,(lte)3 ,(lte)d3,(lte)(K|B|Q)   ,(lte)e2,(lte)(B|Q) ,(lte)f1,(lte)(B|Q)
};
static const lte attacks_white_c5[] = {
(lte)8
    ,(lte)2 ,(lte)b5,(lte)(K|R|Q)   ,(lte)a5,(lte)(R|Q)
    ,(lte)5 ,(lte)d5,(lte)(K|R|Q)   ,(lte)e5,(lte)(R|Q) ,(lte)f5,(lte)(R|Q) ,(lte)g5,(lte)(R|Q) ,(lte)h5,(lte)(R|Q)
    ,(lte)4 ,(lte)c4,(lte)(K|R|Q)   ,(lte)c3,(lte)(R|Q) ,(lte)c2,(lte)(R|Q) ,(lte)c1,(lte)(R|Q)
    ,(lte)3 ,(lte)c6,(lte)(K|R|Q)   ,(lte)c7,(lte)(R|Q) ,(lte)c8,(lte)(R|Q)
    ,(lte)2 ,(lte)b4,(lte)(K|B|Q)   ,(lte)a3,(lte)(B|Q)
    ,(lte)2 ,(lte)b6,(lte)(K|P|B|Q) ,(lte)a7,(lte)(B|Q)
    ,(lte)3 ,(lte)d6,(lte)(K|P|B|Q) ,(lte)e7,(lte)(B|Q) ,(lte)f8,(lte)(B|Q)
    ,(lte)4 ,(lte)d4,(lte)(K|B|Q)   ,(lte)e3,(lte)(B|Q) ,(lte)f2,(lte)(B|Q) ,(lte)g1,(lte)(B|Q)
};
static const lte attacks_white_c6[] = {
(lte)8
    ,(lte)2 ,(lte)b6,(lte)(K|R|Q)   ,(lte)a6,(lte)(R|Q)
    ,(lte)5 ,(lte)d6,(lte)(K|R|Q)   ,(lte)e6,(lte)(R|Q) ,(lte)f6,(lte)(R|Q) ,(lte)g6,(lte)(R|Q) ,(lte)h6,(lte)(R|Q)
    ,(lte)5 ,(lte)c5,(lte)(K|R|Q)   ,(lte)c4,(lte)(R|Q) ,(lte)c3,(lte)(R|Q) ,(lte)c2,(lte)(R|Q) ,(lte)c1,(lte)(R|Q)
    ,(lte)2 ,(lte)c7,(lte)(K|R|Q)   ,(lte)c8,(lte)(R|Q)
    ,(lte)2 ,(lte)b5,(lte)(K|B|Q)   ,(lte)a4,(lte)(B|Q)
    ,(lte)2 ,(lte)b7,(lte)(K|P|B|Q) ,(lte)a8,(lte)(B|Q)
    ,(lte)2 ,(lte)d7,(lte)(K|P|B|Q) ,(lte)e8,(lte)(B|Q)
    ,(lte)5 ,(lte)d5,(lte)(K|B|Q)   ,(lte)e4,(lte)(B|Q) ,(lte)f3,(lte)(B|Q) ,(lte)g2,(lte)(B|Q) ,(lte)h1,(lte)(B|Q)
};
static const lte attacks_white_c7[] = {
(lte)8
    ,(lte)2 ,(lte)b7,(lte)(K|R|Q)   ,(lte)a7,(lte)(R|Q)
    ,(lte)5 ,(lte)d7,(lte)(K|R|Q)   ,(lte)e7,(lte)(R|Q) ,(lte)f7,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)h7,(lte)(R|Q)
    ,(lte)6 ,(lte)c6,(lte)(K|R|Q)   ,(lte)c5,(lte)(R|Q) ,(lte)c4,(lte)(R|Q) ,(lte)c3,(lte)(R|Q) ,(lte)c2,(lte)(R|Q) ,(lte)c1,(lte)(R|Q)
    ,(lte)1 ,(lte)c8,(lte)(K|R|Q)
    ,(lte)2 ,(lte)b6,(lte)(K|B|Q)   ,(lte)a5,(lte)(B|Q)
    ,(lte)1 ,(lte)b8,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)d8,(lte)(K|P|B|Q)
    ,(lte)5 ,(lte)d6,(lte)(K|B|Q)   ,(lte)e5,(lte)(B|Q) ,(lte)f4,(lte)(B|Q) ,(lte)g3,(lte)(B|Q) ,(lte)h2,(lte)(B|Q)
};
static const lte attacks_white_c8[] = {
(lte)5
    ,(lte)2 ,(lte)b8,(lte)(K|R|Q)   ,(lte)a8,(lte)(R|Q)
    ,(lte)5 ,(lte)d8,(lte)(K|R|Q)   ,(lte)e8,(lte)(R|Q) ,(lte)f8,(lte)(R|Q) ,(lte)g8,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)7 ,(lte)c7,(lte)(K|R|Q)   ,(lte)c6,(lte)(R|Q) ,(lte)c5,(lte)(R|Q) ,(lte)c4,(lte)(R|Q) ,(lte)c3,(lte)(R|Q) ,(lte)c2,(lte)(R|Q) ,(lte)c1,(lte)(R|Q)
    ,(lte)2 ,(lte)b7,(lte)(K|B|Q)   ,(lte)a6,(lte)(B|Q)
    ,(lte)5 ,(lte)d7,(lte)(K|B|Q)   ,(lte)e6,(lte)(B|Q) ,(lte)f5,(lte)(B|Q) ,(lte)g4,(lte)(B|Q) ,(lte)h3,(lte)(B|Q)
};
static const lte attacks_white_d1[] = {
(lte)5
    ,(lte)3 ,(lte)c1,(lte)(K|R|Q)   ,(lte)b1,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)4 ,(lte)e1,(lte)(K|R|Q)   ,(lte)f1,(lte)(R|Q) ,(lte)g1,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)7 ,(lte)d2,(lte)(K|R|Q)   ,(lte)d3,(lte)(R|Q) ,(lte)d4,(lte)(R|Q) ,(lte)d5,(lte)(R|Q) ,(lte)d6,(lte)(R|Q) ,(lte)d7,(lte)(R|Q) ,(lte)d8,(lte)(R|Q)
    ,(lte)3 ,(lte)c2,(lte)(K|P|B|Q) ,(lte)b3,(lte)(B|Q) ,(lte)a4,(lte)(B|Q)
    ,(lte)4 ,(lte)e2,(lte)(K|P|B|Q) ,(lte)f3,(lte)(B|Q) ,(lte)g4,(lte)(B|Q) ,(lte)h5,(lte)(B|Q)
};
static const lte attacks_white_d2[] = {
(lte)8
    ,(lte)3 ,(lte)c2,(lte)(K|R|Q)   ,(lte)b2,(lte)(R|Q) ,(lte)a2,(lte)(R|Q)
    ,(lte)4 ,(lte)e2,(lte)(K|R|Q)   ,(lte)f2,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)h2,(lte)(R|Q)
    ,(lte)1 ,(lte)d1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)d3,(lte)(K|R|Q)   ,(lte)d4,(lte)(R|Q) ,(lte)d5,(lte)(R|Q) ,(lte)d6,(lte)(R|Q) ,(lte)d7,(lte)(R|Q) ,(lte)d8,(lte)(R|Q)
    ,(lte)1 ,(lte)c1,(lte)(K|B|Q)
    ,(lte)3 ,(lte)c3,(lte)(K|P|B|Q) ,(lte)b4,(lte)(B|Q) ,(lte)a5,(lte)(B|Q)
    ,(lte)4 ,(lte)e3,(lte)(K|P|B|Q) ,(lte)f4,(lte)(B|Q) ,(lte)g5,(lte)(B|Q) ,(lte)h6,(lte)(B|Q)
    ,(lte)1 ,(lte)e1,(lte)(K|B|Q)
};
static const lte attacks_white_d3[] = {
(lte)8
    ,(lte)3 ,(lte)c3,(lte)(K|R|Q)   ,(lte)b3,(lte)(R|Q) ,(lte)a3,(lte)(R|Q)
    ,(lte)4 ,(lte)e3,(lte)(K|R|Q)   ,(lte)f3,(lte)(R|Q) ,(lte)g3,(lte)(R|Q) ,(lte)h3,(lte)(R|Q)
    ,(lte)2 ,(lte)d2,(lte)(K|R|Q)   ,(lte)d1,(lte)(R|Q)
    ,(lte)5 ,(lte)d4,(lte)(K|R|Q)   ,(lte)d5,(lte)(R|Q) ,(lte)d6,(lte)(R|Q) ,(lte)d7,(lte)(R|Q) ,(lte)d8,(lte)(R|Q)
    ,(lte)2 ,(lte)c2,(lte)(K|B|Q)   ,(lte)b1,(lte)(B|Q)
    ,(lte)3 ,(lte)c4,(lte)(K|P|B|Q) ,(lte)b5,(lte)(B|Q) ,(lte)a6,(lte)(B|Q)
    ,(lte)4 ,(lte)e4,(lte)(K|P|B|Q) ,(lte)f5,(lte)(B|Q) ,(lte)g6,(lte)(B|Q) ,(lte)h7,(lte)(B|Q)
    ,(lte)2 ,(lte)e2,(lte)(K|B|Q)   ,(lte)f1,(lte)(B|Q)
};
static const lte attacks_white_d4[] = {
(lte)8
    ,(lte)3 ,(lte)c4,(lte)(K|R|Q)   ,(lte)b4,(lte)(R|Q) ,(lte)a4,(lte)(R|Q)
    ,(lte)4 ,(lte)e4,(lte)(K|R|Q)   ,(lte)f4,(lte)(R|Q) ,(lte)g4,(lte)(R|Q) ,(lte)h4,(lte)(R|Q)
    ,(lte)3 ,(lte)d3,(lte)(K|R|Q)   ,(lte)d2,(lte)(R|Q) ,(lte)d1,(lte)(R|Q)
    ,(lte)4 ,(lte)d5,(lte)(K|R|Q)   ,(lte)d6,(lte)(R|Q) ,(lte)d7,(lte)(R|Q) ,(lte)d8,(lte)(R|Q)
    ,(lte)3 ,(lte)c3,(lte)(K|B|Q)   ,(lte)b2,(lte)(B|Q) ,(lte)a1,(lte)(B|Q)
    ,(lte)3 ,(lte)c5,(lte)(K|P|B|Q) ,(lte)b6,(lte)(B|Q) ,(lte)a7,(lte)(B|Q)
    ,(lte)4 ,(lte)e5,(lte)(K|P|B|Q) ,(lte)f6,(lte)(B|Q) ,(lte)g7,(lte)(B|Q) ,(lte)h8,(lte)(B|Q)
    ,(lte)3 ,(lte)e3,(lte)(K|B|Q)   ,(lte)f2,(lte)(B|Q) ,(lte)g1,(lte)(B|Q)
};
static const lte attacks_white_d5[] = {
(lte)8
    ,(lte)3 ,(lte)c5,(lte)(K|R|Q)   ,(lte)b5,(lte)(R|Q) ,(lte)a5,(lte)(R|Q)
    ,(lte)4 ,(lte)e5,(lte)(K|R|Q)   ,(lte)f5,(lte)(R|Q) ,(lte)g5,(lte)(R|Q) ,(lte)h5,(lte)(R|Q)
    ,(lte)4 ,(lte)d4,(lte)(K|R|Q)   ,(lte)d3,(lte)(R|Q) ,(lte)d2,(lte)(R|Q) ,(lte)d1,(lte)(R|Q)
    ,(lte)3 ,(lte)d6,(lte)(K|R|Q)   ,(lte)d7,(lte)(R|Q) ,(lte)d8,(lte)(R|Q)
    ,(lte)3 ,(lte)c4,(lte)(K|B|Q)   ,(lte)b3,(lte)(B|Q) ,(lte)a2,(lte)(B|Q)
    ,(lte)3 ,(lte)c6,(lte)(K|P|B|Q) ,(lte)b7,(lte)(B|Q) ,(lte)a8,(lte)(B|Q)
    ,(lte)3 ,(lte)e6,(lte)(K|P|B|Q) ,(lte)f7,(lte)(B|Q) ,(lte)g8,(lte)(B|Q)
    ,(lte)4 ,(lte)e4,(lte)(K|B|Q)   ,(lte)f3,(lte)(B|Q) ,(lte)g2,(lte)(B|Q) ,(lte)h1,(lte)(B|Q)
};
static const lte attacks_white_d6[] = {
(lte)8
    ,(lte)3 ,(lte)c6,(lte)(K|R|Q)   ,(lte)b6,(lte)(R|Q) ,(lte)a6,(lte)(R|Q)
    ,(lte)4 ,(lte)e6,(lte)(K|R|Q)   ,(lte)f6,(lte)(R|Q) ,(lte)g6,(lte)(R|Q) ,(lte)h6,(lte)(R|Q)
    ,(lte)5 ,(lte)d5,(lte)(K|R|Q)   ,(lte)d4,(lte)(R|Q) ,(lte)d3,(lte)(R|Q) ,(lte)d2,(lte)(R|Q) ,(lte)d1,(lte)(R|Q)
    ,(lte)2 ,(lte)d7,(lte)(K|R|Q)   ,(lte)d8,(lte)(R|Q)
    ,(lte)3 ,(lte)c5,(lte)(K|B|Q)   ,(lte)b4,(lte)(B|Q) ,(lte)a3,(lte)(B|Q)
    ,(lte)2 ,(lte)c7,(lte)(K|P|B|Q) ,(lte)b8,(lte)(B|Q)
    ,(lte)2 ,(lte)e7,(lte)(K|P|B|Q) ,(lte)f8,(lte)(B|Q)
    ,(lte)4 ,(lte)e5,(lte)(K|B|Q)   ,(lte)f4,(lte)(B|Q) ,(lte)g3,(lte)(B|Q) ,(lte)h2,(lte)(B|Q)
};
static const lte attacks_white_d7[] = {
(lte)8
    ,(lte)3 ,(lte)c7,(lte)(K|R|Q)   ,(lte)b7,(lte)(R|Q) ,(lte)a7,(lte)(R|Q)
    ,(lte)4 ,(lte)e7,(lte)(K|R|Q)   ,(lte)f7,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)h7,(lte)(R|Q)
    ,(lte)6 ,(lte)d6,(lte)(K|R|Q)   ,(lte)d5,(lte)(R|Q) ,(lte)d4,(lte)(R|Q) ,(lte)d3,(lte)(R|Q) ,(lte)d2,(lte)(R|Q) ,(lte)d1,(lte)(R|Q)
    ,(lte)1 ,(lte)d8,(lte)(K|R|Q)
    ,(lte)3 ,(lte)c6,(lte)(K|B|Q)   ,(lte)b5,(lte)(B|Q) ,(lte)a4,(lte)(B|Q)
    ,(lte)1 ,(lte)c8,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)e8,(lte)(K|P|B|Q)
    ,(lte)4 ,(lte)e6,(lte)(K|B|Q)   ,(lte)f5,(lte)(B|Q) ,(lte)g4,(lte)(B|Q) ,(lte)h3,(lte)(B|Q)
};
static const lte attacks_white_d8[] = {
(lte)5
    ,(lte)3 ,(lte)c8,(lte)(K|R|Q)   ,(lte)b8,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)4 ,(lte)e8,(lte)(K|R|Q)   ,(lte)f8,(lte)(R|Q) ,(lte)g8,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)7 ,(lte)d7,(lte)(K|R|Q)   ,(lte)d6,(lte)(R|Q) ,(lte)d5,(lte)(R|Q) ,(lte)d4,(lte)(R|Q) ,(lte)d3,(lte)(R|Q) ,(lte)d2,(lte)(R|Q) ,(lte)d1,(lte)(R|Q)
    ,(lte)3 ,(lte)c7,(lte)(K|B|Q)   ,(lte)b6,(lte)(B|Q) ,(lte)a5,(lte)(B|Q)
    ,(lte)4 ,(lte)e7,(lte)(K|B|Q)   ,(lte)f6,(lte)(B|Q) ,(lte)g5,(lte)(B|Q) ,(lte)h4,(lte)(B|Q)
};
static const lte attacks_white_e1[] = {
(lte)5
    ,(lte)4 ,(lte)d1,(lte)(K|R|Q)   ,(lte)c1,(lte)(R|Q) ,(lte)b1,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)3 ,(lte)f1,(lte)(K|R|Q)   ,(lte)g1,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)7 ,(lte)e2,(lte)(K|R|Q)   ,(lte)e3,(lte)(R|Q) ,(lte)e4,(lte)(R|Q) ,(lte)e5,(lte)(R|Q) ,(lte)e6,(lte)(R|Q) ,(lte)e7,(lte)(R|Q) ,(lte)e8,(lte)(R|Q)
    ,(lte)4 ,(lte)d2,(lte)(K|P|B|Q) ,(lte)c3,(lte)(B|Q) ,(lte)b4,(lte)(B|Q) ,(lte)a5,(lte)(B|Q)
    ,(lte)3 ,(lte)f2,(lte)(K|P|B|Q) ,(lte)g3,(lte)(B|Q) ,(lte)h4,(lte)(B|Q)
};
static const lte attacks_white_e2[] = {
(lte)8
    ,(lte)4 ,(lte)d2,(lte)(K|R|Q)   ,(lte)c2,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)a2,(lte)(R|Q)
    ,(lte)3 ,(lte)f2,(lte)(K|R|Q)   ,(lte)g2,(lte)(R|Q) ,(lte)h2,(lte)(R|Q)
    ,(lte)1 ,(lte)e1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)e3,(lte)(K|R|Q)   ,(lte)e4,(lte)(R|Q) ,(lte)e5,(lte)(R|Q) ,(lte)e6,(lte)(R|Q) ,(lte)e7,(lte)(R|Q) ,(lte)e8,(lte)(R|Q)
    ,(lte)1 ,(lte)d1,(lte)(K|B|Q)
    ,(lte)4 ,(lte)d3,(lte)(K|P|B|Q) ,(lte)c4,(lte)(B|Q) ,(lte)b5,(lte)(B|Q) ,(lte)a6,(lte)(B|Q)
    ,(lte)3 ,(lte)f3,(lte)(K|P|B|Q) ,(lte)g4,(lte)(B|Q) ,(lte)h5,(lte)(B|Q)
    ,(lte)1 ,(lte)f1,(lte)(K|B|Q)
};
static const lte attacks_white_e3[] = {
(lte)8
    ,(lte)4 ,(lte)d3,(lte)(K|R|Q)   ,(lte)c3,(lte)(R|Q) ,(lte)b3,(lte)(R|Q) ,(lte)a3,(lte)(R|Q)
    ,(lte)3 ,(lte)f3,(lte)(K|R|Q)   ,(lte)g3,(lte)(R|Q) ,(lte)h3,(lte)(R|Q)
    ,(lte)2 ,(lte)e2,(lte)(K|R|Q)   ,(lte)e1,(lte)(R|Q)
    ,(lte)5 ,(lte)e4,(lte)(K|R|Q)   ,(lte)e5,(lte)(R|Q) ,(lte)e6,(lte)(R|Q) ,(lte)e7,(lte)(R|Q) ,(lte)e8,(lte)(R|Q)
    ,(lte)2 ,(lte)d2,(lte)(K|B|Q)   ,(lte)c1,(lte)(B|Q)
    ,(lte)4 ,(lte)d4,(lte)(K|P|B|Q) ,(lte)c5,(lte)(B|Q) ,(lte)b6,(lte)(B|Q) ,(lte)a7,(lte)(B|Q)
    ,(lte)3 ,(lte)f4,(lte)(K|P|B|Q) ,(lte)g5,(lte)(B|Q) ,(lte)h6,(lte)(B|Q)
    ,(lte)2 ,(lte)f2,(lte)(K|B|Q)   ,(lte)g1,(lte)(B|Q)
};
static const lte attacks_white_e4[] = {
(lte)8
    ,(lte)4 ,(lte)d4,(lte)(K|R|Q)   ,(lte)c4,(lte)(R|Q) ,(lte)b4,(lte)(R|Q) ,(lte)a4,(lte)(R|Q)
    ,(lte)3 ,(lte)f4,(lte)(K|R|Q)   ,(lte)g4,(lte)(R|Q) ,(lte)h4,(lte)(R|Q)
    ,(lte)3 ,(lte)e3,(lte)(K|R|Q)   ,(lte)e2,(lte)(R|Q) ,(lte)e1,(lte)(R|Q)
    ,(lte)4 ,(lte)e5,(lte)(K|R|Q)   ,(lte)e6,(lte)(R|Q) ,(lte)e7,(lte)(R|Q) ,(lte)e8,(lte)(R|Q)
    ,(lte)3 ,(lte)d3,(lte)(K|B|Q)   ,(lte)c2,(lte)(B|Q) ,(lte)b1,(lte)(B|Q)
    ,(lte)4 ,(lte)d5,(lte)(K|P|B|Q) ,(lte)c6,(lte)(B|Q) ,(lte)b7,(lte)(B|Q) ,(lte)a8,(lte)(B|Q)
    ,(lte)3 ,(lte)f5,(lte)(K|P|B|Q) ,(lte)g6,(lte)(B|Q) ,(lte)h7,(lte)(B|Q)
    ,(lte)3 ,(lte)f3,(lte)(K|B|Q)   ,(lte)g2,(lte)(B|Q) ,(lte)h1,(lte)(B|Q)
};
static const lte attacks_white_e5[] = {
(lte)8
    ,(lte)4 ,(lte)d5,(lte)(K|R|Q)   ,(lte)c5,(lte)(R|Q) ,(lte)b5,(lte)(R|Q) ,(lte)a5,(lte)(R|Q)
    ,(lte)3 ,(lte)f5,(lte)(K|R|Q)   ,(lte)g5,(lte)(R|Q) ,(lte)h5,(lte)(R|Q)
    ,(lte)4 ,(lte)e4,(lte)(K|R|Q)   ,(lte)e3,(lte)(R|Q) ,(lte)e2,(lte)(R|Q) ,(lte)e1,(lte)(R|Q)
    ,(lte)3 ,(lte)e6,(lte)(K|R|Q)   ,(lte)e7,(lte)(R|Q) ,(lte)e8,(lte)(R|Q)
    ,(lte)4 ,(lte)d4,(lte)(K|B|Q)   ,(lte)c3,(lte)(B|Q) ,(lte)b2,(lte)(B|Q) ,(lte)a1,(lte)(B|Q)
    ,(lte)3 ,(lte)d6,(lte)(K|P|B|Q) ,(lte)c7,(lte)(B|Q) ,(lte)b8,(lte)(B|Q)
    ,(lte)3 ,(lte)f6,(lte)(K|P|B|Q) ,(lte)g7,(lte)(B|Q) ,(lte)h8,(lte)(B|Q)
    ,(lte)3 ,(lte)f4,(lte)(K|B|Q)   ,(lte)g3,(lte)(B|Q) ,(lte)h2,(lte)(B|Q)
};
static const lte attacks_white_e6[] = {
(lte)8
    ,(lte)4 ,(lte)d6,(lte)(K|R|Q)   ,(lte)c6,(lte)(R|Q) ,(lte)b6,(lte)(R|Q) ,(lte)a6,(lte)(R|Q)
    ,(lte)3 ,(lte)f6,(lte)(K|R|Q)   ,(lte)g6,(lte)(R|Q) ,(lte)h6,(lte)(R|Q)
    ,(lte)5 ,(lte)e5,(lte)(K|R|Q)   ,(lte)e4,(lte)(R|Q) ,(lte)e3,(lte)(R|Q) ,(lte)e2,(lte)(R|Q) ,(lte)e1,(lte)(R|Q)
    ,(lte)2 ,(lte)e7,(lte)(K|R|Q)   ,(lte)e8,(lte)(R|Q)
    ,(lte)4 ,(lte)d5,(lte)(K|B|Q)   ,(lte)c4,(lte)(B|Q) ,(lte)b3,(lte)(B|Q) ,(lte)a2,(lte)(B|Q)
    ,(lte)2 ,(lte)d7,(lte)(K|P|B|Q) ,(lte)c8,(lte)(B|Q)
    ,(lte)2 ,(lte)f7,(lte)(K|P|B|Q) ,(lte)g8,(lte)(B|Q)
    ,(lte)3 ,(lte)f5,(lte)(K|B|Q)   ,(lte)g4,(lte)(B|Q) ,(lte)h3,(lte)(B|Q)
};
static const lte attacks_white_e7[] = {
(lte)8
    ,(lte)4 ,(lte)d7,(lte)(K|R|Q)   ,(lte)c7,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)a7,(lte)(R|Q)
    ,(lte)3 ,(lte)f7,(lte)(K|R|Q)   ,(lte)g7,(lte)(R|Q) ,(lte)h7,(lte)(R|Q)
    ,(lte)6 ,(lte)e6,(lte)(K|R|Q)   ,(lte)e5,(lte)(R|Q) ,(lte)e4,(lte)(R|Q) ,(lte)e3,(lte)(R|Q) ,(lte)e2,(lte)(R|Q) ,(lte)e1,(lte)(R|Q)
    ,(lte)1 ,(lte)e8,(lte)(K|R|Q)
    ,(lte)4 ,(lte)d6,(lte)(K|B|Q)   ,(lte)c5,(lte)(B|Q) ,(lte)b4,(lte)(B|Q) ,(lte)a3,(lte)(B|Q)
    ,(lte)1 ,(lte)d8,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)f8,(lte)(K|P|B|Q)
    ,(lte)3 ,(lte)f6,(lte)(K|B|Q)   ,(lte)g5,(lte)(B|Q) ,(lte)h4,(lte)(B|Q)
};
static const lte attacks_white_e8[] = {
(lte)5
    ,(lte)4 ,(lte)d8,(lte)(K|R|Q)   ,(lte)c8,(lte)(R|Q) ,(lte)b8,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)3 ,(lte)f8,(lte)(K|R|Q)   ,(lte)g8,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)7 ,(lte)e7,(lte)(K|R|Q)   ,(lte)e6,(lte)(R|Q) ,(lte)e5,(lte)(R|Q) ,(lte)e4,(lte)(R|Q) ,(lte)e3,(lte)(R|Q) ,(lte)e2,(lte)(R|Q) ,(lte)e1,(lte)(R|Q)
    ,(lte)4 ,(lte)d7,(lte)(K|B|Q)   ,(lte)c6,(lte)(B|Q) ,(lte)b5,(lte)(B|Q) ,(lte)a4,(lte)(B|Q)
    ,(lte)3 ,(lte)f7,(lte)(K|B|Q)   ,(lte)g6,(lte)(B|Q) ,(lte)h5,(lte)(B|Q)
};
static const lte attacks_white_f1[] = {
(lte)5
    ,(lte)5 ,(lte)e1,(lte)(K|R|Q)   ,(lte)d1,(lte)(R|Q) ,(lte)c1,(lte)(R|Q) ,(lte)b1,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)2 ,(lte)g1,(lte)(K|R|Q)   ,(lte)h1,(lte)(R|Q)
    ,(lte)7 ,(lte)f2,(lte)(K|R|Q)   ,(lte)f3,(lte)(R|Q) ,(lte)f4,(lte)(R|Q) ,(lte)f5,(lte)(R|Q) ,(lte)f6,(lte)(R|Q) ,(lte)f7,(lte)(R|Q) ,(lte)f8,(lte)(R|Q)
    ,(lte)5 ,(lte)e2,(lte)(K|P|B|Q) ,(lte)d3,(lte)(B|Q) ,(lte)c4,(lte)(B|Q) ,(lte)b5,(lte)(B|Q) ,(lte)a6,(lte)(B|Q)
    ,(lte)2 ,(lte)g2,(lte)(K|P|B|Q) ,(lte)h3,(lte)(B|Q)
};
static const lte attacks_white_f2[] = {
(lte)8
    ,(lte)5 ,(lte)e2,(lte)(K|R|Q)   ,(lte)d2,(lte)(R|Q) ,(lte)c2,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)a2,(lte)(R|Q)
    ,(lte)2 ,(lte)g2,(lte)(K|R|Q)   ,(lte)h2,(lte)(R|Q)
    ,(lte)1 ,(lte)f1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)f3,(lte)(K|R|Q)   ,(lte)f4,(lte)(R|Q) ,(lte)f5,(lte)(R|Q) ,(lte)f6,(lte)(R|Q) ,(lte)f7,(lte)(R|Q) ,(lte)f8,(lte)(R|Q)
    ,(lte)1 ,(lte)e1,(lte)(K|B|Q)
    ,(lte)5 ,(lte)e3,(lte)(K|P|B|Q) ,(lte)d4,(lte)(B|Q) ,(lte)c5,(lte)(B|Q) ,(lte)b6,(lte)(B|Q) ,(lte)a7,(lte)(B|Q)
    ,(lte)2 ,(lte)g3,(lte)(K|P|B|Q) ,(lte)h4,(lte)(B|Q)
    ,(lte)1 ,(lte)g1,(lte)(K|B|Q)
};
static const lte attacks_white_f3[] = {
(lte)8
    ,(lte)5 ,(lte)e3,(lte)(K|R|Q)   ,(lte)d3,(lte)(R|Q) ,(lte)c3,(lte)(R|Q) ,(lte)b3,(lte)(R|Q) ,(lte)a3,(lte)(R|Q)
    ,(lte)2 ,(lte)g3,(lte)(K|R|Q)   ,(lte)h3,(lte)(R|Q)
    ,(lte)2 ,(lte)f2,(lte)(K|R|Q)   ,(lte)f1,(lte)(R|Q)
    ,(lte)5 ,(lte)f4,(lte)(K|R|Q)   ,(lte)f5,(lte)(R|Q) ,(lte)f6,(lte)(R|Q) ,(lte)f7,(lte)(R|Q) ,(lte)f8,(lte)(R|Q)
    ,(lte)2 ,(lte)e2,(lte)(K|B|Q)   ,(lte)d1,(lte)(B|Q)
    ,(lte)5 ,(lte)e4,(lte)(K|P|B|Q) ,(lte)d5,(lte)(B|Q) ,(lte)c6,(lte)(B|Q) ,(lte)b7,(lte)(B|Q) ,(lte)a8,(lte)(B|Q)
    ,(lte)2 ,(lte)g4,(lte)(K|P|B|Q) ,(lte)h5,(lte)(B|Q)
    ,(lte)2 ,(lte)g2,(lte)(K|B|Q)   ,(lte)h1,(lte)(B|Q)
};
static const lte attacks_white_f4[] = {
(lte)8
    ,(lte)5 ,(lte)e4,(lte)(K|R|Q)   ,(lte)d4,(lte)(R|Q) ,(lte)c4,(lte)(R|Q) ,(lte)b4,(lte)(R|Q) ,(lte)a4,(lte)(R|Q)
    ,(lte)2 ,(lte)g4,(lte)(K|R|Q)   ,(lte)h4,(lte)(R|Q)
    ,(lte)3 ,(lte)f3,(lte)(K|R|Q)   ,(lte)f2,(lte)(R|Q) ,(lte)f1,(lte)(R|Q)
    ,(lte)4 ,(lte)f5,(lte)(K|R|Q)   ,(lte)f6,(lte)(R|Q) ,(lte)f7,(lte)(R|Q) ,(lte)f8,(lte)(R|Q)
    ,(lte)3 ,(lte)e3,(lte)(K|B|Q)   ,(lte)d2,(lte)(B|Q) ,(lte)c1,(lte)(B|Q)
    ,(lte)4 ,(lte)e5,(lte)(K|P|B|Q) ,(lte)d6,(lte)(B|Q) ,(lte)c7,(lte)(B|Q) ,(lte)b8,(lte)(B|Q)
    ,(lte)2 ,(lte)g5,(lte)(K|P|B|Q) ,(lte)h6,(lte)(B|Q)
    ,(lte)2 ,(lte)g3,(lte)(K|B|Q)   ,(lte)h2,(lte)(B|Q)
};
static const lte attacks_white_f5[] = {
(lte)8
    ,(lte)5 ,(lte)e5,(lte)(K|R|Q)   ,(lte)d5,(lte)(R|Q) ,(lte)c5,(lte)(R|Q) ,(lte)b5,(lte)(R|Q) ,(lte)a5,(lte)(R|Q)
    ,(lte)2 ,(lte)g5,(lte)(K|R|Q)   ,(lte)h5,(lte)(R|Q)
    ,(lte)4 ,(lte)f4,(lte)(K|R|Q)   ,(lte)f3,(lte)(R|Q) ,(lte)f2,(lte)(R|Q) ,(lte)f1,(lte)(R|Q)
    ,(lte)3 ,(lte)f6,(lte)(K|R|Q)   ,(lte)f7,(lte)(R|Q) ,(lte)f8,(lte)(R|Q)
    ,(lte)4 ,(lte)e4,(lte)(K|B|Q)   ,(lte)d3,(lte)(B|Q) ,(lte)c2,(lte)(B|Q) ,(lte)b1,(lte)(B|Q)
    ,(lte)3 ,(lte)e6,(lte)(K|P|B|Q) ,(lte)d7,(lte)(B|Q) ,(lte)c8,(lte)(B|Q)
    ,(lte)2 ,(lte)g6,(lte)(K|P|B|Q) ,(lte)h7,(lte)(B|Q)
    ,(lte)2 ,(lte)g4,(lte)(K|B|Q)   ,(lte)h3,(lte)(B|Q)
};
static const lte attacks_white_f6[] = {
(lte)8
    ,(lte)5 ,(lte)e6,(lte)(K|R|Q)   ,(lte)d6,(lte)(R|Q) ,(lte)c6,(lte)(R|Q) ,(lte)b6,(lte)(R|Q) ,(lte)a6,(lte)(R|Q)
    ,(lte)2 ,(lte)g6,(lte)(K|R|Q)   ,(lte)h6,(lte)(R|Q)
    ,(lte)5 ,(lte)f5,(lte)(K|R|Q)   ,(lte)f4,(lte)(R|Q) ,(lte)f3,(lte)(R|Q) ,(lte)f2,(lte)(R|Q) ,(lte)f1,(lte)(R|Q)
    ,(lte)2 ,(lte)f7,(lte)(K|R|Q)   ,(lte)f8,(lte)(R|Q)
    ,(lte)5 ,(lte)e5,(lte)(K|B|Q)   ,(lte)d4,(lte)(B|Q) ,(lte)c3,(lte)(B|Q) ,(lte)b2,(lte)(B|Q) ,(lte)a1,(lte)(B|Q)
    ,(lte)2 ,(lte)e7,(lte)(K|P|B|Q) ,(lte)d8,(lte)(B|Q)
    ,(lte)2 ,(lte)g7,(lte)(K|P|B|Q) ,(lte)h8,(lte)(B|Q)
    ,(lte)2 ,(lte)g5,(lte)(K|B|Q)   ,(lte)h4,(lte)(B|Q)
};
static const lte attacks_white_f7[] = {
(lte)8
    ,(lte)5 ,(lte)e7,(lte)(K|R|Q)   ,(lte)d7,(lte)(R|Q) ,(lte)c7,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)a7,(lte)(R|Q)
    ,(lte)2 ,(lte)g7,(lte)(K|R|Q)   ,(lte)h7,(lte)(R|Q)
    ,(lte)6 ,(lte)f6,(lte)(K|R|Q)   ,(lte)f5,(lte)(R|Q) ,(lte)f4,(lte)(R|Q) ,(lte)f3,(lte)(R|Q) ,(lte)f2,(lte)(R|Q) ,(lte)f1,(lte)(R|Q)
    ,(lte)1 ,(lte)f8,(lte)(K|R|Q)
    ,(lte)5 ,(lte)e6,(lte)(K|B|Q)   ,(lte)d5,(lte)(B|Q) ,(lte)c4,(lte)(B|Q) ,(lte)b3,(lte)(B|Q) ,(lte)a2,(lte)(B|Q)
    ,(lte)1 ,(lte)e8,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)g8,(lte)(K|P|B|Q)
    ,(lte)2 ,(lte)g6,(lte)(K|B|Q)   ,(lte)h5,(lte)(B|Q)
};
static const lte attacks_white_f8[] = {
(lte)5
    ,(lte)5 ,(lte)e8,(lte)(K|R|Q)   ,(lte)d8,(lte)(R|Q) ,(lte)c8,(lte)(R|Q) ,(lte)b8,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)2 ,(lte)g8,(lte)(K|R|Q)   ,(lte)h8,(lte)(R|Q)
    ,(lte)7 ,(lte)f7,(lte)(K|R|Q)   ,(lte)f6,(lte)(R|Q) ,(lte)f5,(lte)(R|Q) ,(lte)f4,(lte)(R|Q) ,(lte)f3,(lte)(R|Q) ,(lte)f2,(lte)(R|Q) ,(lte)f1,(lte)(R|Q)
    ,(lte)5 ,(lte)e7,(lte)(K|B|Q)   ,(lte)d6,(lte)(B|Q) ,(lte)c5,(lte)(B|Q) ,(lte)b4,(lte)(B|Q) ,(lte)a3,(lte)(B|Q)
    ,(lte)2 ,(lte)g7,(lte)(K|B|Q)   ,(lte)h6,(lte)(B|Q)
};
static const lte attacks_white_g1[] = {
(lte)5
    ,(lte)6 ,(lte)f1,(lte)(K|R|Q)   ,(lte)e1,(lte)(R|Q) ,(lte)d1,(lte)(R|Q) ,(lte)c1,(lte)(R|Q) ,(lte)b1,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)1 ,(lte)h1,(lte)(K|R|Q)
    ,(lte)7 ,(lte)g2,(lte)(K|R|Q)   ,(lte)g3,(lte)(R|Q) ,(lte)g4,(lte)(R|Q) ,(lte)g5,(lte)(R|Q) ,(lte)g6,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)g8,(lte)(R|Q)
    ,(lte)6 ,(lte)f2,(lte)(K|P|B|Q) ,(lte)e3,(lte)(B|Q) ,(lte)d4,(lte)(B|Q) ,(lte)c5,(lte)(B|Q) ,(lte)b6,(lte)(B|Q) ,(lte)a7,(lte)(B|Q)
    ,(lte)1 ,(lte)h2,(lte)(K|P|B|Q)
};
static const lte attacks_white_g2[] = {
(lte)8
    ,(lte)6 ,(lte)f2,(lte)(K|R|Q)   ,(lte)e2,(lte)(R|Q) ,(lte)d2,(lte)(R|Q) ,(lte)c2,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)a2,(lte)(R|Q)
    ,(lte)1 ,(lte)h2,(lte)(K|R|Q)
    ,(lte)1 ,(lte)g1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)g3,(lte)(K|R|Q)   ,(lte)g4,(lte)(R|Q) ,(lte)g5,(lte)(R|Q) ,(lte)g6,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)g8,(lte)(R|Q)
    ,(lte)1 ,(lte)f1,(lte)(K|B|Q)
    ,(lte)6 ,(lte)f3,(lte)(K|P|B|Q) ,(lte)e4,(lte)(B|Q) ,(lte)d5,(lte)(B|Q) ,(lte)c6,(lte)(B|Q) ,(lte)b7,(lte)(B|Q) ,(lte)a8,(lte)(B|Q)
    ,(lte)1 ,(lte)h3,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)h1,(lte)(K|B|Q)
};
static const lte attacks_white_g3[] = {
(lte)8
    ,(lte)6 ,(lte)f3,(lte)(K|R|Q)   ,(lte)e3,(lte)(R|Q) ,(lte)d3,(lte)(R|Q) ,(lte)c3,(lte)(R|Q) ,(lte)b3,(lte)(R|Q) ,(lte)a3,(lte)(R|Q)
    ,(lte)1 ,(lte)h3,(lte)(K|R|Q)
    ,(lte)2 ,(lte)g2,(lte)(K|R|Q)   ,(lte)g1,(lte)(R|Q)
    ,(lte)5 ,(lte)g4,(lte)(K|R|Q)   ,(lte)g5,(lte)(R|Q) ,(lte)g6,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)g8,(lte)(R|Q)
    ,(lte)2 ,(lte)f2,(lte)(K|B|Q)   ,(lte)e1,(lte)(B|Q)
    ,(lte)5 ,(lte)f4,(lte)(K|P|B|Q) ,(lte)e5,(lte)(B|Q) ,(lte)d6,(lte)(B|Q) ,(lte)c7,(lte)(B|Q) ,(lte)b8,(lte)(B|Q)
    ,(lte)1 ,(lte)h4,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)h2,(lte)(K|B|Q)
};
static const lte attacks_white_g4[] = {
(lte)8
    ,(lte)6 ,(lte)f4,(lte)(K|R|Q)   ,(lte)e4,(lte)(R|Q) ,(lte)d4,(lte)(R|Q) ,(lte)c4,(lte)(R|Q) ,(lte)b4,(lte)(R|Q) ,(lte)a4,(lte)(R|Q)
    ,(lte)1 ,(lte)h4,(lte)(K|R|Q)
    ,(lte)3 ,(lte)g3,(lte)(K|R|Q)   ,(lte)g2,(lte)(R|Q) ,(lte)g1,(lte)(R|Q)
    ,(lte)4 ,(lte)g5,(lte)(K|R|Q)   ,(lte)g6,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)g8,(lte)(R|Q)
    ,(lte)3 ,(lte)f3,(lte)(K|B|Q)   ,(lte)e2,(lte)(B|Q) ,(lte)d1,(lte)(B|Q)
    ,(lte)4 ,(lte)f5,(lte)(K|P|B|Q) ,(lte)e6,(lte)(B|Q) ,(lte)d7,(lte)(B|Q) ,(lte)c8,(lte)(B|Q)
    ,(lte)1 ,(lte)h5,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)h3,(lte)(K|B|Q)
};
static const lte attacks_white_g5[] = {
(lte)8
    ,(lte)6 ,(lte)f5,(lte)(K|R|Q)   ,(lte)e5,(lte)(R|Q) ,(lte)d5,(lte)(R|Q) ,(lte)c5,(lte)(R|Q) ,(lte)b5,(lte)(R|Q) ,(lte)a5,(lte)(R|Q)
    ,(lte)1 ,(lte)h5,(lte)(K|R|Q)
    ,(lte)4 ,(lte)g4,(lte)(K|R|Q)   ,(lte)g3,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)g1,(lte)(R|Q)
    ,(lte)3 ,(lte)g6,(lte)(K|R|Q)   ,(lte)g7,(lte)(R|Q) ,(lte)g8,(lte)(R|Q)
    ,(lte)4 ,(lte)f4,(lte)(K|B|Q)   ,(lte)e3,(lte)(B|Q) ,(lte)d2,(lte)(B|Q) ,(lte)c1,(lte)(B|Q)
    ,(lte)3 ,(lte)f6,(lte)(K|P|B|Q) ,(lte)e7,(lte)(B|Q) ,(lte)d8,(lte)(B|Q)
    ,(lte)1 ,(lte)h6,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)h4,(lte)(K|B|Q)
};
static const lte attacks_white_g6[] = {
(lte)8
    ,(lte)6 ,(lte)f6,(lte)(K|R|Q)   ,(lte)e6,(lte)(R|Q) ,(lte)d6,(lte)(R|Q) ,(lte)c6,(lte)(R|Q) ,(lte)b6,(lte)(R|Q) ,(lte)a6,(lte)(R|Q)
    ,(lte)1 ,(lte)h6,(lte)(K|R|Q)
    ,(lte)5 ,(lte)g5,(lte)(K|R|Q)   ,(lte)g4,(lte)(R|Q) ,(lte)g3,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)g1,(lte)(R|Q)
    ,(lte)2 ,(lte)g7,(lte)(K|R|Q)   ,(lte)g8,(lte)(R|Q)
    ,(lte)5 ,(lte)f5,(lte)(K|B|Q)   ,(lte)e4,(lte)(B|Q) ,(lte)d3,(lte)(B|Q) ,(lte)c2,(lte)(B|Q) ,(lte)b1,(lte)(B|Q)
    ,(lte)2 ,(lte)f7,(lte)(K|P|B|Q) ,(lte)e8,(lte)(B|Q)
    ,(lte)1 ,(lte)h7,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)h5,(lte)(K|B|Q)
};
static const lte attacks_white_g7[] = {
(lte)8
    ,(lte)6 ,(lte)f7,(lte)(K|R|Q)   ,(lte)e7,(lte)(R|Q) ,(lte)d7,(lte)(R|Q) ,(lte)c7,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)a7,(lte)(R|Q)
    ,(lte)1 ,(lte)h7,(lte)(K|R|Q)
    ,(lte)6 ,(lte)g6,(lte)(K|R|Q)   ,(lte)g5,(lte)(R|Q) ,(lte)g4,(lte)(R|Q) ,(lte)g3,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)g1,(lte)(R|Q)
    ,(lte)1 ,(lte)g8,(lte)(K|R|Q)
    ,(lte)6 ,(lte)f6,(lte)(K|B|Q)   ,(lte)e5,(lte)(B|Q) ,(lte)d4,(lte)(B|Q) ,(lte)c3,(lte)(B|Q) ,(lte)b2,(lte)(B|Q) ,(lte)a1,(lte)(B|Q)
    ,(lte)1 ,(lte)f8,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)h8,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)h6,(lte)(K|B|Q)
};
static const lte attacks_white_g8[] = {
(lte)5
    ,(lte)6 ,(lte)f8,(lte)(K|R|Q)   ,(lte)e8,(lte)(R|Q) ,(lte)d8,(lte)(R|Q) ,(lte)c8,(lte)(R|Q) ,(lte)b8,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)1 ,(lte)h8,(lte)(K|R|Q)
    ,(lte)7 ,(lte)g7,(lte)(K|R|Q)   ,(lte)g6,(lte)(R|Q) ,(lte)g5,(lte)(R|Q) ,(lte)g4,(lte)(R|Q) ,(lte)g3,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)g1,(lte)(R|Q)
    ,(lte)6 ,(lte)f7,(lte)(K|B|Q)   ,(lte)e6,(lte)(B|Q) ,(lte)d5,(lte)(B|Q) ,(lte)c4,(lte)(B|Q) ,(lte)b3,(lte)(B|Q) ,(lte)a2,(lte)(B|Q)
    ,(lte)1 ,(lte)h7,(lte)(K|B|Q)
};
static const lte attacks_white_h1[] = {
(lte)3
    ,(lte)7 ,(lte)g1,(lte)(K|R|Q)   ,(lte)f1,(lte)(R|Q) ,(lte)e1,(lte)(R|Q) ,(lte)d1,(lte)(R|Q) ,(lte)c1,(lte)(R|Q) ,(lte)b1,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)7 ,(lte)h2,(lte)(K|R|Q)   ,(lte)h3,(lte)(R|Q) ,(lte)h4,(lte)(R|Q) ,(lte)h5,(lte)(R|Q) ,(lte)h6,(lte)(R|Q) ,(lte)h7,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)7 ,(lte)g2,(lte)(K|P|B|Q) ,(lte)f3,(lte)(B|Q) ,(lte)e4,(lte)(B|Q) ,(lte)d5,(lte)(B|Q) ,(lte)c6,(lte)(B|Q) ,(lte)b7,(lte)(B|Q) ,(lte)a8,(lte)(B|Q)
};
static const lte attacks_white_h2[] = {
(lte)5
    ,(lte)7 ,(lte)g2,(lte)(K|R|Q)   ,(lte)f2,(lte)(R|Q) ,(lte)e2,(lte)(R|Q) ,(lte)d2,(lte)(R|Q) ,(lte)c2,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)a2,(lte)(R|Q)
    ,(lte)1 ,(lte)h1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)h3,(lte)(K|R|Q)   ,(lte)h4,(lte)(R|Q) ,(lte)h5,(lte)(R|Q) ,(lte)h6,(lte)(R|Q) ,(lte)h7,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)1 ,(lte)g1,(lte)(K|B|Q)
    ,(lte)6 ,(lte)g3,(lte)(K|P|B|Q) ,(lte)f4,(lte)(B|Q) ,(lte)e5,(lte)(B|Q) ,(lte)d6,(lte)(B|Q) ,(lte)c7,(lte)(B|Q) ,(lte)b8,(lte)(B|Q)
};
static const lte attacks_white_h3[] = {
(lte)5
    ,(lte)7 ,(lte)g3,(lte)(K|R|Q)   ,(lte)f3,(lte)(R|Q) ,(lte)e3,(lte)(R|Q) ,(lte)d3,(lte)(R|Q) ,(lte)c3,(lte)(R|Q) ,(lte)b3,(lte)(R|Q) ,(lte)a3,(lte)(R|Q)
    ,(lte)2 ,(lte)h2,(lte)(K|R|Q)   ,(lte)h1,(lte)(R|Q)
    ,(lte)5 ,(lte)h4,(lte)(K|R|Q)   ,(lte)h5,(lte)(R|Q) ,(lte)h6,(lte)(R|Q) ,(lte)h7,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)2 ,(lte)g2,(lte)(K|B|Q)   ,(lte)f1,(lte)(B|Q)
    ,(lte)5 ,(lte)g4,(lte)(K|P|B|Q) ,(lte)f5,(lte)(B|Q) ,(lte)e6,(lte)(B|Q) ,(lte)d7,(lte)(B|Q) ,(lte)c8,(lte)(B|Q)
};
static const lte attacks_white_h4[] = {
(lte)5
    ,(lte)7 ,(lte)g4,(lte)(K|R|Q)   ,(lte)f4,(lte)(R|Q) ,(lte)e4,(lte)(R|Q) ,(lte)d4,(lte)(R|Q) ,(lte)c4,(lte)(R|Q) ,(lte)b4,(lte)(R|Q) ,(lte)a4,(lte)(R|Q)
    ,(lte)3 ,(lte)h3,(lte)(K|R|Q)   ,(lte)h2,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)4 ,(lte)h5,(lte)(K|R|Q)   ,(lte)h6,(lte)(R|Q) ,(lte)h7,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)3 ,(lte)g3,(lte)(K|B|Q)   ,(lte)f2,(lte)(B|Q) ,(lte)e1,(lte)(B|Q)
    ,(lte)4 ,(lte)g5,(lte)(K|P|B|Q) ,(lte)f6,(lte)(B|Q) ,(lte)e7,(lte)(B|Q) ,(lte)d8,(lte)(B|Q)
};
static const lte attacks_white_h5[] = {
(lte)5
    ,(lte)7 ,(lte)g5,(lte)(K|R|Q)   ,(lte)f5,(lte)(R|Q) ,(lte)e5,(lte)(R|Q) ,(lte)d5,(lte)(R|Q) ,(lte)c5,(lte)(R|Q) ,(lte)b5,(lte)(R|Q) ,(lte)a5,(lte)(R|Q)
    ,(lte)4 ,(lte)h4,(lte)(K|R|Q)   ,(lte)h3,(lte)(R|Q) ,(lte)h2,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)3 ,(lte)h6,(lte)(K|R|Q)   ,(lte)h7,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)4 ,(lte)g4,(lte)(K|B|Q)   ,(lte)f3,(lte)(B|Q) ,(lte)e2,(lte)(B|Q) ,(lte)d1,(lte)(B|Q)
    ,(lte)3 ,(lte)g6,(lte)(K|P|B|Q) ,(lte)f7,(lte)(B|Q) ,(lte)e8,(lte)(B|Q)
};
static const lte attacks_white_h6[] = {
(lte)5
    ,(lte)7 ,(lte)g6,(lte)(K|R|Q)   ,(lte)f6,(lte)(R|Q) ,(lte)e6,(lte)(R|Q) ,(lte)d6,(lte)(R|Q) ,(lte)c6,(lte)(R|Q) ,(lte)b6,(lte)(R|Q) ,(lte)a6,(lte)(R|Q)
    ,(lte)5 ,(lte)h5,(lte)(K|R|Q)   ,(lte)h4,(lte)(R|Q) ,(lte)h3,(lte)(R|Q) ,(lte)h2,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)2 ,(lte)h7,(lte)(K|R|Q)   ,(lte)h8,(lte)(R|Q)
    ,(lte)5 ,(lte)g5,(lte)(K|B|Q)   ,(lte)f4,(lte)(B|Q) ,(lte)e3,(lte)(B|Q) ,(lte)d2,(lte)(B|Q) ,(lte)c1,(lte)(B|Q)
    ,(lte)2 ,(lte)g7,(lte)(K|P|B|Q) ,(lte)f8,(lte)(B|Q)
};
static const lte attacks_white_h7[] = {
(lte)5
    ,(lte)7 ,(lte)g7,(lte)(K|R|Q)   ,(lte)f7,(lte)(R|Q) ,(lte)e7,(lte)(R|Q) ,(lte)d7,(lte)(R|Q) ,(lte)c7,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)a7,(lte)(R|Q)
    ,(lte)6 ,(lte)h6,(lte)(K|R|Q)   ,(lte)h5,(lte)(R|Q) ,(lte)h4,(lte)(R|Q) ,(lte)h3,(lte)(R|Q) ,(lte)h2,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)1 ,(lte)h8,(lte)(K|R|Q)
    ,(lte)6 ,(lte)g6,(lte)(K|B|Q)   ,(lte)f5,(lte)(B|Q) ,(lte)e4,(lte)(B|Q) ,(lte)d3,(lte)(B|Q) ,(lte)c2,(lte)(B|Q) ,(lte)b1,(lte)(B|Q)
    ,(lte)1 ,(lte)g8,(lte)(K|P|B|Q)
};
static const lte attacks_white_h8[] = {
(lte)3
    ,(lte)7 ,(lte)g8,(lte)(K|R|Q)   ,(lte)f8,(lte)(R|Q) ,(lte)e8,(lte)(R|Q) ,(lte)d8,(lte)(R|Q) ,(lte)c8,(lte)(R|Q) ,(lte)b8,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)7 ,(lte)h7,(lte)(K|R|Q)   ,(lte)h6,(lte)(R|Q) ,(lte)h5,(lte)(R|Q) ,(lte)h4,(lte)(R|Q) ,(lte)h3,(lte)(R|Q) ,(lte)h2,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)7 ,(lte)g7,(lte)(K|B|Q)   ,(lte)f6,(lte)(B|Q) ,(lte)e5,(lte)(B|Q) ,(lte)d4,(lte)(B|Q) ,(lte)c3,(lte)(B|Q) ,(lte)b2,(lte)(B|Q) ,(lte)a1,(lte)(B|Q)
};

// attacks_white_lookup
const lte *attacks_white_lookup[] = {
    attacks_white_a8,
    attacks_white_b8,
    attacks_white_c8,
    attacks_white_d8,
    attacks_white_e8,
    attacks_white_f8,
    attacks_white_g8,
    attacks_white_h8,
    attacks_white_a7,
    attacks_white_b7,
    attacks_white_c7,
    attacks_white_d7,
    attacks_white_e7,
    attacks_white_f7,
    attacks_white_g7,
    attacks_white_h7,
    attacks_white_a6,
    attacks_white_b6,
    attacks_white_c6,
    attacks_white_d6,
    attacks_white_e6,
    attacks_white_f6,
    attacks_white_g6,
    attacks_white_h6,
    attacks_white_a5,
    attacks_white_b5,
    attacks_white_c5,
    attacks_white_d5,
    attacks_white_e5,
    attacks_white_f5,
    attacks_white_g5,
    attacks_white_h5,
    attacks_white_a4,
    attacks_white_b4,
    attacks_white_c4,
    attacks_white_d4,
    attacks_white_e4,
    attacks_white_f4,
    attacks_white_g4,
    attacks_white_h4,
    attacks_white_a3,
    attacks_white_b3,
    attacks_white_c3,
    attacks_white_d3,
    attacks_white_e3,
    attacks_white_f3,
    attacks_white_g3,
    attacks_white_h3,
    attacks_white_a2,
    attacks_white_b2,
    attacks_white_c2,
    attacks_white_d2,
    attacks_white_e2,
    attacks_white_f2,
    attacks_white_g2,
    attacks_white_h2,
    attacks_white_a1,
    attacks_white_b1,
    attacks_white_c1,
    attacks_white_d1,
    attacks_white_e1,
    attacks_white_f1,
    attacks_white_g1,
    attacks_white_h1,
};

// Attack from up to 8 rays on a black piece
static const lte attacks_black_a1[] = {
(lte)3
    ,(lte)7 ,(lte)b1,(lte)(K|R|Q)   ,(lte)c1,(lte)(R|Q) ,(lte)d1,(lte)(R|Q) ,(lte)e1,(lte)(R|Q) ,(lte)f1,(lte)(R|Q) ,(lte)g1,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)7 ,(lte)a2,(lte)(K|R|Q)   ,(lte)a3,(lte)(R|Q) ,(lte)a4,(lte)(R|Q) ,(lte)a5,(lte)(R|Q) ,(lte)a6,(lte)(R|Q) ,(lte)a7,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)7 ,(lte)b2,(lte)(K|B|Q)   ,(lte)c3,(lte)(B|Q) ,(lte)d4,(lte)(B|Q) ,(lte)e5,(lte)(B|Q) ,(lte)f6,(lte)(B|Q) ,(lte)g7,(lte)(B|Q) ,(lte)h8,(lte)(B|Q)
};
static const lte attacks_black_a2[] = {
(lte)5
    ,(lte)7 ,(lte)b2,(lte)(K|R|Q)   ,(lte)c2,(lte)(R|Q) ,(lte)d2,(lte)(R|Q) ,(lte)e2,(lte)(R|Q) ,(lte)f2,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)h2,(lte)(R|Q)
    ,(lte)1 ,(lte)a1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)a3,(lte)(K|R|Q)   ,(lte)a4,(lte)(R|Q) ,(lte)a5,(lte)(R|Q) ,(lte)a6,(lte)(R|Q) ,(lte)a7,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)6 ,(lte)b3,(lte)(K|B|Q)   ,(lte)c4,(lte)(B|Q) ,(lte)d5,(lte)(B|Q) ,(lte)e6,(lte)(B|Q) ,(lte)f7,(lte)(B|Q) ,(lte)g8,(lte)(B|Q)
    ,(lte)1 ,(lte)b1,(lte)(K|P|B|Q)
};
static const lte attacks_black_a3[] = {
(lte)5
    ,(lte)7 ,(lte)b3,(lte)(K|R|Q)   ,(lte)c3,(lte)(R|Q) ,(lte)d3,(lte)(R|Q) ,(lte)e3,(lte)(R|Q) ,(lte)f3,(lte)(R|Q) ,(lte)g3,(lte)(R|Q) ,(lte)h3,(lte)(R|Q)
    ,(lte)2 ,(lte)a2,(lte)(K|R|Q)   ,(lte)a1,(lte)(R|Q)
    ,(lte)5 ,(lte)a4,(lte)(K|R|Q)   ,(lte)a5,(lte)(R|Q) ,(lte)a6,(lte)(R|Q) ,(lte)a7,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)5 ,(lte)b4,(lte)(K|B|Q)   ,(lte)c5,(lte)(B|Q) ,(lte)d6,(lte)(B|Q) ,(lte)e7,(lte)(B|Q) ,(lte)f8,(lte)(B|Q)
    ,(lte)2 ,(lte)b2,(lte)(K|P|B|Q) ,(lte)c1,(lte)(B|Q)
};
static const lte attacks_black_a4[] = {
(lte)5
    ,(lte)7 ,(lte)b4,(lte)(K|R|Q)   ,(lte)c4,(lte)(R|Q) ,(lte)d4,(lte)(R|Q) ,(lte)e4,(lte)(R|Q) ,(lte)f4,(lte)(R|Q) ,(lte)g4,(lte)(R|Q) ,(lte)h4,(lte)(R|Q)
    ,(lte)3 ,(lte)a3,(lte)(K|R|Q)   ,(lte)a2,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)4 ,(lte)a5,(lte)(K|R|Q)   ,(lte)a6,(lte)(R|Q) ,(lte)a7,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)4 ,(lte)b5,(lte)(K|B|Q)   ,(lte)c6,(lte)(B|Q) ,(lte)d7,(lte)(B|Q) ,(lte)e8,(lte)(B|Q)
    ,(lte)3 ,(lte)b3,(lte)(K|P|B|Q) ,(lte)c2,(lte)(B|Q) ,(lte)d1,(lte)(B|Q)
};
static const lte attacks_black_a5[] = {
(lte)5
    ,(lte)7 ,(lte)b5,(lte)(K|R|Q)   ,(lte)c5,(lte)(R|Q) ,(lte)d5,(lte)(R|Q) ,(lte)e5,(lte)(R|Q) ,(lte)f5,(lte)(R|Q) ,(lte)g5,(lte)(R|Q) ,(lte)h5,(lte)(R|Q)
    ,(lte)4 ,(lte)a4,(lte)(K|R|Q)   ,(lte)a3,(lte)(R|Q) ,(lte)a2,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)3 ,(lte)a6,(lte)(K|R|Q)   ,(lte)a7,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)3 ,(lte)b6,(lte)(K|B|Q)   ,(lte)c7,(lte)(B|Q) ,(lte)d8,(lte)(B|Q)
    ,(lte)4 ,(lte)b4,(lte)(K|P|B|Q) ,(lte)c3,(lte)(B|Q) ,(lte)d2,(lte)(B|Q) ,(lte)e1,(lte)(B|Q)
};
static const lte attacks_black_a6[] = {
(lte)5
    ,(lte)7 ,(lte)b6,(lte)(K|R|Q)   ,(lte)c6,(lte)(R|Q) ,(lte)d6,(lte)(R|Q) ,(lte)e6,(lte)(R|Q) ,(lte)f6,(lte)(R|Q) ,(lte)g6,(lte)(R|Q) ,(lte)h6,(lte)(R|Q)
    ,(lte)5 ,(lte)a5,(lte)(K|R|Q)   ,(lte)a4,(lte)(R|Q) ,(lte)a3,(lte)(R|Q) ,(lte)a2,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)2 ,(lte)a7,(lte)(K|R|Q)   ,(lte)a8,(lte)(R|Q)
    ,(lte)2 ,(lte)b7,(lte)(K|B|Q)   ,(lte)c8,(lte)(B|Q)
    ,(lte)5 ,(lte)b5,(lte)(K|P|B|Q) ,(lte)c4,(lte)(B|Q) ,(lte)d3,(lte)(B|Q) ,(lte)e2,(lte)(B|Q) ,(lte)f1,(lte)(B|Q)
};
static const lte attacks_black_a7[] = {
(lte)5
    ,(lte)7 ,(lte)b7,(lte)(K|R|Q)   ,(lte)c7,(lte)(R|Q) ,(lte)d7,(lte)(R|Q) ,(lte)e7,(lte)(R|Q) ,(lte)f7,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)h7,(lte)(R|Q)
    ,(lte)6 ,(lte)a6,(lte)(K|R|Q)   ,(lte)a5,(lte)(R|Q) ,(lte)a4,(lte)(R|Q) ,(lte)a3,(lte)(R|Q) ,(lte)a2,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)1 ,(lte)a8,(lte)(K|R|Q)
    ,(lte)1 ,(lte)b8,(lte)(K|B|Q)
    ,(lte)6 ,(lte)b6,(lte)(K|P|B|Q) ,(lte)c5,(lte)(B|Q) ,(lte)d4,(lte)(B|Q) ,(lte)e3,(lte)(B|Q) ,(lte)f2,(lte)(B|Q) ,(lte)g1,(lte)(B|Q)
};
static const lte attacks_black_a8[] = {
(lte)3
    ,(lte)7 ,(lte)b8,(lte)(K|R|Q)   ,(lte)c8,(lte)(R|Q) ,(lte)d8,(lte)(R|Q) ,(lte)e8,(lte)(R|Q) ,(lte)f8,(lte)(R|Q) ,(lte)g8,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)7 ,(lte)a7,(lte)(K|R|Q)   ,(lte)a6,(lte)(R|Q) ,(lte)a5,(lte)(R|Q) ,(lte)a4,(lte)(R|Q) ,(lte)a3,(lte)(R|Q) ,(lte)a2,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)7 ,(lte)b7,(lte)(K|P|B|Q) ,(lte)c6,(lte)(B|Q) ,(lte)d5,(lte)(B|Q) ,(lte)e4,(lte)(B|Q) ,(lte)f3,(lte)(B|Q) ,(lte)g2,(lte)(B|Q) ,(lte)h1,(lte)(B|Q)
};
static const lte attacks_black_b1[] = {
(lte)5
    ,(lte)1 ,(lte)a1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c1,(lte)(K|R|Q)   ,(lte)d1,(lte)(R|Q) ,(lte)e1,(lte)(R|Q) ,(lte)f1,(lte)(R|Q) ,(lte)g1,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)7 ,(lte)b2,(lte)(K|R|Q)   ,(lte)b3,(lte)(R|Q) ,(lte)b4,(lte)(R|Q) ,(lte)b5,(lte)(R|Q) ,(lte)b6,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)b8,(lte)(R|Q)
    ,(lte)1 ,(lte)a2,(lte)(K|B|Q)
    ,(lte)6 ,(lte)c2,(lte)(K|B|Q)   ,(lte)d3,(lte)(B|Q) ,(lte)e4,(lte)(B|Q) ,(lte)f5,(lte)(B|Q) ,(lte)g6,(lte)(B|Q) ,(lte)h7,(lte)(B|Q)
};
static const lte attacks_black_b2[] = {
(lte)8
    ,(lte)1 ,(lte)a2,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c2,(lte)(K|R|Q)   ,(lte)d2,(lte)(R|Q) ,(lte)e2,(lte)(R|Q) ,(lte)f2,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)h2,(lte)(R|Q)
    ,(lte)1 ,(lte)b1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)b3,(lte)(K|R|Q)   ,(lte)b4,(lte)(R|Q) ,(lte)b5,(lte)(R|Q) ,(lte)b6,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)b8,(lte)(R|Q)
    ,(lte)1 ,(lte)a1,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)a3,(lte)(K|B|Q)
    ,(lte)6 ,(lte)c3,(lte)(K|B|Q)   ,(lte)d4,(lte)(B|Q) ,(lte)e5,(lte)(B|Q) ,(lte)f6,(lte)(B|Q) ,(lte)g7,(lte)(B|Q) ,(lte)h8,(lte)(B|Q)
    ,(lte)1 ,(lte)c1,(lte)(K|P|B|Q)
};
static const lte attacks_black_b3[] = {
(lte)8
    ,(lte)1 ,(lte)a3,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c3,(lte)(K|R|Q)   ,(lte)d3,(lte)(R|Q) ,(lte)e3,(lte)(R|Q) ,(lte)f3,(lte)(R|Q) ,(lte)g3,(lte)(R|Q) ,(lte)h3,(lte)(R|Q)
    ,(lte)2 ,(lte)b2,(lte)(K|R|Q)   ,(lte)b1,(lte)(R|Q)
    ,(lte)5 ,(lte)b4,(lte)(K|R|Q)   ,(lte)b5,(lte)(R|Q) ,(lte)b6,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)b8,(lte)(R|Q)
    ,(lte)1 ,(lte)a2,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)a4,(lte)(K|B|Q)
    ,(lte)5 ,(lte)c4,(lte)(K|B|Q)   ,(lte)d5,(lte)(B|Q) ,(lte)e6,(lte)(B|Q) ,(lte)f7,(lte)(B|Q) ,(lte)g8,(lte)(B|Q)
    ,(lte)2 ,(lte)c2,(lte)(K|P|B|Q) ,(lte)d1,(lte)(B|Q)
};
static const lte attacks_black_b4[] = {
(lte)8
    ,(lte)1 ,(lte)a4,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c4,(lte)(K|R|Q)   ,(lte)d4,(lte)(R|Q) ,(lte)e4,(lte)(R|Q) ,(lte)f4,(lte)(R|Q) ,(lte)g4,(lte)(R|Q) ,(lte)h4,(lte)(R|Q)
    ,(lte)3 ,(lte)b3,(lte)(K|R|Q)   ,(lte)b2,(lte)(R|Q) ,(lte)b1,(lte)(R|Q)
    ,(lte)4 ,(lte)b5,(lte)(K|R|Q)   ,(lte)b6,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)b8,(lte)(R|Q)
    ,(lte)1 ,(lte)a3,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)a5,(lte)(K|B|Q)
    ,(lte)4 ,(lte)c5,(lte)(K|B|Q)   ,(lte)d6,(lte)(B|Q) ,(lte)e7,(lte)(B|Q) ,(lte)f8,(lte)(B|Q)
    ,(lte)3 ,(lte)c3,(lte)(K|P|B|Q) ,(lte)d2,(lte)(B|Q) ,(lte)e1,(lte)(B|Q)
};
static const lte attacks_black_b5[] = {
(lte)8
    ,(lte)1 ,(lte)a5,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c5,(lte)(K|R|Q)   ,(lte)d5,(lte)(R|Q) ,(lte)e5,(lte)(R|Q) ,(lte)f5,(lte)(R|Q) ,(lte)g5,(lte)(R|Q) ,(lte)h5,(lte)(R|Q)
    ,(lte)4 ,(lte)b4,(lte)(K|R|Q)   ,(lte)b3,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)b1,(lte)(R|Q)
    ,(lte)3 ,(lte)b6,(lte)(K|R|Q)   ,(lte)b7,(lte)(R|Q) ,(lte)b8,(lte)(R|Q)
    ,(lte)1 ,(lte)a4,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)a6,(lte)(K|B|Q)
    ,(lte)3 ,(lte)c6,(lte)(K|B|Q)   ,(lte)d7,(lte)(B|Q) ,(lte)e8,(lte)(B|Q)
    ,(lte)4 ,(lte)c4,(lte)(K|P|B|Q) ,(lte)d3,(lte)(B|Q) ,(lte)e2,(lte)(B|Q) ,(lte)f1,(lte)(B|Q)
};
static const lte attacks_black_b6[] = {
(lte)8
    ,(lte)1 ,(lte)a6,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c6,(lte)(K|R|Q)   ,(lte)d6,(lte)(R|Q) ,(lte)e6,(lte)(R|Q) ,(lte)f6,(lte)(R|Q) ,(lte)g6,(lte)(R|Q) ,(lte)h6,(lte)(R|Q)
    ,(lte)5 ,(lte)b5,(lte)(K|R|Q)   ,(lte)b4,(lte)(R|Q) ,(lte)b3,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)b1,(lte)(R|Q)
    ,(lte)2 ,(lte)b7,(lte)(K|R|Q)   ,(lte)b8,(lte)(R|Q)
    ,(lte)1 ,(lte)a5,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)a7,(lte)(K|B|Q)
    ,(lte)2 ,(lte)c7,(lte)(K|B|Q)   ,(lte)d8,(lte)(B|Q)
    ,(lte)5 ,(lte)c5,(lte)(K|P|B|Q) ,(lte)d4,(lte)(B|Q) ,(lte)e3,(lte)(B|Q) ,(lte)f2,(lte)(B|Q) ,(lte)g1,(lte)(B|Q)
};
static const lte attacks_black_b7[] = {
(lte)8
    ,(lte)1 ,(lte)a7,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c7,(lte)(K|R|Q)   ,(lte)d7,(lte)(R|Q) ,(lte)e7,(lte)(R|Q) ,(lte)f7,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)h7,(lte)(R|Q)
    ,(lte)6 ,(lte)b6,(lte)(K|R|Q)   ,(lte)b5,(lte)(R|Q) ,(lte)b4,(lte)(R|Q) ,(lte)b3,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)b1,(lte)(R|Q)
    ,(lte)1 ,(lte)b8,(lte)(K|R|Q)
    ,(lte)1 ,(lte)a6,(lte)(K|P|B|Q)
    ,(lte)1 ,(lte)a8,(lte)(K|B|Q)
    ,(lte)1 ,(lte)c8,(lte)(K|B|Q)
    ,(lte)6 ,(lte)c6,(lte)(K|P|B|Q) ,(lte)d5,(lte)(B|Q) ,(lte)e4,(lte)(B|Q) ,(lte)f3,(lte)(B|Q) ,(lte)g2,(lte)(B|Q) ,(lte)h1,(lte)(B|Q)
};
static const lte attacks_black_b8[] = {
(lte)5
    ,(lte)1 ,(lte)a8,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c8,(lte)(K|R|Q)   ,(lte)d8,(lte)(R|Q) ,(lte)e8,(lte)(R|Q) ,(lte)f8,(lte)(R|Q) ,(lte)g8,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)7 ,(lte)b7,(lte)(K|R|Q)   ,(lte)b6,(lte)(R|Q) ,(lte)b5,(lte)(R|Q) ,(lte)b4,(lte)(R|Q) ,(lte)b3,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)b1,(lte)(R|Q)
    ,(lte)1 ,(lte)a7,(lte)(K|P|B|Q)
    ,(lte)6 ,(lte)c7,(lte)(K|P|B|Q) ,(lte)d6,(lte)(B|Q) ,(lte)e5,(lte)(B|Q) ,(lte)f4,(lte)(B|Q) ,(lte)g3,(lte)(B|Q) ,(lte)h2,(lte)(B|Q)
};
static const lte attacks_black_c1[] = {
(lte)5
    ,(lte)2 ,(lte)b1,(lte)(K|R|Q)   ,(lte)a1,(lte)(R|Q)
    ,(lte)5 ,(lte)d1,(lte)(K|R|Q)   ,(lte)e1,(lte)(R|Q) ,(lte)f1,(lte)(R|Q) ,(lte)g1,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)7 ,(lte)c2,(lte)(K|R|Q)   ,(lte)c3,(lte)(R|Q) ,(lte)c4,(lte)(R|Q) ,(lte)c5,(lte)(R|Q) ,(lte)c6,(lte)(R|Q) ,(lte)c7,(lte)(R|Q) ,(lte)c8,(lte)(R|Q)
    ,(lte)2 ,(lte)b2,(lte)(K|B|Q)   ,(lte)a3,(lte)(B|Q)
    ,(lte)5 ,(lte)d2,(lte)(K|B|Q)   ,(lte)e3,(lte)(B|Q) ,(lte)f4,(lte)(B|Q) ,(lte)g5,(lte)(B|Q) ,(lte)h6,(lte)(B|Q)
};
static const lte attacks_black_c2[] = {
(lte)8
    ,(lte)2 ,(lte)b2,(lte)(K|R|Q)   ,(lte)a2,(lte)(R|Q)
    ,(lte)5 ,(lte)d2,(lte)(K|R|Q)   ,(lte)e2,(lte)(R|Q) ,(lte)f2,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)h2,(lte)(R|Q)
    ,(lte)1 ,(lte)c1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)c3,(lte)(K|R|Q)   ,(lte)c4,(lte)(R|Q) ,(lte)c5,(lte)(R|Q) ,(lte)c6,(lte)(R|Q) ,(lte)c7,(lte)(R|Q) ,(lte)c8,(lte)(R|Q)
    ,(lte)1 ,(lte)b1,(lte)(K|P|B|Q)
    ,(lte)2 ,(lte)b3,(lte)(K|B|Q)   ,(lte)a4,(lte)(B|Q)
    ,(lte)5 ,(lte)d3,(lte)(K|B|Q)   ,(lte)e4,(lte)(B|Q) ,(lte)f5,(lte)(B|Q) ,(lte)g6,(lte)(B|Q) ,(lte)h7,(lte)(B|Q)
    ,(lte)1 ,(lte)d1,(lte)(K|P|B|Q)
};
static const lte attacks_black_c3[] = {
(lte)8
    ,(lte)2 ,(lte)b3,(lte)(K|R|Q)   ,(lte)a3,(lte)(R|Q)
    ,(lte)5 ,(lte)d3,(lte)(K|R|Q)   ,(lte)e3,(lte)(R|Q) ,(lte)f3,(lte)(R|Q) ,(lte)g3,(lte)(R|Q) ,(lte)h3,(lte)(R|Q)
    ,(lte)2 ,(lte)c2,(lte)(K|R|Q)   ,(lte)c1,(lte)(R|Q)
    ,(lte)5 ,(lte)c4,(lte)(K|R|Q)   ,(lte)c5,(lte)(R|Q) ,(lte)c6,(lte)(R|Q) ,(lte)c7,(lte)(R|Q) ,(lte)c8,(lte)(R|Q)
    ,(lte)2 ,(lte)b2,(lte)(K|P|B|Q) ,(lte)a1,(lte)(B|Q)
    ,(lte)2 ,(lte)b4,(lte)(K|B|Q)   ,(lte)a5,(lte)(B|Q)
    ,(lte)5 ,(lte)d4,(lte)(K|B|Q)   ,(lte)e5,(lte)(B|Q) ,(lte)f6,(lte)(B|Q) ,(lte)g7,(lte)(B|Q) ,(lte)h8,(lte)(B|Q)
    ,(lte)2 ,(lte)d2,(lte)(K|P|B|Q) ,(lte)e1,(lte)(B|Q)
};
static const lte attacks_black_c4[] = {
(lte)8
    ,(lte)2 ,(lte)b4,(lte)(K|R|Q)   ,(lte)a4,(lte)(R|Q)
    ,(lte)5 ,(lte)d4,(lte)(K|R|Q)   ,(lte)e4,(lte)(R|Q) ,(lte)f4,(lte)(R|Q) ,(lte)g4,(lte)(R|Q) ,(lte)h4,(lte)(R|Q)
    ,(lte)3 ,(lte)c3,(lte)(K|R|Q)   ,(lte)c2,(lte)(R|Q) ,(lte)c1,(lte)(R|Q)
    ,(lte)4 ,(lte)c5,(lte)(K|R|Q)   ,(lte)c6,(lte)(R|Q) ,(lte)c7,(lte)(R|Q) ,(lte)c8,(lte)(R|Q)
    ,(lte)2 ,(lte)b3,(lte)(K|P|B|Q) ,(lte)a2,(lte)(B|Q)
    ,(lte)2 ,(lte)b5,(lte)(K|B|Q)   ,(lte)a6,(lte)(B|Q)
    ,(lte)4 ,(lte)d5,(lte)(K|B|Q)   ,(lte)e6,(lte)(B|Q) ,(lte)f7,(lte)(B|Q) ,(lte)g8,(lte)(B|Q)
    ,(lte)3 ,(lte)d3,(lte)(K|P|B|Q) ,(lte)e2,(lte)(B|Q) ,(lte)f1,(lte)(B|Q)
};
static const lte attacks_black_c5[] = {
(lte)8
    ,(lte)2 ,(lte)b5,(lte)(K|R|Q)   ,(lte)a5,(lte)(R|Q)
    ,(lte)5 ,(lte)d5,(lte)(K|R|Q)   ,(lte)e5,(lte)(R|Q) ,(lte)f5,(lte)(R|Q) ,(lte)g5,(lte)(R|Q) ,(lte)h5,(lte)(R|Q)
    ,(lte)4 ,(lte)c4,(lte)(K|R|Q)   ,(lte)c3,(lte)(R|Q) ,(lte)c2,(lte)(R|Q) ,(lte)c1,(lte)(R|Q)
    ,(lte)3 ,(lte)c6,(lte)(K|R|Q)   ,(lte)c7,(lte)(R|Q) ,(lte)c8,(lte)(R|Q)
    ,(lte)2 ,(lte)b4,(lte)(K|P|B|Q) ,(lte)a3,(lte)(B|Q)
    ,(lte)2 ,(lte)b6,(lte)(K|B|Q)   ,(lte)a7,(lte)(B|Q)
    ,(lte)3 ,(lte)d6,(lte)(K|B|Q)   ,(lte)e7,(lte)(B|Q) ,(lte)f8,(lte)(B|Q)
    ,(lte)4 ,(lte)d4,(lte)(K|P|B|Q) ,(lte)e3,(lte)(B|Q) ,(lte)f2,(lte)(B|Q) ,(lte)g1,(lte)(B|Q)
};
static const lte attacks_black_c6[] = {
(lte)8
    ,(lte)2 ,(lte)b6,(lte)(K|R|Q)   ,(lte)a6,(lte)(R|Q)
    ,(lte)5 ,(lte)d6,(lte)(K|R|Q)   ,(lte)e6,(lte)(R|Q) ,(lte)f6,(lte)(R|Q) ,(lte)g6,(lte)(R|Q) ,(lte)h6,(lte)(R|Q)
    ,(lte)5 ,(lte)c5,(lte)(K|R|Q)   ,(lte)c4,(lte)(R|Q) ,(lte)c3,(lte)(R|Q) ,(lte)c2,(lte)(R|Q) ,(lte)c1,(lte)(R|Q)
    ,(lte)2 ,(lte)c7,(lte)(K|R|Q)   ,(lte)c8,(lte)(R|Q)
    ,(lte)2 ,(lte)b5,(lte)(K|P|B|Q) ,(lte)a4,(lte)(B|Q)
    ,(lte)2 ,(lte)b7,(lte)(K|B|Q)   ,(lte)a8,(lte)(B|Q)
    ,(lte)2 ,(lte)d7,(lte)(K|B|Q)   ,(lte)e8,(lte)(B|Q)
    ,(lte)5 ,(lte)d5,(lte)(K|P|B|Q) ,(lte)e4,(lte)(B|Q) ,(lte)f3,(lte)(B|Q) ,(lte)g2,(lte)(B|Q) ,(lte)h1,(lte)(B|Q)
};
static const lte attacks_black_c7[] = {
(lte)8
    ,(lte)2 ,(lte)b7,(lte)(K|R|Q)   ,(lte)a7,(lte)(R|Q)
    ,(lte)5 ,(lte)d7,(lte)(K|R|Q)   ,(lte)e7,(lte)(R|Q) ,(lte)f7,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)h7,(lte)(R|Q)
    ,(lte)6 ,(lte)c6,(lte)(K|R|Q)   ,(lte)c5,(lte)(R|Q) ,(lte)c4,(lte)(R|Q) ,(lte)c3,(lte)(R|Q) ,(lte)c2,(lte)(R|Q) ,(lte)c1,(lte)(R|Q)
    ,(lte)1 ,(lte)c8,(lte)(K|R|Q)
    ,(lte)2 ,(lte)b6,(lte)(K|P|B|Q) ,(lte)a5,(lte)(B|Q)
    ,(lte)1 ,(lte)b8,(lte)(K|B|Q)
    ,(lte)1 ,(lte)d8,(lte)(K|B|Q)
    ,(lte)5 ,(lte)d6,(lte)(K|P|B|Q) ,(lte)e5,(lte)(B|Q) ,(lte)f4,(lte)(B|Q) ,(lte)g3,(lte)(B|Q) ,(lte)h2,(lte)(B|Q)
};
static const lte attacks_black_c8[] = {
(lte)5
    ,(lte)2 ,(lte)b8,(lte)(K|R|Q)   ,(lte)a8,(lte)(R|Q)
    ,(lte)5 ,(lte)d8,(lte)(K|R|Q)   ,(lte)e8,(lte)(R|Q) ,(lte)f8,(lte)(R|Q) ,(lte)g8,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)7 ,(lte)c7,(lte)(K|R|Q)   ,(lte)c6,(lte)(R|Q) ,(lte)c5,(lte)(R|Q) ,(lte)c4,(lte)(R|Q) ,(lte)c3,(lte)(R|Q) ,(lte)c2,(lte)(R|Q) ,(lte)c1,(lte)(R|Q)
    ,(lte)2 ,(lte)b7,(lte)(K|P|B|Q) ,(lte)a6,(lte)(B|Q)
    ,(lte)5 ,(lte)d7,(lte)(K|P|B|Q) ,(lte)e6,(lte)(B|Q) ,(lte)f5,(lte)(B|Q) ,(lte)g4,(lte)(B|Q) ,(lte)h3,(lte)(B|Q)
};
static const lte attacks_black_d1[] = {
(lte)5
    ,(lte)3 ,(lte)c1,(lte)(K|R|Q)   ,(lte)b1,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)4 ,(lte)e1,(lte)(K|R|Q)   ,(lte)f1,(lte)(R|Q) ,(lte)g1,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)7 ,(lte)d2,(lte)(K|R|Q)   ,(lte)d3,(lte)(R|Q) ,(lte)d4,(lte)(R|Q) ,(lte)d5,(lte)(R|Q) ,(lte)d6,(lte)(R|Q) ,(lte)d7,(lte)(R|Q) ,(lte)d8,(lte)(R|Q)
    ,(lte)3 ,(lte)c2,(lte)(K|B|Q)   ,(lte)b3,(lte)(B|Q) ,(lte)a4,(lte)(B|Q)
    ,(lte)4 ,(lte)e2,(lte)(K|B|Q)   ,(lte)f3,(lte)(B|Q) ,(lte)g4,(lte)(B|Q) ,(lte)h5,(lte)(B|Q)
};
static const lte attacks_black_d2[] = {
(lte)8
    ,(lte)3 ,(lte)c2,(lte)(K|R|Q)   ,(lte)b2,(lte)(R|Q) ,(lte)a2,(lte)(R|Q)
    ,(lte)4 ,(lte)e2,(lte)(K|R|Q)   ,(lte)f2,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)h2,(lte)(R|Q)
    ,(lte)1 ,(lte)d1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)d3,(lte)(K|R|Q)   ,(lte)d4,(lte)(R|Q) ,(lte)d5,(lte)(R|Q) ,(lte)d6,(lte)(R|Q) ,(lte)d7,(lte)(R|Q) ,(lte)d8,(lte)(R|Q)
    ,(lte)1 ,(lte)c1,(lte)(K|P|B|Q)
    ,(lte)3 ,(lte)c3,(lte)(K|B|Q)   ,(lte)b4,(lte)(B|Q) ,(lte)a5,(lte)(B|Q)
    ,(lte)4 ,(lte)e3,(lte)(K|B|Q)   ,(lte)f4,(lte)(B|Q) ,(lte)g5,(lte)(B|Q) ,(lte)h6,(lte)(B|Q)
    ,(lte)1 ,(lte)e1,(lte)(K|P|B|Q)
};
static const lte attacks_black_d3[] = {
(lte)8
    ,(lte)3 ,(lte)c3,(lte)(K|R|Q)   ,(lte)b3,(lte)(R|Q) ,(lte)a3,(lte)(R|Q)
    ,(lte)4 ,(lte)e3,(lte)(K|R|Q)   ,(lte)f3,(lte)(R|Q) ,(lte)g3,(lte)(R|Q) ,(lte)h3,(lte)(R|Q)
    ,(lte)2 ,(lte)d2,(lte)(K|R|Q)   ,(lte)d1,(lte)(R|Q)
    ,(lte)5 ,(lte)d4,(lte)(K|R|Q)   ,(lte)d5,(lte)(R|Q) ,(lte)d6,(lte)(R|Q) ,(lte)d7,(lte)(R|Q) ,(lte)d8,(lte)(R|Q)
    ,(lte)2 ,(lte)c2,(lte)(K|P|B|Q) ,(lte)b1,(lte)(B|Q)
    ,(lte)3 ,(lte)c4,(lte)(K|B|Q)   ,(lte)b5,(lte)(B|Q) ,(lte)a6,(lte)(B|Q)
    ,(lte)4 ,(lte)e4,(lte)(K|B|Q)   ,(lte)f5,(lte)(B|Q) ,(lte)g6,(lte)(B|Q) ,(lte)h7,(lte)(B|Q)
    ,(lte)2 ,(lte)e2,(lte)(K|P|B|Q) ,(lte)f1,(lte)(B|Q)
};
static const lte attacks_black_d4[] = {
(lte)8
    ,(lte)3 ,(lte)c4,(lte)(K|R|Q)   ,(lte)b4,(lte)(R|Q) ,(lte)a4,(lte)(R|Q)
    ,(lte)4 ,(lte)e4,(lte)(K|R|Q)   ,(lte)f4,(lte)(R|Q) ,(lte)g4,(lte)(R|Q) ,(lte)h4,(lte)(R|Q)
    ,(lte)3 ,(lte)d3,(lte)(K|R|Q)   ,(lte)d2,(lte)(R|Q) ,(lte)d1,(lte)(R|Q)
    ,(lte)4 ,(lte)d5,(lte)(K|R|Q)   ,(lte)d6,(lte)(R|Q) ,(lte)d7,(lte)(R|Q) ,(lte)d8,(lte)(R|Q)
    ,(lte)3 ,(lte)c3,(lte)(K|P|B|Q) ,(lte)b2,(lte)(B|Q) ,(lte)a1,(lte)(B|Q)
    ,(lte)3 ,(lte)c5,(lte)(K|B|Q)   ,(lte)b6,(lte)(B|Q) ,(lte)a7,(lte)(B|Q)
    ,(lte)4 ,(lte)e5,(lte)(K|B|Q)   ,(lte)f6,(lte)(B|Q) ,(lte)g7,(lte)(B|Q) ,(lte)h8,(lte)(B|Q)
    ,(lte)3 ,(lte)e3,(lte)(K|P|B|Q) ,(lte)f2,(lte)(B|Q) ,(lte)g1,(lte)(B|Q)
};
static const lte attacks_black_d5[] = {
(lte)8
    ,(lte)3 ,(lte)c5,(lte)(K|R|Q)   ,(lte)b5,(lte)(R|Q) ,(lte)a5,(lte)(R|Q)
    ,(lte)4 ,(lte)e5,(lte)(K|R|Q)   ,(lte)f5,(lte)(R|Q) ,(lte)g5,(lte)(R|Q) ,(lte)h5,(lte)(R|Q)
    ,(lte)4 ,(lte)d4,(lte)(K|R|Q)   ,(lte)d3,(lte)(R|Q) ,(lte)d2,(lte)(R|Q) ,(lte)d1,(lte)(R|Q)
    ,(lte)3 ,(lte)d6,(lte)(K|R|Q)   ,(lte)d7,(lte)(R|Q) ,(lte)d8,(lte)(R|Q)
    ,(lte)3 ,(lte)c4,(lte)(K|P|B|Q) ,(lte)b3,(lte)(B|Q) ,(lte)a2,(lte)(B|Q)
    ,(lte)3 ,(lte)c6,(lte)(K|B|Q)   ,(lte)b7,(lte)(B|Q) ,(lte)a8,(lte)(B|Q)
    ,(lte)3 ,(lte)e6,(lte)(K|B|Q)   ,(lte)f7,(lte)(B|Q) ,(lte)g8,(lte)(B|Q)
    ,(lte)4 ,(lte)e4,(lte)(K|P|B|Q) ,(lte)f3,(lte)(B|Q) ,(lte)g2,(lte)(B|Q) ,(lte)h1,(lte)(B|Q)
};
static const lte attacks_black_d6[] = {
(lte)8
    ,(lte)3 ,(lte)c6,(lte)(K|R|Q)   ,(lte)b6,(lte)(R|Q) ,(lte)a6,(lte)(R|Q)
    ,(lte)4 ,(lte)e6,(lte)(K|R|Q)   ,(lte)f6,(lte)(R|Q) ,(lte)g6,(lte)(R|Q) ,(lte)h6,(lte)(R|Q)
    ,(lte)5 ,(lte)d5,(lte)(K|R|Q)   ,(lte)d4,(lte)(R|Q) ,(lte)d3,(lte)(R|Q) ,(lte)d2,(lte)(R|Q) ,(lte)d1,(lte)(R|Q)
    ,(lte)2 ,(lte)d7,(lte)(K|R|Q)   ,(lte)d8,(lte)(R|Q)
    ,(lte)3 ,(lte)c5,(lte)(K|P|B|Q) ,(lte)b4,(lte)(B|Q) ,(lte)a3,(lte)(B|Q)
    ,(lte)2 ,(lte)c7,(lte)(K|B|Q)   ,(lte)b8,(lte)(B|Q)
    ,(lte)2 ,(lte)e7,(lte)(K|B|Q)   ,(lte)f8,(lte)(B|Q)
    ,(lte)4 ,(lte)e5,(lte)(K|P|B|Q) ,(lte)f4,(lte)(B|Q) ,(lte)g3,(lte)(B|Q) ,(lte)h2,(lte)(B|Q)
};
static const lte attacks_black_d7[] = {
(lte)8
    ,(lte)3 ,(lte)c7,(lte)(K|R|Q)   ,(lte)b7,(lte)(R|Q) ,(lte)a7,(lte)(R|Q)
    ,(lte)4 ,(lte)e7,(lte)(K|R|Q)   ,(lte)f7,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)h7,(lte)(R|Q)
    ,(lte)6 ,(lte)d6,(lte)(K|R|Q)   ,(lte)d5,(lte)(R|Q) ,(lte)d4,(lte)(R|Q) ,(lte)d3,(lte)(R|Q) ,(lte)d2,(lte)(R|Q) ,(lte)d1,(lte)(R|Q)
    ,(lte)1 ,(lte)d8,(lte)(K|R|Q)
    ,(lte)3 ,(lte)c6,(lte)(K|P|B|Q) ,(lte)b5,(lte)(B|Q) ,(lte)a4,(lte)(B|Q)
    ,(lte)1 ,(lte)c8,(lte)(K|B|Q)
    ,(lte)1 ,(lte)e8,(lte)(K|B|Q)
    ,(lte)4 ,(lte)e6,(lte)(K|P|B|Q) ,(lte)f5,(lte)(B|Q) ,(lte)g4,(lte)(B|Q) ,(lte)h3,(lte)(B|Q)
};
static const lte attacks_black_d8[] = {
(lte)5
    ,(lte)3 ,(lte)c8,(lte)(K|R|Q)   ,(lte)b8,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)4 ,(lte)e8,(lte)(K|R|Q)   ,(lte)f8,(lte)(R|Q) ,(lte)g8,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)7 ,(lte)d7,(lte)(K|R|Q)   ,(lte)d6,(lte)(R|Q) ,(lte)d5,(lte)(R|Q) ,(lte)d4,(lte)(R|Q) ,(lte)d3,(lte)(R|Q) ,(lte)d2,(lte)(R|Q) ,(lte)d1,(lte)(R|Q)
    ,(lte)3 ,(lte)c7,(lte)(K|P|B|Q) ,(lte)b6,(lte)(B|Q) ,(lte)a5,(lte)(B|Q)
    ,(lte)4 ,(lte)e7,(lte)(K|P|B|Q) ,(lte)f6,(lte)(B|Q) ,(lte)g5,(lte)(B|Q) ,(lte)h4,(lte)(B|Q)
};
static const lte attacks_black_e1[] = {
(lte)5
    ,(lte)4 ,(lte)d1,(lte)(K|R|Q)   ,(lte)c1,(lte)(R|Q) ,(lte)b1,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)3 ,(lte)f1,(lte)(K|R|Q)   ,(lte)g1,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)7 ,(lte)e2,(lte)(K|R|Q)   ,(lte)e3,(lte)(R|Q) ,(lte)e4,(lte)(R|Q) ,(lte)e5,(lte)(R|Q) ,(lte)e6,(lte)(R|Q) ,(lte)e7,(lte)(R|Q) ,(lte)e8,(lte)(R|Q)
    ,(lte)4 ,(lte)d2,(lte)(K|B|Q)   ,(lte)c3,(lte)(B|Q) ,(lte)b4,(lte)(B|Q) ,(lte)a5,(lte)(B|Q)
    ,(lte)3 ,(lte)f2,(lte)(K|B|Q)   ,(lte)g3,(lte)(B|Q) ,(lte)h4,(lte)(B|Q)
};
static const lte attacks_black_e2[] = {
(lte)8
    ,(lte)4 ,(lte)d2,(lte)(K|R|Q)   ,(lte)c2,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)a2,(lte)(R|Q)
    ,(lte)3 ,(lte)f2,(lte)(K|R|Q)   ,(lte)g2,(lte)(R|Q) ,(lte)h2,(lte)(R|Q)
    ,(lte)1 ,(lte)e1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)e3,(lte)(K|R|Q)   ,(lte)e4,(lte)(R|Q) ,(lte)e5,(lte)(R|Q) ,(lte)e6,(lte)(R|Q) ,(lte)e7,(lte)(R|Q) ,(lte)e8,(lte)(R|Q)
    ,(lte)1 ,(lte)d1,(lte)(K|P|B|Q)
    ,(lte)4 ,(lte)d3,(lte)(K|B|Q)   ,(lte)c4,(lte)(B|Q) ,(lte)b5,(lte)(B|Q) ,(lte)a6,(lte)(B|Q)
    ,(lte)3 ,(lte)f3,(lte)(K|B|Q)   ,(lte)g4,(lte)(B|Q) ,(lte)h5,(lte)(B|Q)
    ,(lte)1 ,(lte)f1,(lte)(K|P|B|Q)
};
static const lte attacks_black_e3[] = {
(lte)8
    ,(lte)4 ,(lte)d3,(lte)(K|R|Q)   ,(lte)c3,(lte)(R|Q) ,(lte)b3,(lte)(R|Q) ,(lte)a3,(lte)(R|Q)
    ,(lte)3 ,(lte)f3,(lte)(K|R|Q)   ,(lte)g3,(lte)(R|Q) ,(lte)h3,(lte)(R|Q)
    ,(lte)2 ,(lte)e2,(lte)(K|R|Q)   ,(lte)e1,(lte)(R|Q)
    ,(lte)5 ,(lte)e4,(lte)(K|R|Q)   ,(lte)e5,(lte)(R|Q) ,(lte)e6,(lte)(R|Q) ,(lte)e7,(lte)(R|Q) ,(lte)e8,(lte)(R|Q)
    ,(lte)2 ,(lte)d2,(lte)(K|P|B|Q) ,(lte)c1,(lte)(B|Q)
    ,(lte)4 ,(lte)d4,(lte)(K|B|Q)   ,(lte)c5,(lte)(B|Q) ,(lte)b6,(lte)(B|Q) ,(lte)a7,(lte)(B|Q)
    ,(lte)3 ,(lte)f4,(lte)(K|B|Q)   ,(lte)g5,(lte)(B|Q) ,(lte)h6,(lte)(B|Q)
    ,(lte)2 ,(lte)f2,(lte)(K|P|B|Q) ,(lte)g1,(lte)(B|Q)
};
static const lte attacks_black_e4[] = {
(lte)8
    ,(lte)4 ,(lte)d4,(lte)(K|R|Q)   ,(lte)c4,(lte)(R|Q) ,(lte)b4,(lte)(R|Q) ,(lte)a4,(lte)(R|Q)
    ,(lte)3 ,(lte)f4,(lte)(K|R|Q)   ,(lte)g4,(lte)(R|Q) ,(lte)h4,(lte)(R|Q)
    ,(lte)3 ,(lte)e3,(lte)(K|R|Q)   ,(lte)e2,(lte)(R|Q) ,(lte)e1,(lte)(R|Q)
    ,(lte)4 ,(lte)e5,(lte)(K|R|Q)   ,(lte)e6,(lte)(R|Q) ,(lte)e7,(lte)(R|Q) ,(lte)e8,(lte)(R|Q)
    ,(lte)3 ,(lte)d3,(lte)(K|P|B|Q) ,(lte)c2,(lte)(B|Q) ,(lte)b1,(lte)(B|Q)
    ,(lte)4 ,(lte)d5,(lte)(K|B|Q)   ,(lte)c6,(lte)(B|Q) ,(lte)b7,(lte)(B|Q) ,(lte)a8,(lte)(B|Q)
    ,(lte)3 ,(lte)f5,(lte)(K|B|Q)   ,(lte)g6,(lte)(B|Q) ,(lte)h7,(lte)(B|Q)
    ,(lte)3 ,(lte)f3,(lte)(K|P|B|Q) ,(lte)g2,(lte)(B|Q) ,(lte)h1,(lte)(B|Q)
};
static const lte attacks_black_e5[] = {
(lte)8
    ,(lte)4 ,(lte)d5,(lte)(K|R|Q)   ,(lte)c5,(lte)(R|Q) ,(lte)b5,(lte)(R|Q) ,(lte)a5,(lte)(R|Q)
    ,(lte)3 ,(lte)f5,(lte)(K|R|Q)   ,(lte)g5,(lte)(R|Q) ,(lte)h5,(lte)(R|Q)
    ,(lte)4 ,(lte)e4,(lte)(K|R|Q)   ,(lte)e3,(lte)(R|Q) ,(lte)e2,(lte)(R|Q) ,(lte)e1,(lte)(R|Q)
    ,(lte)3 ,(lte)e6,(lte)(K|R|Q)   ,(lte)e7,(lte)(R|Q) ,(lte)e8,(lte)(R|Q)
    ,(lte)4 ,(lte)d4,(lte)(K|P|B|Q) ,(lte)c3,(lte)(B|Q) ,(lte)b2,(lte)(B|Q) ,(lte)a1,(lte)(B|Q)
    ,(lte)3 ,(lte)d6,(lte)(K|B|Q)   ,(lte)c7,(lte)(B|Q) ,(lte)b8,(lte)(B|Q)
    ,(lte)3 ,(lte)f6,(lte)(K|B|Q)   ,(lte)g7,(lte)(B|Q) ,(lte)h8,(lte)(B|Q)
    ,(lte)3 ,(lte)f4,(lte)(K|P|B|Q) ,(lte)g3,(lte)(B|Q) ,(lte)h2,(lte)(B|Q)
};
static const lte attacks_black_e6[] = {
(lte)8
    ,(lte)4 ,(lte)d6,(lte)(K|R|Q)   ,(lte)c6,(lte)(R|Q) ,(lte)b6,(lte)(R|Q) ,(lte)a6,(lte)(R|Q)
    ,(lte)3 ,(lte)f6,(lte)(K|R|Q)   ,(lte)g6,(lte)(R|Q) ,(lte)h6,(lte)(R|Q)
    ,(lte)5 ,(lte)e5,(lte)(K|R|Q)   ,(lte)e4,(lte)(R|Q) ,(lte)e3,(lte)(R|Q) ,(lte)e2,(lte)(R|Q) ,(lte)e1,(lte)(R|Q)
    ,(lte)2 ,(lte)e7,(lte)(K|R|Q)   ,(lte)e8,(lte)(R|Q)
    ,(lte)4 ,(lte)d5,(lte)(K|P|B|Q) ,(lte)c4,(lte)(B|Q) ,(lte)b3,(lte)(B|Q) ,(lte)a2,(lte)(B|Q)
    ,(lte)2 ,(lte)d7,(lte)(K|B|Q)   ,(lte)c8,(lte)(B|Q)
    ,(lte)2 ,(lte)f7,(lte)(K|B|Q)   ,(lte)g8,(lte)(B|Q)
    ,(lte)3 ,(lte)f5,(lte)(K|P|B|Q) ,(lte)g4,(lte)(B|Q) ,(lte)h3,(lte)(B|Q)
};
static const lte attacks_black_e7[] = {
(lte)8
    ,(lte)4 ,(lte)d7,(lte)(K|R|Q)   ,(lte)c7,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)a7,(lte)(R|Q)
    ,(lte)3 ,(lte)f7,(lte)(K|R|Q)   ,(lte)g7,(lte)(R|Q) ,(lte)h7,(lte)(R|Q)
    ,(lte)6 ,(lte)e6,(lte)(K|R|Q)   ,(lte)e5,(lte)(R|Q) ,(lte)e4,(lte)(R|Q) ,(lte)e3,(lte)(R|Q) ,(lte)e2,(lte)(R|Q) ,(lte)e1,(lte)(R|Q)
    ,(lte)1 ,(lte)e8,(lte)(K|R|Q)
    ,(lte)4 ,(lte)d6,(lte)(K|P|B|Q) ,(lte)c5,(lte)(B|Q) ,(lte)b4,(lte)(B|Q) ,(lte)a3,(lte)(B|Q)
    ,(lte)1 ,(lte)d8,(lte)(K|B|Q)
    ,(lte)1 ,(lte)f8,(lte)(K|B|Q)
    ,(lte)3 ,(lte)f6,(lte)(K|P|B|Q) ,(lte)g5,(lte)(B|Q) ,(lte)h4,(lte)(B|Q)
};
static const lte attacks_black_e8[] = {
(lte)5
    ,(lte)4 ,(lte)d8,(lte)(K|R|Q)   ,(lte)c8,(lte)(R|Q) ,(lte)b8,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)3 ,(lte)f8,(lte)(K|R|Q)   ,(lte)g8,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)7 ,(lte)e7,(lte)(K|R|Q)   ,(lte)e6,(lte)(R|Q) ,(lte)e5,(lte)(R|Q) ,(lte)e4,(lte)(R|Q) ,(lte)e3,(lte)(R|Q) ,(lte)e2,(lte)(R|Q) ,(lte)e1,(lte)(R|Q)
    ,(lte)4 ,(lte)d7,(lte)(K|P|B|Q) ,(lte)c6,(lte)(B|Q) ,(lte)b5,(lte)(B|Q) ,(lte)a4,(lte)(B|Q)
    ,(lte)3 ,(lte)f7,(lte)(K|P|B|Q) ,(lte)g6,(lte)(B|Q) ,(lte)h5,(lte)(B|Q)
};
static const lte attacks_black_f1[] = {
(lte)5
    ,(lte)5 ,(lte)e1,(lte)(K|R|Q)   ,(lte)d1,(lte)(R|Q) ,(lte)c1,(lte)(R|Q) ,(lte)b1,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)2 ,(lte)g1,(lte)(K|R|Q)   ,(lte)h1,(lte)(R|Q)
    ,(lte)7 ,(lte)f2,(lte)(K|R|Q)   ,(lte)f3,(lte)(R|Q) ,(lte)f4,(lte)(R|Q) ,(lte)f5,(lte)(R|Q) ,(lte)f6,(lte)(R|Q) ,(lte)f7,(lte)(R|Q) ,(lte)f8,(lte)(R|Q)
    ,(lte)5 ,(lte)e2,(lte)(K|B|Q)   ,(lte)d3,(lte)(B|Q) ,(lte)c4,(lte)(B|Q) ,(lte)b5,(lte)(B|Q) ,(lte)a6,(lte)(B|Q)
    ,(lte)2 ,(lte)g2,(lte)(K|B|Q)   ,(lte)h3,(lte)(B|Q)
};
static const lte attacks_black_f2[] = {
(lte)8
    ,(lte)5 ,(lte)e2,(lte)(K|R|Q)   ,(lte)d2,(lte)(R|Q) ,(lte)c2,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)a2,(lte)(R|Q)
    ,(lte)2 ,(lte)g2,(lte)(K|R|Q)   ,(lte)h2,(lte)(R|Q)
    ,(lte)1 ,(lte)f1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)f3,(lte)(K|R|Q)   ,(lte)f4,(lte)(R|Q) ,(lte)f5,(lte)(R|Q) ,(lte)f6,(lte)(R|Q) ,(lte)f7,(lte)(R|Q) ,(lte)f8,(lte)(R|Q)
    ,(lte)1 ,(lte)e1,(lte)(K|P|B|Q)
    ,(lte)5 ,(lte)e3,(lte)(K|B|Q)   ,(lte)d4,(lte)(B|Q) ,(lte)c5,(lte)(B|Q) ,(lte)b6,(lte)(B|Q) ,(lte)a7,(lte)(B|Q)
    ,(lte)2 ,(lte)g3,(lte)(K|B|Q)   ,(lte)h4,(lte)(B|Q)
    ,(lte)1 ,(lte)g1,(lte)(K|P|B|Q)
};
static const lte attacks_black_f3[] = {
(lte)8
    ,(lte)5 ,(lte)e3,(lte)(K|R|Q)   ,(lte)d3,(lte)(R|Q) ,(lte)c3,(lte)(R|Q) ,(lte)b3,(lte)(R|Q) ,(lte)a3,(lte)(R|Q)
    ,(lte)2 ,(lte)g3,(lte)(K|R|Q)   ,(lte)h3,(lte)(R|Q)
    ,(lte)2 ,(lte)f2,(lte)(K|R|Q)   ,(lte)f1,(lte)(R|Q)
    ,(lte)5 ,(lte)f4,(lte)(K|R|Q)   ,(lte)f5,(lte)(R|Q) ,(lte)f6,(lte)(R|Q) ,(lte)f7,(lte)(R|Q) ,(lte)f8,(lte)(R|Q)
    ,(lte)2 ,(lte)e2,(lte)(K|P|B|Q) ,(lte)d1,(lte)(B|Q)
    ,(lte)5 ,(lte)e4,(lte)(K|B|Q)   ,(lte)d5,(lte)(B|Q) ,(lte)c6,(lte)(B|Q) ,(lte)b7,(lte)(B|Q) ,(lte)a8,(lte)(B|Q)
    ,(lte)2 ,(lte)g4,(lte)(K|B|Q)   ,(lte)h5,(lte)(B|Q)
    ,(lte)2 ,(lte)g2,(lte)(K|P|B|Q) ,(lte)h1,(lte)(B|Q)
};
static const lte attacks_black_f4[] = {
(lte)8
    ,(lte)5 ,(lte)e4,(lte)(K|R|Q)   ,(lte)d4,(lte)(R|Q) ,(lte)c4,(lte)(R|Q) ,(lte)b4,(lte)(R|Q) ,(lte)a4,(lte)(R|Q)
    ,(lte)2 ,(lte)g4,(lte)(K|R|Q)   ,(lte)h4,(lte)(R|Q)
    ,(lte)3 ,(lte)f3,(lte)(K|R|Q)   ,(lte)f2,(lte)(R|Q) ,(lte)f1,(lte)(R|Q)
    ,(lte)4 ,(lte)f5,(lte)(K|R|Q)   ,(lte)f6,(lte)(R|Q) ,(lte)f7,(lte)(R|Q) ,(lte)f8,(lte)(R|Q)
    ,(lte)3 ,(lte)e3,(lte)(K|P|B|Q) ,(lte)d2,(lte)(B|Q) ,(lte)c1,(lte)(B|Q)
    ,(lte)4 ,(lte)e5,(lte)(K|B|Q)   ,(lte)d6,(lte)(B|Q) ,(lte)c7,(lte)(B|Q) ,(lte)b8,(lte)(B|Q)
    ,(lte)2 ,(lte)g5,(lte)(K|B|Q)   ,(lte)h6,(lte)(B|Q)
    ,(lte)2 ,(lte)g3,(lte)(K|P|B|Q) ,(lte)h2,(lte)(B|Q)
};
static const lte attacks_black_f5[] = {
(lte)8
    ,(lte)5 ,(lte)e5,(lte)(K|R|Q)   ,(lte)d5,(lte)(R|Q) ,(lte)c5,(lte)(R|Q) ,(lte)b5,(lte)(R|Q) ,(lte)a5,(lte)(R|Q)
    ,(lte)2 ,(lte)g5,(lte)(K|R|Q)   ,(lte)h5,(lte)(R|Q)
    ,(lte)4 ,(lte)f4,(lte)(K|R|Q)   ,(lte)f3,(lte)(R|Q) ,(lte)f2,(lte)(R|Q) ,(lte)f1,(lte)(R|Q)
    ,(lte)3 ,(lte)f6,(lte)(K|R|Q)   ,(lte)f7,(lte)(R|Q) ,(lte)f8,(lte)(R|Q)
    ,(lte)4 ,(lte)e4,(lte)(K|P|B|Q) ,(lte)d3,(lte)(B|Q) ,(lte)c2,(lte)(B|Q) ,(lte)b1,(lte)(B|Q)
    ,(lte)3 ,(lte)e6,(lte)(K|B|Q)   ,(lte)d7,(lte)(B|Q) ,(lte)c8,(lte)(B|Q)
    ,(lte)2 ,(lte)g6,(lte)(K|B|Q)   ,(lte)h7,(lte)(B|Q)
    ,(lte)2 ,(lte)g4,(lte)(K|P|B|Q) ,(lte)h3,(lte)(B|Q)
};
static const lte attacks_black_f6[] = {
(lte)8
    ,(lte)5 ,(lte)e6,(lte)(K|R|Q)   ,(lte)d6,(lte)(R|Q) ,(lte)c6,(lte)(R|Q) ,(lte)b6,(lte)(R|Q) ,(lte)a6,(lte)(R|Q)
    ,(lte)2 ,(lte)g6,(lte)(K|R|Q)   ,(lte)h6,(lte)(R|Q)
    ,(lte)5 ,(lte)f5,(lte)(K|R|Q)   ,(lte)f4,(lte)(R|Q) ,(lte)f3,(lte)(R|Q) ,(lte)f2,(lte)(R|Q) ,(lte)f1,(lte)(R|Q)
    ,(lte)2 ,(lte)f7,(lte)(K|R|Q)   ,(lte)f8,(lte)(R|Q)
    ,(lte)5 ,(lte)e5,(lte)(K|P|B|Q) ,(lte)d4,(lte)(B|Q) ,(lte)c3,(lte)(B|Q) ,(lte)b2,(lte)(B|Q) ,(lte)a1,(lte)(B|Q)
    ,(lte)2 ,(lte)e7,(lte)(K|B|Q)   ,(lte)d8,(lte)(B|Q)
    ,(lte)2 ,(lte)g7,(lte)(K|B|Q)   ,(lte)h8,(lte)(B|Q)
    ,(lte)2 ,(lte)g5,(lte)(K|P|B|Q) ,(lte)h4,(lte)(B|Q)
};
static const lte attacks_black_f7[] = {
(lte)8
    ,(lte)5 ,(lte)e7,(lte)(K|R|Q)   ,(lte)d7,(lte)(R|Q) ,(lte)c7,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)a7,(lte)(R|Q)
    ,(lte)2 ,(lte)g7,(lte)(K|R|Q)   ,(lte)h7,(lte)(R|Q)
    ,(lte)6 ,(lte)f6,(lte)(K|R|Q)   ,(lte)f5,(lte)(R|Q) ,(lte)f4,(lte)(R|Q) ,(lte)f3,(lte)(R|Q) ,(lte)f2,(lte)(R|Q) ,(lte)f1,(lte)(R|Q)
    ,(lte)1 ,(lte)f8,(lte)(K|R|Q)
    ,(lte)5 ,(lte)e6,(lte)(K|P|B|Q) ,(lte)d5,(lte)(B|Q) ,(lte)c4,(lte)(B|Q) ,(lte)b3,(lte)(B|Q) ,(lte)a2,(lte)(B|Q)
    ,(lte)1 ,(lte)e8,(lte)(K|B|Q)
    ,(lte)1 ,(lte)g8,(lte)(K|B|Q)
    ,(lte)2 ,(lte)g6,(lte)(K|P|B|Q) ,(lte)h5,(lte)(B|Q)
};
static const lte attacks_black_f8[] = {
(lte)5
    ,(lte)5 ,(lte)e8,(lte)(K|R|Q)   ,(lte)d8,(lte)(R|Q) ,(lte)c8,(lte)(R|Q) ,(lte)b8,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)2 ,(lte)g8,(lte)(K|R|Q)   ,(lte)h8,(lte)(R|Q)
    ,(lte)7 ,(lte)f7,(lte)(K|R|Q)   ,(lte)f6,(lte)(R|Q) ,(lte)f5,(lte)(R|Q) ,(lte)f4,(lte)(R|Q) ,(lte)f3,(lte)(R|Q) ,(lte)f2,(lte)(R|Q) ,(lte)f1,(lte)(R|Q)
    ,(lte)5 ,(lte)e7,(lte)(K|P|B|Q) ,(lte)d6,(lte)(B|Q) ,(lte)c5,(lte)(B|Q) ,(lte)b4,(lte)(B|Q) ,(lte)a3,(lte)(B|Q)
    ,(lte)2 ,(lte)g7,(lte)(K|P|B|Q) ,(lte)h6,(lte)(B|Q)
};
static const lte attacks_black_g1[] = {
(lte)5
    ,(lte)6 ,(lte)f1,(lte)(K|R|Q)   ,(lte)e1,(lte)(R|Q) ,(lte)d1,(lte)(R|Q) ,(lte)c1,(lte)(R|Q) ,(lte)b1,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)1 ,(lte)h1,(lte)(K|R|Q)
    ,(lte)7 ,(lte)g2,(lte)(K|R|Q)   ,(lte)g3,(lte)(R|Q) ,(lte)g4,(lte)(R|Q) ,(lte)g5,(lte)(R|Q) ,(lte)g6,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)g8,(lte)(R|Q)
    ,(lte)6 ,(lte)f2,(lte)(K|B|Q)   ,(lte)e3,(lte)(B|Q) ,(lte)d4,(lte)(B|Q) ,(lte)c5,(lte)(B|Q) ,(lte)b6,(lte)(B|Q) ,(lte)a7,(lte)(B|Q)
    ,(lte)1 ,(lte)h2,(lte)(K|B|Q)
};
static const lte attacks_black_g2[] = {
(lte)8
    ,(lte)6 ,(lte)f2,(lte)(K|R|Q)   ,(lte)e2,(lte)(R|Q) ,(lte)d2,(lte)(R|Q) ,(lte)c2,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)a2,(lte)(R|Q)
    ,(lte)1 ,(lte)h2,(lte)(K|R|Q)
    ,(lte)1 ,(lte)g1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)g3,(lte)(K|R|Q)   ,(lte)g4,(lte)(R|Q) ,(lte)g5,(lte)(R|Q) ,(lte)g6,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)g8,(lte)(R|Q)
    ,(lte)1 ,(lte)f1,(lte)(K|P|B|Q)
    ,(lte)6 ,(lte)f3,(lte)(K|B|Q)   ,(lte)e4,(lte)(B|Q) ,(lte)d5,(lte)(B|Q) ,(lte)c6,(lte)(B|Q) ,(lte)b7,(lte)(B|Q) ,(lte)a8,(lte)(B|Q)
    ,(lte)1 ,(lte)h3,(lte)(K|B|Q)
    ,(lte)1 ,(lte)h1,(lte)(K|P|B|Q)
};
static const lte attacks_black_g3[] = {
(lte)8
    ,(lte)6 ,(lte)f3,(lte)(K|R|Q)   ,(lte)e3,(lte)(R|Q) ,(lte)d3,(lte)(R|Q) ,(lte)c3,(lte)(R|Q) ,(lte)b3,(lte)(R|Q) ,(lte)a3,(lte)(R|Q)
    ,(lte)1 ,(lte)h3,(lte)(K|R|Q)
    ,(lte)2 ,(lte)g2,(lte)(K|R|Q)   ,(lte)g1,(lte)(R|Q)
    ,(lte)5 ,(lte)g4,(lte)(K|R|Q)   ,(lte)g5,(lte)(R|Q) ,(lte)g6,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)g8,(lte)(R|Q)
    ,(lte)2 ,(lte)f2,(lte)(K|P|B|Q) ,(lte)e1,(lte)(B|Q)
    ,(lte)5 ,(lte)f4,(lte)(K|B|Q)   ,(lte)e5,(lte)(B|Q) ,(lte)d6,(lte)(B|Q) ,(lte)c7,(lte)(B|Q) ,(lte)b8,(lte)(B|Q)
    ,(lte)1 ,(lte)h4,(lte)(K|B|Q)
    ,(lte)1 ,(lte)h2,(lte)(K|P|B|Q)
};
static const lte attacks_black_g4[] = {
(lte)8
    ,(lte)6 ,(lte)f4,(lte)(K|R|Q)   ,(lte)e4,(lte)(R|Q) ,(lte)d4,(lte)(R|Q) ,(lte)c4,(lte)(R|Q) ,(lte)b4,(lte)(R|Q) ,(lte)a4,(lte)(R|Q)
    ,(lte)1 ,(lte)h4,(lte)(K|R|Q)
    ,(lte)3 ,(lte)g3,(lte)(K|R|Q)   ,(lte)g2,(lte)(R|Q) ,(lte)g1,(lte)(R|Q)
    ,(lte)4 ,(lte)g5,(lte)(K|R|Q)   ,(lte)g6,(lte)(R|Q) ,(lte)g7,(lte)(R|Q) ,(lte)g8,(lte)(R|Q)
    ,(lte)3 ,(lte)f3,(lte)(K|P|B|Q) ,(lte)e2,(lte)(B|Q) ,(lte)d1,(lte)(B|Q)
    ,(lte)4 ,(lte)f5,(lte)(K|B|Q)   ,(lte)e6,(lte)(B|Q) ,(lte)d7,(lte)(B|Q) ,(lte)c8,(lte)(B|Q)
    ,(lte)1 ,(lte)h5,(lte)(K|B|Q)
    ,(lte)1 ,(lte)h3,(lte)(K|P|B|Q)
};
static const lte attacks_black_g5[] = {
(lte)8
    ,(lte)6 ,(lte)f5,(lte)(K|R|Q)   ,(lte)e5,(lte)(R|Q) ,(lte)d5,(lte)(R|Q) ,(lte)c5,(lte)(R|Q) ,(lte)b5,(lte)(R|Q) ,(lte)a5,(lte)(R|Q)
    ,(lte)1 ,(lte)h5,(lte)(K|R|Q)
    ,(lte)4 ,(lte)g4,(lte)(K|R|Q)   ,(lte)g3,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)g1,(lte)(R|Q)
    ,(lte)3 ,(lte)g6,(lte)(K|R|Q)   ,(lte)g7,(lte)(R|Q) ,(lte)g8,(lte)(R|Q)
    ,(lte)4 ,(lte)f4,(lte)(K|P|B|Q) ,(lte)e3,(lte)(B|Q) ,(lte)d2,(lte)(B|Q) ,(lte)c1,(lte)(B|Q)
    ,(lte)3 ,(lte)f6,(lte)(K|B|Q)   ,(lte)e7,(lte)(B|Q) ,(lte)d8,(lte)(B|Q)
    ,(lte)1 ,(lte)h6,(lte)(K|B|Q)
    ,(lte)1 ,(lte)h4,(lte)(K|P|B|Q)
};
static const lte attacks_black_g6[] = {
(lte)8
    ,(lte)6 ,(lte)f6,(lte)(K|R|Q)   ,(lte)e6,(lte)(R|Q) ,(lte)d6,(lte)(R|Q) ,(lte)c6,(lte)(R|Q) ,(lte)b6,(lte)(R|Q) ,(lte)a6,(lte)(R|Q)
    ,(lte)1 ,(lte)h6,(lte)(K|R|Q)
    ,(lte)5 ,(lte)g5,(lte)(K|R|Q)   ,(lte)g4,(lte)(R|Q) ,(lte)g3,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)g1,(lte)(R|Q)
    ,(lte)2 ,(lte)g7,(lte)(K|R|Q)   ,(lte)g8,(lte)(R|Q)
    ,(lte)5 ,(lte)f5,(lte)(K|P|B|Q) ,(lte)e4,(lte)(B|Q) ,(lte)d3,(lte)(B|Q) ,(lte)c2,(lte)(B|Q) ,(lte)b1,(lte)(B|Q)
    ,(lte)2 ,(lte)f7,(lte)(K|B|Q)   ,(lte)e8,(lte)(B|Q)
    ,(lte)1 ,(lte)h7,(lte)(K|B|Q)
    ,(lte)1 ,(lte)h5,(lte)(K|P|B|Q)
};
static const lte attacks_black_g7[] = {
(lte)8
    ,(lte)6 ,(lte)f7,(lte)(K|R|Q)   ,(lte)e7,(lte)(R|Q) ,(lte)d7,(lte)(R|Q) ,(lte)c7,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)a7,(lte)(R|Q)
    ,(lte)1 ,(lte)h7,(lte)(K|R|Q)
    ,(lte)6 ,(lte)g6,(lte)(K|R|Q)   ,(lte)g5,(lte)(R|Q) ,(lte)g4,(lte)(R|Q) ,(lte)g3,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)g1,(lte)(R|Q)
    ,(lte)1 ,(lte)g8,(lte)(K|R|Q)
    ,(lte)6 ,(lte)f6,(lte)(K|P|B|Q) ,(lte)e5,(lte)(B|Q) ,(lte)d4,(lte)(B|Q) ,(lte)c3,(lte)(B|Q) ,(lte)b2,(lte)(B|Q) ,(lte)a1,(lte)(B|Q)
    ,(lte)1 ,(lte)f8,(lte)(K|B|Q)
    ,(lte)1 ,(lte)h8,(lte)(K|B|Q)
    ,(lte)1 ,(lte)h6,(lte)(K|P|B|Q)
};
static const lte attacks_black_g8[] = {
(lte)5
    ,(lte)6 ,(lte)f8,(lte)(K|R|Q)   ,(lte)e8,(lte)(R|Q) ,(lte)d8,(lte)(R|Q) ,(lte)c8,(lte)(R|Q) ,(lte)b8,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)1 ,(lte)h8,(lte)(K|R|Q)
    ,(lte)7 ,(lte)g7,(lte)(K|R|Q)   ,(lte)g6,(lte)(R|Q) ,(lte)g5,(lte)(R|Q) ,(lte)g4,(lte)(R|Q) ,(lte)g3,(lte)(R|Q) ,(lte)g2,(lte)(R|Q) ,(lte)g1,(lte)(R|Q)
    ,(lte)6 ,(lte)f7,(lte)(K|P|B|Q) ,(lte)e6,(lte)(B|Q) ,(lte)d5,(lte)(B|Q) ,(lte)c4,(lte)(B|Q) ,(lte)b3,(lte)(B|Q) ,(lte)a2,(lte)(B|Q)
    ,(lte)1 ,(lte)h7,(lte)(K|P|B|Q)
};
static const lte attacks_black_h1[] = {
(lte)3
    ,(lte)7 ,(lte)g1,(lte)(K|R|Q)   ,(lte)f1,(lte)(R|Q) ,(lte)e1,(lte)(R|Q) ,(lte)d1,(lte)(R|Q) ,(lte)c1,(lte)(R|Q) ,(lte)b1,(lte)(R|Q) ,(lte)a1,(lte)(R|Q)
    ,(lte)7 ,(lte)h2,(lte)(K|R|Q)   ,(lte)h3,(lte)(R|Q) ,(lte)h4,(lte)(R|Q) ,(lte)h5,(lte)(R|Q) ,(lte)h6,(lte)(R|Q) ,(lte)h7,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)7 ,(lte)g2,(lte)(K|B|Q)   ,(lte)f3,(lte)(B|Q) ,(lte)e4,(lte)(B|Q) ,(lte)d5,(lte)(B|Q) ,(lte)c6,(lte)(B|Q) ,(lte)b7,(lte)(B|Q) ,(lte)a8,(lte)(B|Q)
};
static const lte attacks_black_h2[] = {
(lte)5
    ,(lte)7 ,(lte)g2,(lte)(K|R|Q)   ,(lte)f2,(lte)(R|Q) ,(lte)e2,(lte)(R|Q) ,(lte)d2,(lte)(R|Q) ,(lte)c2,(lte)(R|Q) ,(lte)b2,(lte)(R|Q) ,(lte)a2,(lte)(R|Q)
    ,(lte)1 ,(lte)h1,(lte)(K|R|Q)
    ,(lte)6 ,(lte)h3,(lte)(K|R|Q)   ,(lte)h4,(lte)(R|Q) ,(lte)h5,(lte)(R|Q) ,(lte)h6,(lte)(R|Q) ,(lte)h7,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)1 ,(lte)g1,(lte)(K|P|B|Q)
    ,(lte)6 ,(lte)g3,(lte)(K|B|Q)   ,(lte)f4,(lte)(B|Q) ,(lte)e5,(lte)(B|Q) ,(lte)d6,(lte)(B|Q) ,(lte)c7,(lte)(B|Q) ,(lte)b8,(lte)(B|Q)
};
static const lte attacks_black_h3[] = {
(lte)5
    ,(lte)7 ,(lte)g3,(lte)(K|R|Q)   ,(lte)f3,(lte)(R|Q) ,(lte)e3,(lte)(R|Q) ,(lte)d3,(lte)(R|Q) ,(lte)c3,(lte)(R|Q) ,(lte)b3,(lte)(R|Q) ,(lte)a3,(lte)(R|Q)
    ,(lte)2 ,(lte)h2,(lte)(K|R|Q)   ,(lte)h1,(lte)(R|Q)
    ,(lte)5 ,(lte)h4,(lte)(K|R|Q)   ,(lte)h5,(lte)(R|Q) ,(lte)h6,(lte)(R|Q) ,(lte)h7,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)2 ,(lte)g2,(lte)(K|P|B|Q) ,(lte)f1,(lte)(B|Q)
    ,(lte)5 ,(lte)g4,(lte)(K|B|Q)   ,(lte)f5,(lte)(B|Q) ,(lte)e6,(lte)(B|Q) ,(lte)d7,(lte)(B|Q) ,(lte)c8,(lte)(B|Q)
};
static const lte attacks_black_h4[] = {
(lte)5
    ,(lte)7 ,(lte)g4,(lte)(K|R|Q)   ,(lte)f4,(lte)(R|Q) ,(lte)e4,(lte)(R|Q) ,(lte)d4,(lte)(R|Q) ,(lte)c4,(lte)(R|Q) ,(lte)b4,(lte)(R|Q) ,(lte)a4,(lte)(R|Q)
    ,(lte)3 ,(lte)h3,(lte)(K|R|Q)   ,(lte)h2,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)4 ,(lte)h5,(lte)(K|R|Q)   ,(lte)h6,(lte)(R|Q) ,(lte)h7,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)3 ,(lte)g3,(lte)(K|P|B|Q) ,(lte)f2,(lte)(B|Q) ,(lte)e1,(lte)(B|Q)
    ,(lte)4 ,(lte)g5,(lte)(K|B|Q)   ,(lte)f6,(lte)(B|Q) ,(lte)e7,(lte)(B|Q) ,(lte)d8,(lte)(B|Q)
};
static const lte attacks_black_h5[] = {
(lte)5
    ,(lte)7 ,(lte)g5,(lte)(K|R|Q)   ,(lte)f5,(lte)(R|Q) ,(lte)e5,(lte)(R|Q) ,(lte)d5,(lte)(R|Q) ,(lte)c5,(lte)(R|Q) ,(lte)b5,(lte)(R|Q) ,(lte)a5,(lte)(R|Q)
    ,(lte)4 ,(lte)h4,(lte)(K|R|Q)   ,(lte)h3,(lte)(R|Q) ,(lte)h2,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)3 ,(lte)h6,(lte)(K|R|Q)   ,(lte)h7,(lte)(R|Q) ,(lte)h8,(lte)(R|Q)
    ,(lte)4 ,(lte)g4,(lte)(K|P|B|Q) ,(lte)f3,(lte)(B|Q) ,(lte)e2,(lte)(B|Q) ,(lte)d1,(lte)(B|Q)
    ,(lte)3 ,(lte)g6,(lte)(K|B|Q)   ,(lte)f7,(lte)(B|Q) ,(lte)e8,(lte)(B|Q)
};
static const lte attacks_black_h6[] = {
(lte)5
    ,(lte)7 ,(lte)g6,(lte)(K|R|Q)   ,(lte)f6,(lte)(R|Q) ,(lte)e6,(lte)(R|Q) ,(lte)d6,(lte)(R|Q) ,(lte)c6,(lte)(R|Q) ,(lte)b6,(lte)(R|Q) ,(lte)a6,(lte)(R|Q)
    ,(lte)5 ,(lte)h5,(lte)(K|R|Q)   ,(lte)h4,(lte)(R|Q) ,(lte)h3,(lte)(R|Q) ,(lte)h2,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)2 ,(lte)h7,(lte)(K|R|Q)   ,(lte)h8,(lte)(R|Q)
    ,(lte)5 ,(lte)g5,(lte)(K|P|B|Q) ,(lte)f4,(lte)(B|Q) ,(lte)e3,(lte)(B|Q) ,(lte)d2,(lte)(B|Q) ,(lte)c1,(lte)(B|Q)
    ,(lte)2 ,(lte)g7,(lte)(K|B|Q)   ,(lte)f8,(lte)(B|Q)
};
static const lte attacks_black_h7[] = {
(lte)5
    ,(lte)7 ,(lte)g7,(lte)(K|R|Q)   ,(lte)f7,(lte)(R|Q) ,(lte)e7,(lte)(R|Q) ,(lte)d7,(lte)(R|Q) ,(lte)c7,(lte)(R|Q) ,(lte)b7,(lte)(R|Q) ,(lte)a7,(lte)(R|Q)
    ,(lte)6 ,(lte)h6,(lte)(K|R|Q)   ,(lte)h5,(lte)(R|Q) ,(lte)h4,(lte)(R|Q) ,(lte)h3,(lte)(R|Q) ,(lte)h2,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)1 ,(lte)h8,(lte)(K|R|Q)
    ,(lte)6 ,(lte)g6,(lte)(K|P|B|Q) ,(lte)f5,(lte)(B|Q) ,(lte)e4,(lte)(B|Q) ,(lte)d3,(lte)(B|Q) ,(lte)c2,(lte)(B|Q) ,(lte)b1,(lte)(B|Q)
    ,(lte)1 ,(lte)g8,(lte)(K|B|Q)
};
static const lte attacks_black_h8[] = {
(lte)3
    ,(lte)7 ,(lte)g8,(lte)(K|R|Q)   ,(lte)f8,(lte)(R|Q) ,(lte)e8,(lte)(R|Q) ,(lte)d8,(lte)(R|Q) ,(lte)c8,(lte)(R|Q) ,(lte)b8,(lte)(R|Q) ,(lte)a8,(lte)(R|Q)
    ,(lte)7 ,(lte)h7,(lte)(K|R|Q)   ,(lte)h6,(lte)(R|Q) ,(lte)h5,(lte)(R|Q) ,(lte)h4,(lte)(R|Q) ,(lte)h3,(lte)(R|Q) ,(lte)h2,(lte)(R|Q) ,(lte)h1,(lte)(R|Q)
    ,(lte)7 ,(lte)g7,(lte)(K|P|B|Q) ,(lte)f6,(lte)(B|Q) ,(lte)e5,(lte)(B|Q) ,(lte)d4,(lte)(B|Q) ,(lte)c3,(lte)(B|Q) ,(lte)b2,(lte)(B|Q) ,(lte)a1,(lte)(B|Q)
};

// attacks_black_lookup
const lte *attacks_black_lookup[] = {
    attacks_black_a8,
    attacks_black_b8,
    attacks_black_c8,
    attacks_black_d8,
    attacks_black_e8,
    attacks_black_f8,
    attacks_black_g8,
    attacks_black_h8,
    attacks_black_a7,
    attacks_black_b7,
    attacks_black_c7,
    attacks_black_d7,
    attacks_black_e7,
    attacks_black_f7,
    attacks_black_g7,
    attacks_black_h7,
    attacks_black_a6,
    attacks_black_b6,
    attacks_black_c6,
    attacks_black_d6,
    attacks_black_e6,
    attacks_black_f6,
    attacks_black_g6,
    attacks_black_h6,
    attacks_black_a5,
    attacks_black_b5,
    attacks_black_c5,
    attacks_black_d5,
    attacks_black_e5,
    attacks_black_f5,
    attacks_black_g5,
    attacks_black_h5,
    attacks_black_a4,
    attacks_black_b4,
    attacks_black_c4,
    attacks_black_d4,
    attacks_black_e4,
    attacks_black_f4,
    attacks_black_g4,
    attacks_black_h4,
    attacks_black_a3,
    attacks_black_b3,
    attacks_black_c3,
    attacks_black_d3,
    attacks_black_e3,
    attacks_black_f3,
    attacks_black_g3,
    attacks_black_h3,
    attacks_black_a2,
    attacks_black_b2,
    attacks_black_c2,
    attacks_black_d2,
    attacks_black_e2,
    attacks_black_f2,
    attacks_black_g2,
    attacks_black_h2,
    attacks_black_a1,
    attacks_black_b1,
    attacks_black_c1,
    attacks_black_d1,
    attacks_black_e1,
    attacks_black_f1,
    attacks_black_g1,
    attacks_black_h1,
};
