// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "screen.h"
#include "epd2in9d.h"
#include "graphics.h"

struct Screen screen = {0};

void screen_close(void) {
    if (screen.context) {
        context_free(screen.context);
        screen.context = NULL;
    }
    for (int i = 0; i != 2; ++i) {
        if (screen.image[i]) {
            image_free(screen.image[i]);
            screen.image[i] = NULL;
        }
    }

    screen_sleep();
    epd2in9d_close();
}

int screen_open(void) {
    if (epd2in9d_open() != 0) {
        return 1;
    }

    screen.image[0] = NULL;
    screen.image[1] = NULL;
    screen.context  = NULL;

    for (int i = 0; i != 2; ++i) {
        screen.image[i] = image_alloc(SCREEN_WIDTH, SCREEN_HEIGHT);
        if (!screen.image[i]) {
            goto error;
        }
    }

    screen.context = context_alloc();
    if (!screen.context) {
        goto error;
    }
    screen.context->rotate = ROTATE_180;

    model_init(&screen.model);

    return 0;

error:
    screen_close();
    return 1;
}

struct Context *screen_context(void) {
    return screen.context;
}

void screen_sleep(void) {
    // Includes necessary delay
    epd2in9d_sleep();
}

void screen_wake(void) {
    epd2in9d_wake();
}

void screen_clear(void) {
    epd2in9d_clear();
}

void screen_render(struct View *view) {
    struct Image *image = screen.image[1];
    screen.image[1] = screen.image[0];
    screen.image[0] = image;

    screen.context->image = screen.image[0];
    graphics_clear(screen.context);
    if (view) {
        view->render(view, screen.context);
    }

    if (!image_equal(screen.image[0], screen.image[1])) {
        epd2in9d_update(screen.image[0]->data);
        model_changed(&screen.model);
    }
}

int screen_png(uint8_t **png, size_t *size) {
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
