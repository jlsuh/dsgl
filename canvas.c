#include "canvas.h"

#include <stdlib.h>

struct Canvas {
    uint32_t *pixels;
    int width;
    int height;
};

Canvas *create_canvas(int width, int height)
{
    Canvas *self = malloc(sizeof(*self));
    if (NULL == self)
        return NULL;
    self->width = width;
    self->height = height;
    self->pixels = malloc(width * height * sizeof(*self->pixels));
    if (NULL == self->pixels) {
        free(self);
        return NULL;
    }
    return self;
}

void destroy_canvas(Canvas *self)
{
    if (self) {
        free(self->pixels);
        free(self);
    }
}

static inline void draw_pixel(Canvas *self, int x, int y, uint32_t color)
{
    if (x < 0 || y < 0 || x >= self->width || y >= self->height)
        return;
    self->pixels[y * self->width + x] = color;
}

uint32_t *get_pixels(Canvas *self)
{
    return self->pixels;
}

void draw_rect(Canvas *self, int x0, int y0, int width, int height,
               uint32_t color)
{
    for (int y = y0; y < y0 + height; ++y)
        for (int x = x0; x < x0 + width; ++x)
            draw_pixel(self, x, y, color);
}
