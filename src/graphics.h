// Copyright (C) 2024 Eric Sessoms
// See license at end of file

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "fonts/fonts.h"
#include "image.h"

enum DotStyle {
    DOT_FILL_AROUND,
    DOT_FILL_RIGHTUP,
};

enum LineStyle {
    LINE_STYLE_SOLID,
    LINE_STYLE_DOTTED,
};

// CCW
enum Rotate {
    ROTATE_0   =   0,
    ROTATE_90  =  90,
    ROTATE_180 = 180,
    ROTATE_270 = 270,
};

struct Image;
struct sFONT;

struct Context {
    struct Context *next;
    struct Image   *image;
    enum PixelColor foreground;
    enum PixelColor background;
    enum DotStyle   dot_style;
    int             dot_size;
    enum LineStyle  line_style;
    int             line_width;
    enum Rotate     rotate;
    const struct sFONT *font;
};

struct Rect {
    int left;
    int top;
    int right;
    int bottom;
};

struct View;

typedef void (*ViewRender)(struct View *view, struct Context *context);

struct View {
    ViewRender  render;
    struct Rect bounds;
};

void context_free(struct Context *context);
struct Context *context_alloc();
void context_push(struct Context **context);
void context_pop(struct Context **context);

void graphics_clear(struct Context*);
void graphics_drawpoint(struct Context*, int x, int y, enum PixelColor color);
void graphics_drawline(struct Context*, int x0, int y0, int x1, int y1);
void graphics_drawrect(struct Context*, int x0, int y0, int x1, int y1);
void graphics_eraserect(struct Context*, int x0, int y0, int x1, int y1);
void graphics_fillrect(struct Context*, int x0, int y0, int x1, int y1);
void graphics_drawstring(struct Context*, int left, int top, const char *s);

void
graphics_drawimage(
    struct Context *context,
    int x_to,
    int y_to,
    const struct Image *image,
    int x_from,
    int y_from,
    int w,
    int h);

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
