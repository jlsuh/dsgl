#ifndef DSGL_H_
#define DSGL_H_

#include <stdint.h>

#define DSGL_SUCCESS 0
#define DSGL_FAILURE -1
#define DSGL_CANVAS_NULL ((Dsgl_Canvas){0})
#define DSGL_MAX_DIM 8192

#define RGBA(r, g, b, a)                                                       \
    ((((uint32_t)(a) & 0xFF) << 24) | (((uint32_t)(b) & 0xFF) << 16) |         \
     (((uint32_t)(g) & 0xFF) << 8) | (((uint32_t)(r) & 0xFF)))

#define C_BLUE RGBA(0, 0, 255, 255)

#define DSGL_CLAMP(val, min, max)                                              \
    (((val) < (min)) ? (min) : (((val) > (max)) ? (max) : (val)))

#define DSGL_SWAP(a, b)                                                        \
    do {                                                                       \
        int t = (a);                                                           \
        (a) = (b);                                                             \
        (b) = t;                                                               \
    } while (0)

typedef struct {
    uint32_t *pixels;
    int width;
    int height;
} Dsgl_Canvas;

Dsgl_Canvas dsgl_create_canvas(uint32_t *pixels, int width, int height);
int8_t dsgl_fill_rect(Dsgl_Canvas *self, int x0, int y0, int width, int height,
                      uint32_t color);
void dsgl_stroke_rect(Dsgl_Canvas *self, int x0, int y0, int width, int height,
                      int border, uint32_t color);

#endif // DSGL_H_
