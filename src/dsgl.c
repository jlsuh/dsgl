#include "dsgl.h"

#include <stdint.h>
#include <stdio.h>

Dsgl_Canvas dsgl_create_canvas(uint32_t *pixels, int width, int height)
{
    if (width <= 0 || height <= 0)
        return DSGL_CANVAS_NULL;
    if (width > DSGL_MAX_DIM || height > DSGL_MAX_DIM)
        return DSGL_CANVAS_NULL;
    if (NULL == pixels)
        return DSGL_CANVAS_NULL;
    return (Dsgl_Canvas){.pixels = pixels, .width = width, .height = height};
}

int8_t dsgl_fill_rect(Dsgl_Canvas self, int x0, int y0, int width, int height,
                      uint32_t color)
{
    if (NULL == self.pixels)
        return DSGL_FAILURE;
    int x1 = x0 + width;
    int y1 = y0 + height;
    if (x1 < x0)
        DSGL_SWAP(x0, x1);
    if (y1 < y0)
        DSGL_SWAP(y0, y1);
    x0 = DSGL_CLAMP(x0, 0, self.width);
    y0 = DSGL_CLAMP(y0, 0, self.height);
    x1 = DSGL_CLAMP(x1, 0, self.width);
    y1 = DSGL_CLAMP(y1, 0, self.height);
    if (x1 <= x0 || y1 <= y0)
        return DSGL_FAILURE;
    for (int y = y0; y < y1; ++y) {
        uint32_t *row = self.pixels + (y * self.width);
        for (int x = x0; x < x1; ++x)
            row[x] = color;
    }
    return DSGL_SUCCESS;
}

void dsgl_stroke_rect(Dsgl_Canvas self, int x0, int y0, int width, int height,
                      int border, uint32_t color)
{
    if (border <= 0 || NULL == self.pixels)
        return;
    if (border * 2 >= width || border * 2 >= height) {
        dsgl_fill_rect(self, x0, y0, width, height, color);
        return;
    }
    dsgl_fill_rect(self, x0, y0, width, border, color);
    dsgl_fill_rect(self, x0, y0 + height - border, width, border, color);
    dsgl_fill_rect(self, x0, y0 + border, border, height - (2 * border), color);
    dsgl_fill_rect(self, x0 + width - border, y0 + border, border,
                   height - (2 * border), color);
}
