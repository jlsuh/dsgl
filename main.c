#include <stdint.h>
#include <stdio.h>

#include "canvas.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define COMPONENTS 4

int main(void)
{
    int w = 800;
    int h = 600;
    Canvas *c = create_canvas(w, h);
    uint32_t red = pack_color(255, 0, 0, 255);
    printf("Color Value: 0x%08X\n", red);
    draw_rect(c, 100, 100, 200, 300, red);
    uint32_t *pixels = get_pixels(c);
    stbi_write_png("output.png", w, h, COMPONENTS, pixels, w * sizeof(*pixels));
    destroy_canvas(c);
    return 0;
}
