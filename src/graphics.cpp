// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#include "graphics.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

Context::Context()
    : image{nullptr},
      foreground{PIXEL_BLACK},
      background{PIXEL_WHITE},
      dot_style{DOT_FILL_AROUND},
      dot_size{1},
      line_style{LINE_STYLE_SOLID},
      line_width{1},
      rotate{ROTATE_180},
      font{&Font16}
{
}

void Context::clear() {
    memset(image->data(), background, image->size_bytes);
}

void Context::transform_point(int& x, int& y) const {
    const auto x_orig = x;
    switch (rotate) {
    case ROTATE_0:
        break;
    case ROTATE_90:
        x = y;
        y = image->height - 1 - x_orig;
        break;
    case ROTATE_180:
        x = image->width  - 1 - x_orig;
        y = image->height - 1 - y;
        break;
    case ROTATE_270:
        x = image->width  - 1 - y;
        y = x_orig;
        break;
    default:
        assert(0);
    }
}

void Context::setpixel(int x, int y, PixelColor color) {
    transform_point(x, y);
    if (x < 0 || image->width <= x || y < 0 || image->height <= y) {
        return;
    }

    const int byte = y * image->width_bytes + x / 8;
    const int bit  = 0x80 >> (x % 8); // MSB
    if (color == PIXEL_BLACK) {
        image->data_[byte] &= ~bit;
    } else {
        image->data_[byte] |=  bit;
    }
}

void Context::drawpoint(int x, int y, PixelColor color)
{
    switch (dot_style) {
    case DOT_FILL_AROUND:
        for (auto xd = 0; xd < 2 * dot_size - 1; ++xd) {
            for (auto yd = 0; yd < 2 * dot_size - 1; ++yd) {
                setpixel(x + xd - dot_size, y + yd - dot_size, color);
            }
        }
        break;
    case DOT_FILL_RIGHTUP:
        for (auto xd = 0; xd < dot_size; ++xd) {
            for (auto yd = 0; yd < dot_size; ++yd) {
                setpixel(x + xd - 1, y + yd - 1, color);
            }
        }
        break;
    default:
        assert(0);
    }
}

inline int sign(int x) { return x < 0 ? -1 : x > 0 ? 1 : 0; }

void Context::drawline_low(int x0, int y0, int x1, int y1) {
    auto dots = 0;

    const auto dx = x1 - x0;
    const auto dy = abs(y1 - y0);
    const auto sy = sign(y1 - y0);

    auto d = 2 * dy - dx;
    auto y = y0;
    for (auto x = x0; x <= x1; ++x) {
        if (line_style == LINE_STYLE_DOTTED && ++dots % 3 == 0) {
            drawpoint(x, y, background);
        } else {
            drawpoint(x, y, foreground);
        }
        if (d > 0) {
            y += sy;
            d -= 2 * dx;
        }
        d += 2 * dy;
    }
}

void  Context::drawline_high(int x0, int y0, int x1, int y1) {
    auto dots = 0;

    const auto dy = y1 - y0;
    const auto dx = abs(x1 - x0);
    const auto sx = sign(x1 - x0);

    auto d = 2 * dx - dy;
    auto x = x0;
    for (auto y = y0; y <= y1; ++y) {
        if (line_style == LINE_STYLE_DOTTED && ++dots % 3 == 0) {
            drawpoint(x, y, background);
        } else {
            drawpoint(x, y, foreground);
        }
        if (d > 0) {
            x += sx;
            d -= 2 * dy;
        }
        d += 2 * dx;
    }
}

void Context::drawline(int x0, int y0, int x1, int y1) {
    const auto save_dot_style = dot_style;
    const auto save_dot_size  = dot_size;
    dot_style = DOT_FILL_AROUND;
    dot_size  = line_width;

    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 <= x1) {
            drawline_low(x0, y0, x1, y1);
        } else {
            drawline_low(x1, y1, x0, y0);
        }
    } else {
        if (y0 <= y1) {
            drawline_high(x0, y0, x1, y1);
        } else {
            drawline_high(x1, y1, x0, y0);
        }
    }

    dot_style = save_dot_style;
    dot_size  = save_dot_size;
}

void Context::drawrect(int x0, int y0, int x1, int y1) {
    drawline(x0, y0, x1, y0);
    drawline(x1, y0, x1, y1);
    drawline(x1, y1, x0, y1);
    drawline(x0, y1, x0, y0);
}

void Context::fillrect(int x0, int y0, int x1, int y1) {
    for (auto y = y0; y <= y1; ++y) {
        drawline(x0, y, x1, y);
    }
}

void Context::eraserect(int x0, int y0, int x1, int y1) {
    for (auto y = y0; y <= y1; ++y) {
        for (auto x = x0; x <= x1; ++x) {
            setpixel(x, y, background);
        }
    }
}

void Context::drawchar(int x, int y, char c) {
    auto index = (c - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    auto ptr = &font->table[index];

    for (auto r = 0; r < font->Height; ++r) {
        for (auto c = 0; c < font->Width; ++c) {
            if (*ptr & (0x80 >> (c % 8))) {
                setpixel(x + c, y + r, foreground);
            }
            else {
                setpixel(x + c, y + r, background);
            }

            // Move to next byte of font data
            if (c % 8 == 7) {
                ptr++;
            }
        }

        // Move to next row of font data
        if (font->Width % 8 != 0) {
            ptr++;
        }
    }
}

void Context::drawstring(int left, int top, const char* s) {
    auto x = left;
    auto y = top;
    while (*s) {
        if (x + font->Width > image->width) {
            // Wrap to next line
            x = left;
            y += font->Height;
        }
        if (y + font->Height > image->height) {
            // Wrap back to start
            x = left;
            y = top;
        }

        drawchar(x, y, *s++);
        x += font->Width;
    }
}

void Context::drawimage(
    int x_to,
    int y_to,
    const Image& source,
    int x_from,
    int y_from,
    int w,
    int h)
{
    for (auto y = 0; y < h; ++y) {
        for (auto x = 0; x < w; ++x) {
            const auto x_src = x_from + x;
            const auto y_src = y_from + y;
            const auto x_dst = x_to + x;
            const auto y_dst = y_to + y;
            if (x_src < 0 || source.width <= x_src || y_src < 0 || source.height <= y_src) {
                continue;
            }
            const auto byte = source.data_[y_src * source.width_bytes + x_src / 8];
            const auto bit  = 0x80 >> (x_src % 8);
            const auto color = byte & bit ? background : foreground;
            setpixel(x_dst, y_dst, color);
        }
    }
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
