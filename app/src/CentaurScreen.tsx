// Copyright (C) 2024 Eric Sessoms
// See license at end of file

import {
  ArrowUturnLeftIcon,
  CheckIcon,
  ChevronDownIcon,
  ChevronUpIcon,
  PlayPauseIcon,
  QuestionMarkCircleIcon,
} from '@heroicons/react/24/outline'
import { events } from './events'
import { useEffect, useState } from 'react'

const pushButton = (_web_button: number) => {}

export const CentaurScreen = () => {
  const [timestamp, setTimestamp] = useState(new Date().getTime())
  const imageUrl = `/api/screen?${timestamp}`

  useEffect(() => {
    const updated = (_event: MessageEvent) => {
      setTimestamp(new Date().getTime())
    }
    events.addEventListener('screen', updated)
    return () => events.removeEventListener('screen', updated)
  }, [])

  return (
    <div className="align-items-center flex flex-col">
      <img className="rotate-180" src={imageUrl} width="128" height="296" />
      <div className="grid grid-cols-3">
        <ChevronUpIcon
          className="btn btn-outline btn-sm m-1 col-start-2"
          onClick={() => pushButton(3)}
        />
        <ArrowUturnLeftIcon
          className="btn btn-outline btn-sm m-1 col-start-1"
          onClick={() => pushButton(1)}
        />
        <CheckIcon
          className="btn btn-outline btn-sm m-1 col-start-3"
          onClick={() => pushButton(6)}
        />
        <ChevronDownIcon
          className="btn btn-outline btn-sm m-1 col-start-2"
          onClick={() => pushButton(4)}
        />
        <QuestionMarkCircleIcon
          className="btn btn-outline btn-sm m-1 col-start-1"
          onClick={() => pushButton(5)}
        />
        <PlayPauseIcon
          className="btn btn-outline btn-sm m-1 col-start-3"
          onClick={() => pushButton(2)}
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
