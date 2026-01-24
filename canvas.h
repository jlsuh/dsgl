#ifndef CANVAS_H_INCLUDED
#define CANVAS_H_INCLUDED

#include <stdint.h>

typedef struct Canvas Canvas;

Canvas *create_canvas(int width, int height);
void destroy_canvas(Canvas *self);
void draw_pixel(Canvas *self, int x, int y, uint32_t color);
uint32_t pack_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void draw_rect(Canvas *self, int x0, int y0, int width, int height, uint32_t color);
uint32_t *get_pixels(Canvas *self);

#endif
