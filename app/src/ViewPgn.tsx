// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { useAppDispatch, useAppSelector } from './store/hooks'
import * as display from './store/displaySlice'
import CodeMirror from '@uiw/react-codemirror'

// Only available in secure contexts
const hasClipboard = () => navigator?.clipboard !== undefined

const openLichess = () => window.open('https://lichess.org/paste', '_blank')

export const ViewPgn = () => {
  const dialogs = useAppSelector(display.selectDialogs)
  const dispatch = useAppDispatch()

  const onClose = () => dispatch(display.showDialogs({}))
  const onCopy = () => navigator.clipboard.writeText(dialogs['viewPgn'])
  const onCopyAndGo = async () => {
    await onCopy()
    openLichess()
    onClose()
  }

  return (
    <dialog className={`modal ${dialogs['viewPgn'] ? 'modal-open' : ''}`}>
      <div className="modal-box">
        <h3 className="font-bold">View current PGN</h3>
        <CodeMirror autoFocus={true} readOnly={true} value={dialogs['viewPgn']} />
        <div className="modal-action">
          {hasClipboard() ?
            <>
              <button className="btn" onClick={onCopy}>
                Copy
              </button>
              <button className="btn" onClick={onCopyAndGo}>
                Copy &amp; go
              </button>
            </>
          : <button className="btn" onClick={openLichess}>
              Lichess
            </button>
          }
          <button className="btn btn-primary" onClick={onClose}>
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
