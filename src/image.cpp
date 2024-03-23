// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "image.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <png.h>

bool Image::is_valid() const {
    return
        width  > 0 &&
        height > 0 &&
        width_bytes == (width + 7) / 8 &&
        size_bytes  == width_bytes * height &&
        data_.size() == size_bytes;
}

Image::Image(int width, int height)
    : width{width},
      height{height},
      width_bytes{(width + 7) / 8},
      size_bytes{width_bytes * height},
      data_(size_bytes)
{
    assert(is_valid());
}

bool operator==(const Image &a, const Image &b) {
    return
        a.width  == b.width &&
        a.height == b.height &&
        a.data_   == b.data_;
}

bool operator!=(const Image &a, const Image &b) {
    return !(a == b);
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

std::unique_ptr<Image> Image::readbmp(const char* path) {
    auto fp = fopen(path, "rb");
    if (!fp) {
        return NULL;
    }

    struct BMPFILEHEADER header;
    fread(&header, sizeof header, 1, fp);
    if (strncmp((const char*)&header.bType, "BM", 2) != 0) {
        // Not a Windows bitmap file
        fclose(fp);
        return NULL;
    }

    struct BMPINFOHEADER info;
    fread(&info, sizeof info, 1, fp);
    if (info.biInfoSize != 40) {
        // Not a Windows bitmap file
        fclose(fp);
        return NULL;
    }

    if (info.biBitCount != 1 && info.biBitCount != 8) {
        // Not a supported size
        fclose(fp);
        return NULL;
    }

    struct BMPRGBQUAD rgb[2];
    fread(&rgb, sizeof rgb, 1, fp);

    PixelColor colors[2] = { PIXEL_BLACK, PIXEL_WHITE };
    if (rgb[0].rgbBlue == 255 && rgb[0].rgbGreen == 255 && rgb[0].rgbRed == 255) {
        colors[0] = PIXEL_WHITE;
        colors[1] = PIXEL_BLACK;
    }

    auto image = std::make_unique<Image>(int(info.biWidth), int(info.biHeight));
    fseek(fp, header.bOffset, SEEK_SET);

    // Bitmap pads each row out to 4 bytes
    const auto row_bytes = ((info.biWidth * info.biBitCount + 31) / 32) * 4;

    // Bitmap starts with lower-left corner
    for (auto y = image->height - 1 ; y >= 0; --y) {
        for (auto x = 0; x < row_bytes; ++x) {
            std::uint8_t Rdata;
            if (fread((char*)&Rdata, 1, 1, fp) != 1) {
                fclose(fp);
                return NULL;
            }

            if (info.biBitCount == 1) {
                if (x < image->width_bytes) {
                    image->data_[y * image->width_bytes + x] = Rdata;
                }
            } else if (info.biBitCount == 8) {
                const PixelColor color = Rdata ? colors[1] : colors[0];
                if (x < image->width) {
                    if (color == PIXEL_WHITE) {
                        image->data_[y * image->width_bytes + x / 8] |=   0x80 >> (x % 8);
                    } else {
                        image->data_[y * image->width_bytes + x / 8] &= ~(0x80 >> (x % 8));
                    }
                }
            }
        }
    }

    fclose(fp);

    assert(image->is_valid());
    return image;
}

int Image::png(std::uint8_t** png, std::size_t* size) const {
    assert(is_valid());
    assert(png && !*png);
    assert(size && !*size);

    auto file = open_memstream((char**)png, size);

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(file);
        return 1;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(file);
        return 1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(file);
        if (*png) {
            free(*png);
            *png = NULL;
        }
        return 1;
    }

    png_init_io(png_ptr, file);
    png_set_IHDR(
        png_ptr,
        info_ptr,
        width,
        height,
        1,
        PNG_COLOR_TYPE_GRAY,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);

    png_bytep* row_pointers = (png_bytep*)alloca(height * sizeof(png_bytep));
    for (auto y = 0; y != height; ++y) {
        row_pointers[y] = const_cast<std::uint8_t*>(data()) + y * width_bytes;
    }
    png_write_image(png_ptr, row_pointers);

    png_write_end(png_ptr, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    fclose(file);
    return 0;
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
