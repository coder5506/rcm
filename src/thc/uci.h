/****************************************************************************
 *  Author:  Bill Forster
 *  License: MIT license. Full text of license is in associated file LICENSE
 *  Copyright 2010-2020, Bill Forster <billforsternz at gmail dot com>
 ****************************************************************************/
#pragma once

#ifndef UCI_H
#define UCI_H

#include "Move.h"

#include <string>

namespace thc {

class ChessPosition;


namespace uci {
    std::string from_move(const Move&);
    thc::Move to_move(const ChessPosition&, std::string_view);
}

}

#endif
