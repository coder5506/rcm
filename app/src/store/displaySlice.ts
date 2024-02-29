// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { createSelector } from '@reduxjs/toolkit'
import { createSlice, PayloadAction } from '@reduxjs/toolkit'
import type { RootState } from './index'

const displaySettings = {
  activeBoard: false,
  centaurScreen: true,
  chatPanel: true,
  kingsChecks: false,
  liveEvaluation: true,
  pgnPanel: true,
  previousMove: true,
  reversedBoard: false,
}

export const webSettings = [
  { id: 'previousMove', label: 'Previous move displayed' },
  { id: 'kingsChecks', label: 'Kings checks displayed' },
  { id: 'liveEvaluation', label: 'Live evaluation displayed' },
  { id: 'centaurScreen', label: 'Centaur screen displayed' },
  { id: 'pgnPanel', label: 'PGN panel displayed' },
  { id: 'chatPanel', label: 'Chat panel displayed' },
  { id: 'reversedBoard', label: 'Board is reversed' },
  { id: 'activeBoard', label: 'Board is active' },
]

const initFromLocalStorage = () => {
  const json = localStorage.getItem('display')
  const data = json ? JSON.parse(json) : null
  return { ...displaySettings, ...data }
}

// Control and configure display of user-interface
const displaySlice = createSlice({
  name: 'display',
  initialState: {
    // List of alert messages to display
    alerts: [],
    // Display options for various dialogs
    dialogs: {},
    // Current release for document/display title
    release: '',
    // Display settings
    settings: initFromLocalStorage(),
    // Controls display of side-drawer
    showDrawer: false,
    // Controls display of editor panel
    showEditor: false,
  },
  reducers: {
    evaluationDisabled(state, { payload: value }: PayloadAction<boolean>) {
      state.settings.liveEvaluation = !value
    },

    release(state, { payload: value }: PayloadAction<string>) {
      state.release = value
    },

    // Dismiss alert message
    removeAlert(state, { payload: message }: PayloadAction<string>) {
      state.alerts = state.alerts.filter((alert) => alert !== message)
    },

    // Add new alert message
    showAlert(state, { payload: message }: PayloadAction<string>) {
      state.alerts = state.alerts.filter((alert) => alert !== message)
      state.alerts.push(message)
    },

    showDialogs(state, { payload: dialogs }) {
      state.dialogs = dialogs
    },

    showDrawer(state, { payload: value }: PayloadAction<boolean>) {
      state.showDrawer = value
    },

    showEditor(state, { payload: value }: PayloadAction<boolean>) {
      state.showEditor = value
    },

    showWebSettings(state) {
      state.dialogs = { webSettings }
    },

    updateSettings(state, { payload: settings }) {
      state.settings = { ...state.settings, ...settings }
      localStorage.setItem('display', JSON.stringify(state.settings))
    },
  },
})

export const displayReducer = displaySlice.reducer
export const {
  evaluationDisabled,
  release,
  removeAlert,
  showAlert,
  showDialogs,
  showDrawer,
  showEditor,
  showWebSettings,
  updateSettings,
} = displaySlice.actions

export const selectRelease = ({ display }: RootState) => display.release
export const selectTitle = createSelector(
  selectRelease,
  (release) => `DGTCentaurMods ${release}`,
)

export const selectAlerts = ({ display }: RootState) => display.alerts
export const selectDialogs = ({ display }: RootState) => display.dialogs

export const selectSettings = ({ display }: RootState) => display.settings
export const selectSetting = (id: string) =>
  createSelector(selectSettings, (settings) => settings[id])

export const selectLiveEvaluation = createSelector(
  selectSettings,
  (settings) => settings.liveEvaluation,
)

export const selectReversedBoard = createSelector(
  selectSettings,
  (settings) => settings.reversedBoard,
)

export const selectShowDrawer = ({ display }: RootState) => display.showDrawer
export const selectShowEditor = ({ display }: RootState) => display.showEditor

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
