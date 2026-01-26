#ifndef DSGL_H_
#define DSGL_H_

#include <stdint.h>
#include <stdio.h>

#define RGBA(r, g, b, a)                                                       \
    ((((uint32_t)(a) & 0xFF) << 24) | (((uint32_t)(b) & 0xFF) << 16) |         \
     (((uint32_t)(g) & 0xFF) << 8) | (((uint32_t)(r) & 0xFF)))

#define C_BLUE RGBA(0, 0, 255, 255)

typedef struct {
    uint32_t *pixels;
    size_t width;
    size_t height;
} Dsgl_Canvas;

Dsgl_Canvas dsgl_create_canvas(uint32_t *pixels, size_t width, size_t height);
uint32_t *dsgl_get_pixels(Dsgl_Canvas self);
int8_t dsgl_fill_rect(Dsgl_Canvas self, int x0, int y0, size_t width,
                      size_t height, uint32_t color);
void dsgl_stroke_rect(Dsgl_Canvas self, int x0, int y0, size_t width,
                      size_t height, size_t border, uint32_t color);

#endif // DSGL_H_

#ifdef DSGL_IMPLEMENTATION

#define DSGL_MAX_DIM 8192

static inline int dsgl_clamp(int value, int min, int max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

Dsgl_Canvas dsgl_create_canvas(uint32_t *pixels, size_t width, size_t height)
{
    Dsgl_Canvas error = {0};
    if (NULL == pixels) {
        fprintf(stderr, "Error: Pixels NULL\n");
        return error;
    }
    if (0 == width || 0 == height) {
        fprintf(stderr, "Error: Dims zero\n");
        return error;
    }
    if (width > DSGL_MAX_DIM || height > DSGL_MAX_DIM) {
        fprintf(stderr, "Error: DSGL_MAX_DIM\n");
        return error;
    }
    return (Dsgl_Canvas){.pixels = pixels, .width = width, .height = height};
}

int8_t dsgl_fill_rect(Dsgl_Canvas self, int x0, int y0, size_t width,
                      size_t height, uint32_t color)
{
    if (NULL == self.pixels)
        return -1;
    int clamped_x0 = dsgl_clamp(x0, 0, (int)self.width);
    int clamped_y0 = dsgl_clamp(y0, 0, (int)self.height);
    int clamped_x1 = dsgl_clamp(x0 + (int)width, 0, (int)self.width);
    int clamped_y1 = dsgl_clamp(y0 + (int)height, 0, (int)self.height);
    if (clamped_x1 <= clamped_x0 || clamped_y1 <= clamped_y0)
        return -1;
    for (size_t y = (size_t)clamped_y0; y < (size_t)clamped_y1; ++y) {
        uint32_t *row = self.pixels + (y * self.width);
        for (size_t x = (size_t)clamped_x0; x < (size_t)clamped_x1; ++x) {
            row[x] = color;
        }
    }
    return 0;
}

void dsgl_stroke_rect(Dsgl_Canvas self, int x0, int y0, size_t width,
                      size_t height, size_t border, uint32_t color)
{
    if (0 == border || NULL == self.pixels)
        return;
    if (border * 2 >= width || border * 2 >= height) {
        dsgl_fill_rect(self, x0, y0, width, height, color);
        return;
    }
    int b = (int)border;
    int w = (int)width;
    int h = (int)height;
    dsgl_fill_rect(self, x0, y0, width, border, color);
    dsgl_fill_rect(self, x0, y0 + h - b, width, border, color);
    dsgl_fill_rect(self, x0, y0 + b, border, height - (2 * b), color);
    dsgl_fill_rect(self, x0 + w - b, y0 + b, border, height - (2 * b), color);
}

#endif // DSGL_IMPLEMENTATION
