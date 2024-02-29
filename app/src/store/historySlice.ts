// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { Chess, Move } from 'chess.js'
import { createSelector } from '@reduxjs/toolkit'
import { createSlice, PayloadAction } from '@reduxjs/toolkit'
import type { RootState } from './index'

const goPosition = (state, position: number) => {
  const { fens } = state
  const index = position < 0 ? fens.length + position : position
  return { ...state, index: Math.max(0, Math.min(fens.length - 1, index)) }
}

const historySlice = createSlice({
  name: 'history',
  initialState: {
    fens: [] as string[],
    history: [] as Move[],
    index: 0,
    pgn: '',
  },
  reducers: {
    go(state, { payload: position }: PayloadAction<number>) {
      return goPosition(state, position)
    },

    initFromPGN(state, { payload: pgn }: PayloadAction<string>) {
      const chess = new Chess()
      chess.loadPgn(pgn)
      const history = chess.history({ verbose: true })
      const fens = history.map(({ after }) => after)
      return goPosition({ ...state, fens, history, pgn }, -1)
    },

    setCurrentFEN(state, { payload: fen }: PayloadAction<string>) {
      return goPosition({ ...state, fens: [fen] }, 0)
    },
  },
})

export const { go, initFromPGN, setCurrentFEN } = historySlice.actions
export const historyReducer = historySlice.reducer

export const backward = () => (dispatch, getState) => {
  const index = selectIndex(getState())
  if (index > 0) {
    dispatch(go(index - 1))
  }
}

export const forward = () => (dispatch, getState) => {
  const index = selectIndex(getState())
  const fens = selectFENs(getState())
  if (index < fens.length - 1) {
    dispatch(go(index + 1))
  }
}

export const initFromMoves = (uciMoves: string[]) => (dispatch) => {
  const chess = new Chess()
  uciMoves.forEach((move) => {
    if (move) {
      chess.move(move)
    }
  })
  const pgn = chess.pgn({ maxWidth: 5, newline: '\n' })
  dispatch(initFromPGN(pgn))
}

export const selectFENs = ({ history }: RootState) => history.fens
export const selectIndex = ({ history }: RootState) => history.index
export const selectCurrentFEN = createSelector(
  selectFENs,
  selectIndex,
  (fens, index) => fens[index],
)

export const selectPgn = ({ history }: RootState) => history.pgn
export const selectPgnList = createSelector(selectPgn, (pgn) =>
  pgn.split('\n').map((row, i) => {
    const items = row.split(' ')
    return {
      move: i,
      wsan: items[1],
      bsan: items[2] ?? '',
    }
  }),
)

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
