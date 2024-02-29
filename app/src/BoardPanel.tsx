// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { Chessboard } from './Chessboard'
import { useAppDispatch, useAppSelector } from './store/hooks'
import { useEffect, useRef } from 'react'
import * as board from './store/boardSlice'
import * as chessboardStore from './store/chessboardSlice'
import * as display from './store/displaySlice'

// Used to score current position
const stockfish = new Worker('/stockfish/stockfish.js')

export const BoardPanel = () => {
  const boardSize = useAppSelector(chessboardStore.selectBoardSize)
  const dispatch = useAppDispatch()
  const evaluation = useAppSelector(board.selectEvaluation)
  const lastEval = useRef(0)
  const liveEvaluation = useAppSelector(display.selectLiveEvaluation)
  const synchronized = useAppSelector(board.selectSynchronized)
  const turn = useAppSelector(board.selectTurn)
  const turnCaption = useAppSelector(board.selectTurnCaption)

  // const triggerEvaluation = useCallback(() => {
  //   if (!liveEvaluation) {
  //     return
  //   }
  //
  //   // Stockfish evaluation
  //   stockfish.postMessage('position fen ' + currentFEN)
  //   stockfish.postMessage('go depth 1')
  // }, [liveEvaluation])

  useEffect(() => {
    const handleMessage = ({ data }) => {
      if (!data) {
        return
      }

      // Stockfish evaluation finishes with the bestmove message
      if (data.includes('bestmove')) {
        dispatch(board.setEvaluation(50 * lastEval.current))
      }

      // Stockfish evaluation feedback
      if (data.includes('score cp')) {
        // info depth 1 seldepth 1 multipv 1 score cp -537 nodes
        const regexp = /score cp (\d+) /gi
        const matches = regexp.exec(data)

        const MAX_VALUE = 1500
        if (matches?.length) {
          let value = parseInt(matches[1])

          // black plays?
          if (turn === 0) {
            value = -value
          }
          value = Math.max(-MAX_VALUE, Math.min(MAX_VALUE, value))
          lastEval.current = (value / (MAX_VALUE * 2)) * 100
        }
      }

      // Stockfish detected (future) mat state
      if (data.includes('score mate')) {
        let value = 50

        // black plays?
        if (turn == 0) {
          value = -value
        }
        lastEval.current = value
        dispatch(board.setEvaluation(50 * value))
      }
    }

    stockfish.addEventListener('message', handleMessage)
    return () => stockfish.removeEventListener('message', handleMessage)
  }, [turn])

  return (
    <div className="flex flex-auto flex-col overflow-hidden">
      <Chessboard />
      <div className="flex-none text-center" style={{ width: boardSize }}>
        {synchronized ?
          <div>
            <h3 className="font-bold">{turnCaption}</h3>
            {liveEvaluation && (
              <progress className="progress" max="100" value={evaluation} />
            )}
          </div>
        : <div>
            <h3 className="font-bold">--</h3>
          </div>
        }
      </div>
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
