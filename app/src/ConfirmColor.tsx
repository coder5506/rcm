// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { useAppDispatch, useAppSelector } from './store/hooks'
import * as display from './store/displaySlice'

export const ConfirmColor = () => {
  const dialogs = useAppSelector(display.selectDialogs)
  const dispatch = useAppDispatch()
  const execute = dialogs['color']

  const close = () => dispatch(display.showDialogs({}))
  const play = (_color) => {
    close()
  }

  return (
    <dialog className={`modal ${execute ? 'modal-open' : ''}`}>
      <div className="modal-box">
        <h3 className="font-bold">What color do you play?</h3>
        <div className="modal-action">
          <button className="btn" onClick={() => play('white')}>
            WHITE
          </button>
          <button className="btn" onClick={() => play('black')}>
            BLACK
          </button>
          <button className="btn btn-primary" onClick={close}>
            Cancel
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
