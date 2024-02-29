// Copyright (c) 2024 Eric Sessoms
// See license at end of file

#include "image.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <png.h>

static bool image_valid(const struct Image *image) {
    return
        image &&
        image->width  > 0 &&
        image->height > 0 &&
        image->width_bytes == (image->width + 7) / 8 &&
        image->size_bytes  == image->width_bytes * image->height;
}

void image_free(struct Image *image) {
    if (!image) {
        return;
    }
    free(image);
}

struct Image *image_alloc(int width, int height) {
    assert(width > 0 && height > 0);
    const int width_bytes = (width + 7) / 8;
    const int size_bytes  = width_bytes * height;

    struct Image *image = malloc(sizeof(struct Image) + size_bytes);
    image->width  = width;
    image->height = height;
    image->width_bytes = width_bytes;
    image->size_bytes  = size_bytes;

    assert(image_valid(image));
    return image;
}

bool image_equal(const struct Image *a, const struct Image *b) {
    assert(image_valid(a) && image_valid(b));
    if (a->width != b->width || a->height != b->height) {
        return false;
    }
    return memcmp(a->data, b->data, a->size_bytes) == 0;
}

struct BMPFILEHEADER {
    uint16_t bType;
    uint32_t bSize;
    uint16_t bReserved1;
    uint16_t bReserved2;
    uint32_t bOffset; // Offset to beginning of image data
} __attribute__((packed));

struct BMPINFOHEADER {
    uint32_t biInfoSize;
    uint32_t biWidth;    // Pixel width
    uint32_t biHeight;   // Pixel height
    uint16_t biPlanes;
    uint16_t biBitCount; // Bits per pixel
    uint32_t biCompression;
    uint32_t bimpImageSize;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} __attribute__((packed));

struct BMPRGBQUAD {
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReversed;
} __attribute__((packed));

struct Image *image_readbmp(const char *path) {
    struct Image *image = NULL;

    FILE *const fp = fopen(path, "rb");
    if (!fp) {
        return NULL;
    }

    struct BMPFILEHEADER header;
    fread(&header, sizeof header, 1, fp);
    if (strncmp((const char*)&header.bType, "BM", 2) != 0) {
        // Not a Windows bitmap file
        goto error;
    }

    struct BMPINFOHEADER info;
    fread(&info, sizeof info, 1, fp);
    if (info.biInfoSize != 40) {
        // Not a Windows bitmap file
        goto error;
    }

    if (info.biBitCount != 1 && info.biBitCount != 8) {
        // Not a supported size
        goto error;
    }

    struct BMPRGBQUAD rgb[2];
    fread(&rgb, sizeof rgb, 1, fp);

    enum PixelColor colors[2] = { PIXEL_BLACK, PIXEL_WHITE };
    if (rgb[0].rgbBlue == 255 && rgb[0].rgbGreen == 255 && rgb[0].rgbRed == 255) {
        colors[0] = PIXEL_WHITE;
        colors[1] = PIXEL_BLACK;
    }

    image = image_alloc(info.biWidth, info.biHeight);
    fseek(fp, header.bOffset, SEEK_SET);

    // Bitmap pads each row out to 4 bytes
    const int row_bytes = ((info.biWidth * info.biBitCount + 31) / 32) * 4;

    // Bitmap starts with lower-left corner
    for (int y = image->height - 1 ; y >= 0; --y) {
        for (int x = 0; x < row_bytes; ++x) {
            uint8_t Rdata;
            if (fread((char*)&Rdata, 1, 1, fp) != 1) {
                goto error;
            }

            if (info.biBitCount == 1) {
                if (x < image->width_bytes) {
                    image->data[y * image->width_bytes + x] = Rdata;
                }
            } else if (info.biBitCount == 8) {
                const enum PixelColor color = Rdata ? colors[1] : colors[0];
                if (x < image->width) {
                    if (color == PIXEL_WHITE) {
                        image->data[y * image->width_bytes + x / 8] |=   0x80 >> (x % 8);
                    } else {
                        image->data[y * image->width_bytes + x / 8] &= ~(0x80 >> (x % 8));
                    }
                }
            }
        }
    }

    fclose(fp);

    assert(image_valid(image));
    return image;

error:
    fclose(fp);
    if (image) {
        image_free(image);
    }
    return NULL;
}

int image_png(const struct Image *image, uint8_t **png, size_t *size) {
    assert(image_valid(image));
    assert(png && !*png);
    assert(size && !*size);

    FILE *file = open_memstream((char**)png, size);

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        goto error;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        goto error;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        goto error;
    }

    png_init_io(png_ptr, file);
    png_set_IHDR(
        png_ptr,
        info_ptr,
        image->width,
        image->height,
        1,
        PNG_COLOR_TYPE_GRAY,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);

    png_bytep *row_pointers = alloca(image->height * sizeof(png_bytep));
    for (int y = 0; y != image->height; ++y) {
        row_pointers[y] = (uint8_t*)image->data + y * image->width_bytes;
    }
    png_write_image(png_ptr, row_pointers);

    png_write_end(png_ptr, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    fclose(file);
    return 0;

error:
    fclose(file);
    if (*png) {
        free(*png);
        *png = NULL;
    }
    *size = 0;
    return 1;
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
