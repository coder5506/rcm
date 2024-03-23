// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef SCREEN_H
#define SCREEN_H

#include "epd2in9d.h"
#include "graphics.h"
#include "utility/model.h"

#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <thread>

class Screen : public Model<Screen> {
public:
    Epd2in9d    epd2in9d;
    Context     context;
    std::unique_ptr<Image>  image[2];
    std::condition_variable cond;
    mutable std::mutex  mutex;
    std::thread thread;
    bool        shutdown{false};

    // Shutdown display
    ~Screen();

    // Initialize display
    Screen();

    // Render UI to display
    void render(View& view);

    // Get PNG image of display
    int png(std::uint8_t** png, std::size_t* size) const;

private:
    void update_epd2in9d();
};

#endif

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
