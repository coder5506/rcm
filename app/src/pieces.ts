// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import bB from './assets/images/pieces/bB.png'
import bK from './assets/images/pieces/bK.png'
import bN from './assets/images/pieces/bN.png'
import bP from './assets/images/pieces/bP.png'
import bQ from './assets/images/pieces/bQ.png'
import bR from './assets/images/pieces/bR.png'
import wB from './assets/images/pieces/wB.png'
import wK from './assets/images/pieces/wK.png'
import wN from './assets/images/pieces/wN.png'
import wP from './assets/images/pieces/wP.png'
import wQ from './assets/images/pieces/wQ.png'
import wR from './assets/images/pieces/wR.png'

const pieces = { bB, bK, bN, bP, bQ, bR, wB, wK, wN, wP, wQ, wR }
export const pieceTheme = (piece: string) => pieces[piece]

// This file is part of the Raccoon's Centaur Mods (RCM).
//
// RCM is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// RCM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
