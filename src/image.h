// Copyright (c) 2024 Eric Sessoms
// See license at end of file

#ifndef IMAGE_H
#define IMAGE_H

#include <cstddef>
#include <cstdint>

enum PixelColor {
    PIXEL_BLACK =  0,
    PIXEL_WHITE = -1,
};

struct Image {
    int width;
    int height;
    int width_bytes;
    int size_bytes;
    uint8_t data[];
};

void image_free(struct Image *image);
struct Image *image_alloc(int width, int height);

bool image_equal(const struct Image *a, const struct Image *b);

struct Image *image_readbmp(const char *path);

int image_png(const struct Image *image, uint8_t **png, size_t *size);

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
