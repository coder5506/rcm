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

struct sFONT;

class Context {
public:
    Image*       image;
    PixelColor   foreground;
    PixelColor   background;
    DotStyle     dot_style;
    int          dot_size;
    LineStyle    line_style;
    int          line_width;
    Rotate       rotate;
    const sFONT* font;

    Context();

    void clear();
    void drawpoint(int x, int y, PixelColor color);
    void drawline(int x0, int y0, int x1, int y1);
    void drawrect(int x0, int y0, int x1, int y1);
    void fillrect(int x0, int y0, int x1, int y1);
    void eraserect(int x0, int y0, int x1, int y1);
    void drawstring(int left, int top, const char* s);
    void drawimage(
        int x_to,
        int y_to,
        const Image& source,
        int x_from,
        int y_from,
        int w,
        int h);

private:
    void transform_point(int& x, int& y) const;
    void setpixel(int x, int y, PixelColor color);
    void drawline_low(int x0, int y0, int x1, int y1);
    void drawline_high(int x0, int y0, int x1, int y1);
    void drawchar(int x, int y, char c);
};

struct Rect {
    int left;
    int top;
    int right;
    int bottom;
};

class View {
public:
    Rect bounds;

    virtual ~View() = default;
    virtual void render(Context& context) = 0;
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
