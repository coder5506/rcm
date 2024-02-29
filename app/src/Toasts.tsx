// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { useAppDispatch, useAppSelector } from './store/hooks'
import * as display from './store/displaySlice'

export const Toasts = () => {
  const alerts = useAppSelector(display.selectAlerts)
  const dispatch = useAppDispatch()

  return (
    <div className="toast">
      {alerts.map((alert) => (
        <div key={alert} className="alert alert-info">
          <span className="info-content">{alert}</span>
          <div>
            <button
              className="btn btn-primary btn-sm"
              onClick={() => dispatch(display.removeAlert(alert))}
            >
              Got it!
            </button>
          </div>
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
