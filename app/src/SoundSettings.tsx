// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { useState } from 'react'

export const SoundSettings = () => {
  const [sounds, setSounds] = useState([])

  const updateAppSettings = (sound, value) => {
    const newSound = { ...sound, value }
    setSounds(sounds.map((s) => (s === sound ? newSound : s)))
  }

  return (
    <dialog className={`modal ${sounds.length ? 'modal-open' : ''}`}>
      <div className="modal-box">
        <h3 className="font-bold">🎵 Board sounds</h3>
        <div>
          <ul className="menu">
            {sounds.map((sound) => (
              <li key={sound}>
                <label>
                  <input
                    checked={sound.value}
                    className="toggle"
                    type="checkbox"
                    onChange={(e) => updateAppSettings(sound, e.target.checked)}
                  />
                  {sound.label}
                </label>
              </li>
            ))}
          </ul>
        </div>
        <div className="modal-action">
          <button className="btn btn-primary" onClick={() => setSounds([])}>
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
