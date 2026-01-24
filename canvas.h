#ifndef CANVAS_H_INCLUDED
#define CANVAS_H_INCLUDED

#include <stdint.h>

#define RGBA(r, g, b, a)                                                       \
    ((((uint32_t)(a) & 0xFF) << 24) | (((uint32_t)(b) & 0xFF) << 16) |         \
     (((uint32_t)(g) & 0xFF) << 8) | (((uint32_t)(r) & 0xFF)))

typedef struct Canvas Canvas;

Canvas *create_canvas(int width, int height);
void destroy_canvas(Canvas *self);
uint32_t *get_pixels(Canvas *self);
void draw_rect(Canvas *self, int x0, int y0, int width, int height,
               uint32_t color);

#endif
