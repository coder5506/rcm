/****************************************************************************
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/
#pragma once

#ifndef FEN_H
#define FEN_H

#include "ChessPosition.h"

#include <string>

namespace thc {


namespace fen {
    std::string from_position(const ChessPosition&);
    ChessPosition to_position(const char*);
}

}

#endif
