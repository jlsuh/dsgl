#include "canvas.h"
#include <stdint.h>
#include <stdio.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define COMPONENTS 4

#define C_RED RGBA(220, 20, 20, 255)
#define C_BLUE RGBA(0, 70, 165, 255)
#define C_YELLOW RGBA(250, 210, 0, 255)
#define C_WHITE RGBA(245, 245, 240, 255)
#define C_BLACK RGBA(15, 15, 15, 255)

int main(void)
{
    int w = 600;
    int h = 600;
    Canvas *c = create_canvas(w, h);
    draw_rect(c, 0, 0, w, h, C_WHITE);
    int line_thick = 22;
    int x_axis = 140;
    int y_axis = 450;
    int y_split_left = 220;
    int x_split_right = 530;
    int y_split_right = 530;
    draw_rect(c, x_axis + line_thick, 0, w - (x_axis + line_thick), y_axis,
              C_RED);
    draw_rect(c, 0, y_axis + line_thick, x_axis, h - (y_axis + line_thick),
              C_BLUE);
    draw_rect(c, x_split_right + line_thick, y_split_right + line_thick,
              w - (x_split_right + line_thick),
              h - (y_split_right + line_thick), C_YELLOW);
    draw_rect(c, x_axis, 0, line_thick, h, C_BLACK);
    draw_rect(c, 0, y_axis, w, line_thick, C_BLACK);
    draw_rect(c, 0, y_split_left, x_axis, line_thick, C_BLACK);
    draw_rect(c, x_split_right, y_axis, line_thick, h - y_axis, C_BLACK);
    draw_rect(c, x_split_right, y_split_right, w - x_split_right, line_thick,
              C_BLACK);
    uint32_t *pixels = get_pixels(c);
    stbi_write_png("output/mondrian.png", w, h, COMPONENTS, pixels,
                   w * sizeof(*pixels));
    destroy_canvas(c);
    return 0;
}
