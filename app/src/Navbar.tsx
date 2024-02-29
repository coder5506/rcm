// Copyright (c) 2024 Eric Sessoms
// See license at end of file

import { Menu } from './Menu'
import { useAppSelector } from './store/hooks'
import * as menu from './store/menuSlice'

export const Navbar = () => {
  const menuItems = useAppSelector(menu.selectMenuItems)

  const initializeMenu = (item) => {
    item = structuredClone(item)
    if (item.items) {
      item.items = item.items.map(initializeMenu)
    }
    return item
  }

  const initializedItems = menuItems.map(initializeMenu)

  return (
    <div className="bg-base-100 navbar w-full z-10">
      <div>
        <Menu className="menu-horizontal" menuItems={initializedItems} />
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
