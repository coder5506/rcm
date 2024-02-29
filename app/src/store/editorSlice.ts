// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { createSlice, PayloadAction } from '@reduxjs/toolkit'

export type EditorState = {
  canDelete: boolean
  canExecute: boolean
  editableName: string
  extension: string
  file: string
  id: string
  newFile: string
  text: string
}

// Used to edit files on the server
const editorSlice = createSlice({
  name: 'editor',
  initialState: {
    canDelete: false,
    canExecute: false,
    editableName: '',
    extension: '',
    file: '',
    id: '',
    newFile: '',
    text: '',
  } as EditorState,
  reducers: {
    update(state, { payload }: PayloadAction<Partial<EditorState>>) {
      Object.assign(state, payload)
    },
  },
})

export const { update } = editorSlice.actions
export const editorReducer = editorSlice.reducer

export const deleteFile = () => (_dispatch, _getState) => {}

export const execute = () => (_dispatch, getState) => {
  const { text } = getState().editor
  localStorage.setItem('live_script', text)
}

export const save = () => (_dispatch, _getState) => {}

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
