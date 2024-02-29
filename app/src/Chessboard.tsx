// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import '@chrisoakman/chessboardjs/dist/chessboard-1.0.0.css'
import '@chrisoakman/chessboardjs/dist/chessboard-1.0.0.js'

import './Chessboard.css'
import { ChessboardArrows } from './ChessboardArrows'
import { pieceTheme } from './pieces'
import { useAppDispatch, useAppSelector } from './store/hooks'
import { useCallback, useEffect, useRef } from 'react'
import * as boardStore from './store/boardSlice'
import * as chessboardStore from './store/chessboardSlice'
import * as display from './store/displaySlice'
import * as history from './store/historySlice'

export const Chessboard = (props) => {
  const activeBoard = useAppSelector(display.selectSetting('activeBoard'))
  const board = useRef<HTMLDivElement | null>(null)
  const boardSize = useAppSelector(chessboardStore.selectBoardSize)
  const chessboard = useRef(null)
  const currentFEN = useAppSelector(history.selectCurrentFEN)
  const dispatch = useAppDispatch()
  const reversedBoard = useAppSelector(display.selectReversedBoard)

  useEffect(() => {
    ;(async () => {
      const response = await fetch('/api/fen')
      if (!response.ok) {
        return
      }
      const fen = await response.text()
      dispatch(history.setCurrentFEN(fen))
    })()
  }, [])

  useEffect(() => {
    const currentBoard = board.current
    const resizeObserver = new ResizeObserver(() => {
      chessboard.current?.resize()
      const actualBoard = currentBoard.querySelector('.board-b72b1')
      const boardBounds = actualBoard.getBoundingClientRect()
      dispatch(chessboardStore.setBoardSize(boardBounds.width))
    })
    resizeObserver.observe(currentBoard)
    return () => resizeObserver.unobserve(currentBoard)
  }, [])

  useEffect(() => {
    const onKeyDown = (e: KeyboardEvent) => {
      switch (e.code) {
        case 'ArrowLeft':
          dispatch(history.backward())
          e.preventDefault()
          break
        case 'ArrowRight':
          dispatch(history.forward())
          e.preventDefault()
          break
      }
    }
    document.addEventListener('keydown', onKeyDown)
    return () => document.removeEventListener('keydown', onKeyDown)
  }, [])

  const onDragStart = useCallback(() => activeBoard, [activeBoard])
  useEffect(() => {
    const onDrop = (_source: string, _target: string) => {
      dispatch(boardStore.setSynchronized(false))
      return true
    }

    const Chessboard = window['Chessboard']
    chessboard.current = Chessboard(board.current, {
      onDragStart: onDragStart,
      onDrop,
      onSnapEnd: () => 'snapback',
      pieceTheme,
      showNotation: true,
    })
  }, [])

  useEffect(() => {
    if (chessboard.current) {
      chessboard.current.draggable = activeBoard
      chessboard.current.resize()
    }
  }, [activeBoard])

  useEffect(() => {
    if (chessboard.current) {
      chessboard.current.orientation(reversedBoard ? 'black' : 'white')
      chessboard.current.resize()
    }
  }, [reversedBoard])

  useEffect(() => {
    if (chessboard.current && currentFEN) {
      chessboard.current.position(currentFEN)
    }
  }, [currentFEN])

  return (
    <div className="flex flex-auto flex-col">
      <div {...props} className="relative">
        <div ref={board} className="absolute left-0 top-0 w-full h-full" />
        <ChessboardArrows
          canvasSize={boardSize}
          className="absolute left-0 top-0 p-0 opacity-70 pointer-events-none"
        />
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
