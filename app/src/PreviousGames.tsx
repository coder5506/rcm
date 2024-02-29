// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { useAppDispatch } from './store/hooks'
import { useState } from 'react'
import * as board from './store/boardSlice'
import * as display from './store/displaySlice'

type Game = {
  black: string
  created_at: string
  event: string
  id: number
  result: string
  round: string
  site: string
  source: string
  white: string
}

export const PreviousGames = () => {
  const dispatch = useAppDispatch()
  const [games, setGames] = useState<Game[]>([])

  const loadGame = (_id: number) => {
    dispatch(board.setSynchronized(false))
    dispatch(display.showDrawer(false))
  }

  const removeGame = (id: number) => {
    setGames(games.filter((each) => each.id !== id))
  }

  return (
    <div className="bg-base-100 h-full overflow-y-auto z-20">
      <ul className="menu">
        {games.map(({ black, created_at, id, result, white }) => (
          <li key={id}>
            <div>
              <button
                className="btn btn-circle btn-error btn-outline btn-xs"
                onClick={() => removeGame(id)}
              >
                🗑
              </button>
              <div onClick={() => loadGame(id)}>
                <h3 className="font-bold">
                  {white} vs. {black} {result}
                </h3>
                {created_at}
              </div>
            </div>
          </li>
        ))}
      </ul>
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
