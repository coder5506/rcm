// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { Chess } from 'chess.js'
import { properties } from '@codemirror/legacy-modes/mode/properties'
import { StreamLanguage } from '@codemirror/language'
import { useAppDispatch, useAppSelector } from './store/hooks'
import { useRef } from 'react'
import * as display from './store/displaySlice'
import * as editor from './store/editorSlice'
import CodeMirror from '@uiw/react-codemirror'
import type { RootState } from './store/index'
import type { EditorState } from './store/editorSlice'

export const Editor = () => {
  const dirty = useRef(false)
  const dispatch = useAppDispatch()
  const state = useAppSelector(({ editor }: RootState) => editor) as EditorState

  // Load Codemirror extensions appropriate to filetype
  let extensions = []
  if (state.id === 'live_script') {
    extensions = []
  } else {
    switch (state.extension) {
      case 'ini':
      case 'uci':
        extensions = [StreamLanguage.define(properties)]
        break
      case 'py':
        extensions = []
        break
      default:
        break
    }
  }

  // Inform user if PGN is invalid
  let parseError = ''
  if (state.extension === 'pgn') {
    try {
      new Chess().loadPgn(state.text, { strict: true })
    } catch (e) {
      parseError = e.message
    }
  }

  const onClose = () => {
    dispatch(display.showEditor(false))
    dirty.current = false
  }

  const onDelete = () => {
    dispatch(editor.deleteFile())
    dirty.current = true
    onClose()
  }

  const onExecute = () => {
    dispatch(editor.execute())
    onClose()
  }

  const onSave = () => {
    dispatch(editor.save())
    dirty.current = true
    onClose()
  }

  const onChange = (text: string) => {
    dispatch(editor.update({ text }))
  }

  return (
    <div>
      <div className="join">
        <div className="flex flex-col join-item justify-center mx-4">
          {state.editableName ?
            <label>
              <input
                className="input input-bordered"
                value={state.newFile}
                onChange={(e) => dispatch(editor.update({ newFile: e.target.value }))}
              />
              .{state.extension}
            </label>
          : <h2 className="font-bold">
              {state.file}
              {state.extension && <span>.{state.extension}</span>}
            </h2>
          }
        </div>
        {state.canExecute && (
          <button
            className={`btn join-item ${parseError ? 'btn-disabled' : ''}`}
            onClick={onSave}
          >
            Save
          </button>
        )}
        {state.canDelete && (
          <button className="btn join-item" onClick={onDelete}>
            Delete
          </button>
        )}
        {state.canExecute && (
          <button className="btn join-item" onClick={onExecute}>
            Execute
          </button>
        )}
        <button className="btn btn-primary join-item" onClick={onClose}>
          Back
        </button>
        <div className="flex flex-col join-item justify-center mx-4">
          <h2 className="font-bold text-error">{parseError}</h2>
        </div>
      </div>
      <CodeMirror
        autoFocus={true}
        extensions={extensions}
        value={state.text}
        onChange={onChange}
      />
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
