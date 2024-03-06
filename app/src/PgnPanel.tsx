// Copyright (C) 2024 Eric Sessoms
// See license at end of file

import './PgnPanel.css'
import { useAppDispatch, useAppSelector } from './store/hooks'
import * as history from './store/historySlice'

export const PgnPanel = () => {
  const dispatch = useAppDispatch()
  const index = useAppSelector(history.selectIndex)
  const pgnList = useAppSelector(history.selectPgnList)

  const go = (move: number) => dispatch(history.go(move))

  return (
    <div className="h-full min-w-48 overflow-y-auto">
      {pgnList.map(({ move, wsan, bsan }) => (
        <div key={move}>
          <span className={move === Math.floor(index / 2) ? 'current' : ''}>
            {move + 1}.
          </span>
          &nbsp;
          <a
            className={`cursor-pointer ${index === 2 * move ? 'current' : ''}`}
            onClick={() => go(2 * move)}
          >
            {wsan}
          </a>
          &nbsp;
          <a
            className={`cursor-pointer ${index === 2 * move + 1 ? 'current' : ''}`}
            onClick={() => go(2 * move + 1)}
          >
            {bsan}
          </a>
        </div>
      ))}
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
