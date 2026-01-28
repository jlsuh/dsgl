#include <stdint.h>
#include <stdlib.h>

#include "dsgl.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define C_MAGENTA_RED RGBA(255, 0, 128, 255)
#define C_CYAN RGBA(0, 255, 255, 255)

#define FILENAME "output/patterns_negative.png"

int main(void)
{
    int w = 800, h = 600;
    uint32_t *p = calloc(w * h, sizeof(*p));
    Dsgl_Canvas c = dsgl_create_canvas(p, w, h);
    for (int i = 0; i < 40; ++i) {
        int gap = i * 10;
        dsgl_stroke_rect(c, w - gap, h - gap, -(w - gap * 2), -(h - gap * 2), 4,
                         (i % 2) ? C_MAGENTA_RED : C_CYAN);
    }
    stbi_write_png(FILENAME, w, h, 4, p, w * 4);
    free(p);
    return 0;
}
