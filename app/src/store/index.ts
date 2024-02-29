// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { boardReducer } from './boardSlice'
import { chessboardReducer } from './chessboardSlice'
import { configureStore } from '@reduxjs/toolkit'
import { displayReducer } from './displaySlice'
import { editorReducer } from './editorSlice'
import { historyReducer } from './historySlice'
import { menuReducer } from './menuSlice'

export const store = configureStore({
  reducer: {
    board: boardReducer,
    chessboard: chessboardReducer,
    display: displayReducer,
    editor: editorReducer,
    history: historyReducer,
    menu: menuReducer,
  },
  middleware: (getDefaultMiddleware) =>
    getDefaultMiddleware({
      serializableCheck: false,
    }),
})

export type RootState = ReturnType<typeof store.getState>
export type AppDispatch = typeof store.dispatch

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
