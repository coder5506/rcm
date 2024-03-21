// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "screen.h"
#include "epd2in9d.h"
#include "graphics.h"

#include <alloca.h>
#include <chrono>
#include <cstdio>
#include <cstring>

struct Screen screen;

Screen::Screen() {
    image[0] = nullptr;
    image[1] = nullptr;
    context = nullptr;
    shutdown = false;
}

// E-Paper updates can be slow, and we don't want to block, so we offload
// them to a separate thread.
static void update_epd2in9d() {
    const int width_bytes = (SCREEN_WIDTH + 7) / 8;
    const int size_bytes  = width_bytes * SCREEN_HEIGHT;

    uint8_t *const new_image = (uint8_t*)alloca(size_bytes);
    uint8_t *const old_image = (uint8_t*)alloca(size_bytes);
    memset(old_image, -1, size_bytes);
    memset(new_image, -1, size_bytes);

    epd2in9d_init();

    {
        std::lock_guard<std::mutex> lock(screen.mutex);
        memcpy(new_image, screen.image[0]->data, size_bytes);
    }
    memcpy(old_image, new_image, size_bytes);
    epd2in9d_display(old_image);

    struct timespec last_render;
    clock_gettime(CLOCK_REALTIME, &last_render);

    while (!screen.shutdown) {
        auto timeout = std::chrono::system_clock::now() + std::chrono::seconds(100);
        {
            std::unique_lock<std::mutex> lock(screen.mutex);
            screen.cond.wait_until(lock, timeout);
            if (screen.shutdown) {
                break;
            }
            memcpy(new_image, screen.image[0]->data, size_bytes);
        }

        const bool dirty = memcmp(new_image, old_image, size_bytes) != 0;
        if (!dirty) {
            continue;
        }

        memcpy(old_image, new_image, size_bytes);
        epd2in9d_update(old_image);

        // Eventually we'll want to sleep if nothing is happening.
        clock_gettime(CLOCK_REALTIME, &last_render);
    }
}

static void signal_thread(void) {
    screen.cond.notify_one();
}

static void shutdown_thread(void) {
    screen.shutdown = true;
    signal_thread();
    screen.thread.join();
}

void screen_close(void) {
    shutdown_thread();
    epd2in9d_sleep();
    epd2in9d_close();
}

static void start_thread(void) {
    screen.shutdown = false;
    screen.thread = std::thread(update_epd2in9d);
}

int screen_open(void) {
    if (epd2in9d_open() != 0) {
        return 1;
    }

    for (int i = 0; i != 2; ++i) {
        screen.image[i] = image_alloc(SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    screen.context = context_alloc();
    screen.context->rotate = ROTATE_180;

    start_thread();
    return 0;
}

void screen_render(struct View *view) {
    {
        std::lock_guard<std::mutex> lock(screen.mutex);

        struct Image *image = screen.image[1];
        screen.image[1] = screen.image[0];
        screen.image[0] = image;

        screen.context->image = screen.image[0];
        graphics_clear(screen.context);
        if (view) {
            view->render(view, screen.context);
        }
    }

    if (!image_equal(screen.image[0], screen.image[1])) {
        signal_thread();
        screen.changed();
    }
}

int screen_png(uint8_t **png, size_t *size) {
    std::lock_guard<std::mutex> lock(screen.mutex);
    return image_png(screen.image[0], png, size);
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
