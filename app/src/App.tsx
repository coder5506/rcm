// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { Content } from './Content'
import { Dialogs } from './Dialogs'
import { Editor } from './Editor'
import { Navbar } from './Navbar'
import { PreviousGames } from './PreviousGames'
import { Toasts } from './Toasts'
import { useAppDispatch, useAppSelector } from './store/hooks'
import * as display from './store/displaySlice'

export const App = (): JSX.Element => {
  const dispatch = useAppDispatch()
  const showDrawer = useAppSelector(display.selectShowDrawer)
  const showEditor = useAppSelector(display.selectShowEditor)

  return (
    <div className="drawer">
      <input
        checked={showDrawer}
        className="drawer-toggle"
        id="showDrawer"
        type="checkbox"
        onChange={(e) => dispatch(display.showDrawer(!!e.target.checked))}
      />
      <div className="drawer-content flex flex-col h-screen overflow-hidden">
        {showEditor ?
          <Editor />
        : <>
            <Navbar />
            <Content />
          </>
        }
        <Dialogs />
      </div>
      <div className="drawer-side z-10">
        <label className="drawer-overlay" htmlFor="showDrawer" />
        <PreviousGames />
      </div>
      <Toasts />
    </div>
  )
}

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
