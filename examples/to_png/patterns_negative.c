#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "dsgl.h"

#define FILENAME "output/patterns_negative.png"
#define WIDTH 800
#define HEIGHT 600

static uint32_t pixels[WIDTH * HEIGHT];

Dsgl_Canvas render(uint32_t *pixels, int w, int h);

int main(void)
{
    Dsgl_Canvas c = render(pixels, WIDTH, HEIGHT);
    stbi_write_png(FILENAME, WIDTH, HEIGHT, 4, c.pixels, WIDTH * 4);
    return 0;
}
