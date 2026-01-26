#ifndef DSGL_H_
#define DSGL_H_

#include <stdint.h>

#define DSGL_MAX_DIM 8192

#define RGBA(r, g, b, a)                                                       \
    ((((uint32_t)(a) & 0xFF) << 24) | (((uint32_t)(b) & 0xFF) << 16) |         \
     (((uint32_t)(g) & 0xFF) << 8) | (((uint32_t)(r) & 0xFF)))

#define C_BLUE RGBA(0, 0, 255, 255)

typedef struct {
    uint32_t *pixels;
    int width;
    int height;
} Dsgl_Canvas;

Dsgl_Canvas dsgl_create_canvas(uint32_t *pixels, int width, int height);
int8_t dsgl_fill_rect(Dsgl_Canvas self, int x0, int y0, int width, int height,
                      uint32_t color);
void dsgl_stroke_rect(Dsgl_Canvas self, int x0, int y0, int width, int height,
                      int border, uint32_t color);

#endif // DSGL_H_
