#include "dsgl.h"

#define WIDTH 800
#define HEIGHT 600

uint32_t pixels[WIDTH * HEIGHT];

#define C_MAGENTA_RED RGBA(255, 0, 128, 255)
#define C_CYAN RGBA(0, 255, 255, 255)

Dsgl_Canvas render(void)
{
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    for (int i = 0; i < 40; ++i) {
        int gap = i * 10;
        dsgl_stroke_rect(&c, WIDTH - gap, HEIGHT - gap, -(WIDTH - gap * 2),
                         -(HEIGHT - gap * 2), 4,
                         (i % 2) ? C_MAGENTA_RED : C_CYAN);
    }

    return c;
}
