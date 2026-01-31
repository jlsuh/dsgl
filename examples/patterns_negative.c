#include "dsgl.h"

#define C_MAGENTA_RED RGBA(255, 0, 128, 255)
#define C_CYAN RGBA(0, 255, 255, 255)

Dsgl_Canvas render(uint32_t *pixels, int w, int h)
{
    Dsgl_Canvas c = dsgl_create_canvas(pixels, w, h);
    for (int i = 0; i < 40; ++i) {
        int gap = i * 10;
        dsgl_stroke_rect(c, w - gap, h - gap, -(w - gap * 2), -(h - gap * 2), 4,
                         (i % 2) ? C_MAGENTA_RED : C_CYAN);
    }

    return c;
}
