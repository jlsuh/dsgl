#include "dsgl.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static inline int dsgl_clamp(int value, int min, int max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

Dsgl_Canvas dsgl_create_canvas(uint32_t *pixels, int width, int height)
{
    if (width <= 0 || height <= 0) {
        return DSGL_CANVAS_NULL;
    }
    if (width > DSGL_MAX_DIM || height > DSGL_MAX_DIM) {
        return DSGL_CANVAS_NULL;
    }
    if (NULL == pixels) {
        return DSGL_CANVAS_NULL;
    }
    return (Dsgl_Canvas){.pixels = pixels, .width = width, .height = height};
}

int8_t dsgl_fill_rect(Dsgl_Canvas self, int x0, int y0, int width, int height,
                      uint32_t color)
{
    if (NULL == self.pixels)
        return DSGL_FAILURE;
    if (width <= 0 || height <= 0)
        return DSGL_FAILURE;
    int x1 = dsgl_clamp(x0 + width, 0, self.width);
    int y1 = dsgl_clamp(y0 + height, 0, self.height);
    x0 = dsgl_clamp(x0, 0, self.width);
    y0 = dsgl_clamp(y0, 0, self.height);
    if (x1 <= x0 || y1 <= y0)
        return DSGL_FAILURE;
    for (int y = y0; y < y1; ++y) {
        uint32_t *row = self.pixels + (y * self.width);
        for (int x = x0; x < x1; ++x) {
            row[x] = color;
        }
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
