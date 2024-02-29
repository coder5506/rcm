// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { createSlice, PayloadAction } from '@reduxjs/toolkit'
import type { RootState } from './index'

// Options specifically for interfacing with chessboard.js
const chessboardSlice = createSlice({
  name: 'chessboard',
  initialState: {
    boardSize: 384,
    moveHighlight: {},
  },
  reducers: {
    setBoardSize(state, { payload: boardSize }: PayloadAction<number>) {
      state.boardSize = boardSize
    },

    setMoveHighlight(state, { payload: value }: PayloadAction<object>) {
      state.moveHighlight = value
    },
  },
})

export const { setBoardSize, setMoveHighlight } = chessboardSlice.actions
export const chessboardReducer = chessboardSlice.reducer

export const selectBoardSize = ({ chessboard }: RootState) => chessboard.boardSize
export const selectMoveHighlight = ({ chessboard }: RootState) => chessboard.moveHighlight

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
