// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { useAppDispatch, useAppSelector } from './store/hooks'
import * as display from './store/displaySlice'

export const WebSettings = () => {
  const dialogs = useAppSelector(display.selectDialogs)
  const dispatch = useAppDispatch()
  const settings = useAppSelector(display.selectSettings)

  return (
    <dialog className={`modal ${dialogs['webSettings'] ? 'modal-open' : ''}`}>
      <div className="modal-box">
        <h3 className="font-bold">🕸 Web settings</h3>
        <div>
          <ul className="menu">
            {display.webSettings.map(({ id, label }) => (
              <li key={id}>
                <label>
                  <input
                    checked={settings[id]}
                    className="toggle"
                    type="checkbox"
                    onChange={(e) =>
                      dispatch(display.updateSettings({ [id]: e.target.checked }))
                    }
                  />
                  {label}
                </label>
              </li>
            ))}
          </ul>
        </div>
        <div className="modal-action">
          <button
            className="btn btn-primary"
            onClick={() => dispatch(display.showDialogs({}))}
          >
            Close
          </button>
        </div>
      </div>
    </dialog>
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
