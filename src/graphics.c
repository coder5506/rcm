// Copyright (c) 2024 Eric Sessoms
// See license at end of file

#include "graphics.h"
#include "fonts/fonts.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Context default_context = {
    .next       = NULL,
    .image      = NULL,
    .foreground = PIXEL_BLACK,
    .background = PIXEL_WHITE,
    .dot_style  = DOT_FILL_AROUND,
    .dot_size   = 1,
    .line_style = LINE_STYLE_SOLID,
    .line_width = 1,
    .rotate     = ROTATE_180,
    .font       = &Font16,
};

static struct Context *context_free_list = NULL;

void context_free(struct Context *context) {
    assert(context && context != &default_context);
    context->next = context_free_list;
    context_free_list = context;
}

struct Context *context_alloc() {
    struct Context *context = context_free_list;
    if (context) {
        context_free_list = context->next;
    } else {
        context = malloc(sizeof *context);
    }
    *context = default_context;
    return context;
}

void context_push(struct Context **context) {
    struct Context *new_context = context_alloc();
    *new_context = **context;
    new_context->next = *context;
    *context = new_context;
}

void context_pop(struct Context **context) {
    assert(context && (*context)->next);
    struct Context *old_context = *context;
    *context = old_context->next;
    context_free(old_context);
}

void graphics_clear(struct Context *context) {
    struct Image *image = context->image;
    memset(image->data, context->background, image->size_bytes);
}

static void transform_point(const struct Context *context, int *x, int *y) {
    const struct Image *image = context->image;
    const int x_orig = *x;

    switch (context->rotate) {
    case ROTATE_0:
        break;
    case ROTATE_90:
        *x = *y;
        *y = image->height - 1 - x_orig;
        break;
    case ROTATE_180:
        *x = image->width  - 1 - x_orig;
        *y = image->height - 1 - *y;
        break;
    case ROTATE_270:
        *x = image->width  - 1 - *y;
        *y = x_orig;
        break;
    default:
        assert(0);
    }
}

static void
setpixel(struct Context *context, int x, int y, enum PixelColor color) {
    struct Image *image = context->image;

    transform_point(context, &x, &y);
    if (x < 0 || image->width <= x || y < 0 || image->height <= y) {
        return;
    }

    const int byte = y * image->width_bytes + x / 8;
    const int bit  = 0x80 >> (x % 8); // MSB
    if (color == PIXEL_BLACK) {
        image->data[byte] &= ~bit;
    } else {
        image->data[byte] |=  bit;
    }
}

void
graphics_drawpoint(
    struct Context *context, int x, int y, enum PixelColor color)
{
    const int dot_size = context->dot_size;
    switch (context->dot_style) {
    case DOT_FILL_AROUND:
        for (int xd = 0; xd < 2 * dot_size - 1; ++xd) {
            for (int yd = 0; yd < 2 * dot_size - 1; ++yd) {
                setpixel(context, x + xd - dot_size, y + yd - dot_size, color);
            }
        }
        break;
    case DOT_FILL_RIGHTUP:
        for (int xd = 0; xd < dot_size; ++xd) {
            for (int yd = 0; yd < dot_size; ++yd) {
                setpixel(context, x + xd - 1, y + yd - 1, color);
            }
        }
        break;
    default:
        assert(0);
    }
}

static int sign(int x) {
    return x < 0 ? -1 : x > 0 ? 1 : 0;
}

static void
drawline_low(struct Context *context, int x0, int y0, int x1, int y1) {
    const int line_style = context->line_style;
    int dots = 0;

    const int dx = x1 - x0;
    const int dy = abs(y1 - y0);
    const int sy = sign(y1 - y0);

    int d = 2 * dy - dx;
    int y = y0;
    for (int x = x0; x <= x1; ++x) {
        if (line_style == LINE_STYLE_DOTTED && ++dots % 3 == 0) {
            graphics_drawpoint(context, x, y, context->background);
        } else {
            graphics_drawpoint(context, x, y, context->foreground);
        }
        if (d > 0) {
            y += sy;
            d -= 2 * dx;
        }
        d += 2 * dy;
    }
}

static void
drawline_high(struct Context *context, int x0, int y0, int x1, int y1) {
    const int line_style = context->line_style;
    int dots = 0;

    const int dy = y1 - y0;
    const int dx = abs(x1 - x0);
    const int sx = sign(x1 - x0);

    int d = 2 * dx - dy;
    int x = x0;
    for (int y = y0; y <= y1; ++y) {
        if (line_style == LINE_STYLE_DOTTED && ++dots % 3 == 0) {
            graphics_drawpoint(context, x, y, context->background);
        } else {
            graphics_drawpoint(context, x, y, context->foreground);
        }
        if (d > 0) {
            x += sx;
            d -= 2 * dy;
        }
        d += 2 * dx;
    }
}

void
graphics_drawline(struct Context *context, int x0, int y0, int x1, int y1) {
    context_push(&context);
    context->dot_style = DOT_FILL_AROUND;
    context->dot_size  = context->line_width;

    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 <= x1) {
            drawline_low(context, x0, y0, x1, y1);
        } else {
            drawline_low(context, x1, y1, x0, y0);
        }
    } else {
        if (y0 <= y1) {
            drawline_high(context, x0, y0, x1, y1);
        } else {
            drawline_high(context, x1, y1, x0, y0);
        }
    }

    context_pop(&context);
}

void
graphics_drawrect(struct Context *context, int x0, int y0, int x1, int y1) {
    graphics_drawline(context, x0, y0, x1, y0);
    graphics_drawline(context, x1, y0, x1, y1);
    graphics_drawline(context, x1, y1, x0, y1);
    graphics_drawline(context, x0, y1, x0, y0);
}

void
graphics_fillrect(struct Context *context, int x0, int y0, int x1, int y1) {
    for (int y = y0; y <= y1; ++y) {
        graphics_drawline(context, x0, y, x1, y);
    }
}

void
graphics_eraserect(struct Context *context, int x0, int y0, int x1, int y1) {
    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            setpixel(context, x, y, context->background);
        }
    }
}

static void drawchar(struct Context *context, int x, int y, char c) {
    const struct sFONT *font = context->font;
    const int index = (c - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const uint8_t *ptr = &font->table[index];

    for (int r = 0; r < font->Height; ++r) {
        for (int c = 0; c < font->Width; ++c) {
            if (*ptr & (0x80 >> (c % 8))) {
                setpixel(context, x + c, y + r, context->foreground);
            }
            else {
                setpixel(context, x + c, y + r, context->background);
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

void
graphics_drawstring(struct Context *context, int left, int top, const char *s) {
    const struct sFONT *font = context->font;
    struct Image *image = context->image;

    int x = left;
    int y = top;
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

        drawchar(context, x, y, *s++);
        x += font->Width;
    }
}

void
graphics_drawimage(
    struct Context *context,
    int x_to,
    int y_to,
    const struct Image *image,
    int x_from,
    int y_from,
    int w,
    int h)
{
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            const int x_src = x_from + x;
            const int y_src = y_from + y;
            const int x_dst = x_to + x;
            const int y_dst = y_to + y;
            if (x_src < 0 || image->width <= x_src || y_src < 0 || image->height <= y_src) {
                continue;
            }
            const int byte = image->data[y_src * image->width_bytes + x_src / 8];
            const int bit  = 0x80 >> (x_src % 8);
            enum PixelColor color = byte & bit ? context->background : context->foreground;
            setpixel(context, x_dst, y_dst, color);
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
