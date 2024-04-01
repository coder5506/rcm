// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "screen.h"

#include <chrono>
#include <cstdio>
#include <cstring>

#include <alloca.h>

using namespace std;

// E-Paper updates can be slow, and we don't want to block, so we offload
// them to a separate thread.
void Screen::update_epd2in9d() {
    epd2in9d.wake();

    const auto width_bytes = (SCREEN_WIDTH + 7) / 8;
    const auto size_bytes  = width_bytes * SCREEN_HEIGHT;

    const auto new_image = (uint8_t*)alloca(size_bytes);
    const auto old_image = (uint8_t*)alloca(size_bytes);
    memset(old_image, -1, size_bytes);
    memset(new_image, -1, size_bytes);

    {
        lock_guard<std::mutex> lock(mutex);
        memcpy(new_image, image[0]->data(), size_bytes);
    }
    memcpy(old_image, new_image, size_bytes);
    epd2in9d.display(old_image);

    struct timespec last_render;
    clock_gettime(CLOCK_REALTIME, &last_render);

    while (!shutdown) {
        auto timeout = chrono::system_clock::now() + chrono::seconds(100);
        {
            unique_lock<std::mutex> lock(mutex);
            cond.wait_until(lock, timeout);
            if (shutdown) {
                break;
            }
            memcpy(new_image, image[0]->data(), size_bytes);
        }

        const bool dirty = memcmp(new_image, old_image, size_bytes) != 0;
        if (!dirty) {
            continue;
        }

        memcpy(old_image, new_image, size_bytes);
        epd2in9d.update(old_image);

        // Eventually we'll want to sleep if nothing is happening.
        clock_gettime(CLOCK_REALTIME, &last_render);
    }
}

Screen::~Screen() {
    shutdown = true;
    cond.notify_one();
    thread.join();
}

Screen::Screen()
{
    image[0] = make_unique<Image>(SCREEN_WIDTH, SCREEN_HEIGHT);
    image[1] = make_unique<Image>(SCREEN_WIDTH, SCREEN_HEIGHT);
    context.rotate = ROTATE_180;
    thread = std::thread(&Screen::update_epd2in9d, this);
}

void Screen::render(View& view) {
    {
        lock_guard<std::mutex> lock(mutex);

        swap(image[0], image[1]);
        context.image = image[0].get();
        context.clear();
        view.render(context);
    }

    if (*image[0] != *image[1]) {
        cond.notify_all();
        changed();
    }
}

int Screen::png(uint8_t** png, size_t* size) const {
    lock_guard<std::mutex> lock(mutex);
    return image[0]->png(png, size);
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
