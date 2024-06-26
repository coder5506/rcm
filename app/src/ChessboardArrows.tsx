// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { useAppSelector } from './store/hooks'
import { useEffect, useRef } from 'react'
import * as board from './store/boardSlice'
import * as chessboard from './store/chessboardSlice'
import * as display from './store/displaySlice'

const parseFile = (c: string) => 'abcdefgh'.indexOf(c)
const parseCol = (c: string) => parseFile(c) + 1

export const ChessboardArrows = ({ canvasSize = 550, ...props }) => {
  const canvas = useRef<HTMLCanvasElement | null>(null)
  const kingsChecks = useAppSelector(display.selectSetting('kingsChecks'))
  const moveHighlight = useAppSelector(chessboard.selectMoveHighlight)
  const previousMove = useAppSelector(display.selectSetting('previousMove'))
  const reversedBoard = useAppSelector(display.selectReversedBoard)
  const turn = useAppSelector(board.selectTurn)

  const getSquareSize = () => ((canvas.current?.clientWidth ?? 0) - 2) / 8

  const clearGraphics = () => {
    if (!canvas.current) {
      return
    }

    const ctx = canvas.current.getContext('2d')
    if (ctx) {
      ctx.clearRect(0, 0, canvas.current.width, canvas.current.height)
    }
  }

  const getContext = (color: string) => {
    if (!canvas.current) {
      return null
    }

    const ctx = canvas.current.getContext('2d')
    if (ctx) {
      ctx.fillStyle = color
      ctx.strokeStyle = color
    }
    return ctx
  }

  const drawGraphicArrow = (move: string, color: string) => {
    const ctx = getContext(color)
    if (!ctx) {
      return
    }

    ctx.lineCap = 'round'
    ctx.lineWidth = 15

    const sqsize = getSquareSize()

    const _lineSegment = (x0, y0, x1, y1) => {
      ctx.beginPath()
      ctx.moveTo((x0 - 0.5) * sqsize, (y0 - 0.5) * sqsize)
      ctx.lineTo((x1 - 0.5) * sqsize, (y1 - 0.5) * sqsize)
      ctx.stroke()
    }

    const head_angle = Math.PI / 6
    const head_len = 15
    const width = 15

    const _arrowHead = (x0, y0, x1, y1) => {
      x0 = (x0 - 0.5) * sqsize
      y0 = (y0 - 0.5) * sqsize
      x1 = (x1 - 0.5) * sqsize
      y1 = (y1 - 0.5) * sqsize

      const angle = Math.atan2(y1 - y0, x1 - x0)
      x1 -= width * Math.cos(angle)
      y1 -= width * Math.sin(angle)

      ctx.beginPath()
      ctx.moveTo(x1, y1)
      ctx.lineTo(
        x1 - head_len * Math.cos(angle - head_angle),
        y1 - head_len * Math.sin(angle - head_angle),
      )
      ctx.lineTo(
        x1 - head_len * Math.cos(angle + head_angle),
        y1 - head_len * Math.sin(angle + head_angle),
      )
      ctx.closePath()
      ctx.stroke()
      ctx.fill()
    }

    const reversed = reversedBoard
    const shift_value = 0.2
    do {
      const sarrow = move.substring(0, 4)
      move = move.substring(5)

      let x0 = parseCol(sarrow[0])
      let y0 = 9 - parseInt(sarrow[1])
      let x1 = parseCol(sarrow[2])
      let y1 = 9 - parseInt(sarrow[3])

      if (reversed) {
        x0 = 9 - x0
        y0 = 9 - y0
        x1 = 9 - x1
        y1 = 9 - y1
      }

      const _x0 = x0
      const _y0 = y0
      const _x1 = x1
      const _y1 = y1

      if (x1 > x0) {
        x0 = x0 + shift_value
        x1 = x1 - shift_value
      }
      if (x1 < x0) {
        x0 = x0 - shift_value
        x1 = x1 + shift_value
      }
      if (y1 > y0) {
        y0 = y0 + shift_value
        y1 = y1 - shift_value
      }
      if (y1 < y0) {
        y0 = y0 - shift_value
        y1 = y1 + shift_value
      }

      let singleline = sarrow[2] + sarrow[3] === move[0] + move[1]

      // last arrow to draw
      if (move.length === 0) {
        // special case for knight moves
        if (Math.abs(_x0 - _x1) === 2 && Math.abs(_y0 - _y1) === 1) {
          if (y1 > y0) {
            y1 = y1 + shift_value
          }
          if (y1 < y0) {
            y1 = y1 - shift_value
          }

          _lineSegment(x0, _y0, x1, y0)
          singleline = false
          x0 = x1
        } else if (Math.abs(_x0 - _x1) === 1 && Math.abs(_y0 - _y1) === 2) {
          if (x1 > x0) {
            x1 = x1 + shift_value
          }
          if (x1 < x0) {
            x1 = x1 - shift_value
          }

          _lineSegment(_x0, y0, x0, y1)
          singleline = false
          y0 = y1
        }
      }

      _lineSegment(x0, y0, x1, y1)
      if (!singleline) {
        _arrowHead(x0, y0, x1, y1)
      }
    } while (move.length > 4)
  }

  const highlightSquare = (san: string, color: string) => {
    let x = parseFile(san.charAt(0))
    let y = 8 - parseInt(san.charAt(1))
    if (reversedBoard) {
      x = 7 - x
      y = 7 - y
    }

    const sqsize = getSquareSize()
    const ctx = getContext(color)
    if (!ctx) {
      return
    }

    ctx.lineWidth = 10
    ctx.beginPath()
    ctx.rect(x * sqsize - 1, y * sqsize - 1, sqsize, sqsize)
    ctx.stroke()
  }

  const highlighters = {
    checkers({ checkers, kings }) {
      if (kingsChecks) {
        const kingSquare = kings[1 - turn]
        highlightSquare(kingSquare, 'red')
        checkers.forEach((checker: string) => {
          drawGraphicArrow(checker + kingSquare, 'red')
        })
      }
    },

    computer_uci_move(uci_move: string) {
      drawGraphicArrow(uci_move, 'yellow')
    },

    tip_uci_move(uci_move: string) {
      drawGraphicArrow(uci_move, 'green')
    },

    tip_uci_moves(uci_moves: string[]) {
      uci_moves.forEach((uci_move) => {
        highlightSquare(uci_move.substring(0, 2), 'green')
        drawGraphicArrow(uci_move, 'green')
      })
    },

    uci_move(uci_move: string) {
      if (previousMove) {
        highlightSquare(uci_move.substring(0, 2), 'black')
        drawGraphicArrow(uci_move, 'black')
      }
    },

    uci_undo_move(uci_move: string) {
      drawGraphicArrow(uci_move.substring(0, 2), 'orange')
      drawGraphicArrow(uci_move, 'orange')
    },
  }

  useEffect(() => {
    clearGraphics()
    for (const key in highlighters) {
      if (moveHighlight[key]) {
        highlighters[key](moveHighlight[key])
        return
      }
    }
  }, [moveHighlight, reversedBoard])

  return <canvas {...props} ref={canvas} height={canvasSize} width={canvasSize} />
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
