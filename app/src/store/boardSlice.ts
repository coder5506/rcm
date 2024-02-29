// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { createSlice, PayloadAction } from '@reduxjs/toolkit'
import * as history from './historySlice'
import type { RootState } from './index'

type BoardState = {
  evaluation: number
  loading: boolean
  plugin: string | null
  synchronized: boolean
  turn: number
  turnCaption: string
}

// Current state of the "board" (i.e., game)
const boardSlice = createSlice({
  name: 'board',
  initialState: {
    evaluation: 50,
    loading: false,
    plugin: null,
    synchronized: true,
    turn: 1, // white
    turnCaption: '--',
  } as BoardState,
  reducers: {
    setEvaluation(state, { payload: evaluation }: PayloadAction<number>) {
      state.evaluation = evaluation
    },

    setLoading(state, { payload: loading }: PayloadAction<boolean>) {
      state.loading = loading
    },

    setPlugin(state, { payload: plugin }: PayloadAction<string | null>) {
      state.plugin = plugin
    },

    setSynchronized(state, { payload: synchronized }: PayloadAction<boolean>) {
      state.synchronized = synchronized
    },

    setTurnCaption(state, { payload: turnCaption }: PayloadAction<string>) {
      state.turnCaption = turnCaption
    },

    updateBoard(
      state,
      { payload }: PayloadAction<Partial<typeof boardSlice.initialState>>,
    ) {
      Object.assign(state, payload)
    },
  },
})

export const { setEvaluation, setLoading, setPlugin, setSynchronized, setTurnCaption } =
  boardSlice.actions
export const boardReducer = boardSlice.reducer

export const setFEN = (value: string) => (dispatch, getState) => {
  const state = getState()
  const currentFEN = history.selectCurrentFEN(state)
  const synchronized = selectSynchronized(state)
  if (synchronized && currentFEN === value) {
    return
  }
  const turn = value.includes(' w ') ? 1 : 0
  const turnCaption = `turn → ${turn ? 'white' : 'black'}`
  dispatch(boardSlice.actions.updateBoard({ turn, turnCaption, synchronized: true }))
  dispatch(history.setCurrentFEN(value))
}

export const selectEvaluation = ({ board }: RootState) => (<BoardState>board).evaluation
export const selectLoading = ({ board }: RootState) => (<BoardState>board).loading
export const selectPlugin = ({ board }: RootState) => (<BoardState>board).plugin
export const selectSynchronized = ({ board }: RootState) =>
  (<BoardState>board).synchronized
export const selectTurn = ({ board }: RootState) => (<BoardState>board).turn
export const selectTurnCaption = ({ board }: RootState) => (<BoardState>board).turnCaption

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
