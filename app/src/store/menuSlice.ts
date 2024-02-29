// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { createSlice, PayloadAction } from '@reduxjs/toolkit'
import type { RootState } from './index'

const mapMenuItems = (items, fn) => {
  return items.map((item) => {
    if (item.items) {
      item = { ...item, items: mapMenuItems(item.items, fn) }
    }
    return fn(item)
  })
}

const disableMenuItem = (items, pred, disabled) => {
  return mapMenuItems(items, (item) => {
    if (pred(item)) {
      return { ...item, disabled }
    }
    return item
  })
}

const enableMenuItem = (items, pred, enabled) => disableMenuItem(items, pred, !enabled)

const menuSlice = createSlice({
  name: 'menu',
  initialState: {
    menuItems: [],
  },
  reducers: {
    disableMenu(state, { payload: value }: PayloadAction<string>) {
      state.menuItems = disableMenuItem(
        state.menuItems,
        (each) => each.id === value,
        true,
      )
    },

    enableMenu(state, { payload: value }: PayloadAction<string>) {
      state.menuItems = enableMenuItem(state.menuItems, (each) => each.id === value, true)
    },

    updateMenu(state, { payload: items }: PayloadAction<[]>) {
      state.menuItems = items
    },
  },
})

export const { disableMenu, enableMenu, updateMenu } = menuSlice.actions
export const menuReducer = menuSlice.reducer

export const selectMenuItems = ({ menu }: RootState) => menu.menuItems

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
