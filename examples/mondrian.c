#include <stdio.h>
#include <stdlib.h>

#define DSGL_IMPLEMENTATION
#include "dsgl.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define C_MONDRIAN_RED RGBA(220, 20, 20, 255)
#define C_MONDRIAN_BLUE RGBA(0, 70, 165, 255)
#define C_MONDRIAN_YELLOW RGBA(250, 210, 0, 255)
#define C_MONDRIAN_WHITE RGBA(245, 245, 240, 255)
#define C_MONDRIAN_BLACK RGBA(15, 15, 15, 255)

#define FILENAME "output/mondrian.png"

int main(void)
{
    size_t w = 600;
    size_t h = 600;
    uint32_t *pixels = calloc(w * h, sizeof(*pixels));
    Dsgl_Canvas c = dsgl_create_canvas(pixels, w, h);
    dsgl_fill_rect(c, 0, 0, w, h, C_MONDRIAN_WHITE);
    int line_thick = 22;
    int x_axis = 140;
    int y_axis = 450;
    int y_split_left = 220;
    int x_split_right = 530;
    int y_split_right = 530;
    dsgl_fill_rect(c, x_axis + line_thick, 0, w - (x_axis + line_thick), y_axis,
                   C_MONDRIAN_RED);
    dsgl_fill_rect(c, 0, y_axis + line_thick, x_axis, h - (y_axis + line_thick),
                   C_MONDRIAN_BLUE);
    dsgl_fill_rect(c, x_split_right + line_thick, y_split_right + line_thick,
                   w - (x_split_right + line_thick),
                   h - (y_split_right + line_thick), C_MONDRIAN_YELLOW);
    dsgl_fill_rect(c, x_axis, 0, line_thick, h, C_MONDRIAN_BLACK);
    dsgl_fill_rect(c, 0, y_axis, w, line_thick, C_MONDRIAN_BLACK);
    dsgl_fill_rect(c, 0, y_split_left, x_axis, line_thick, C_MONDRIAN_BLACK);
    dsgl_fill_rect(c, x_split_right, y_axis, line_thick, h - y_axis,
                   C_MONDRIAN_BLACK);
    dsgl_fill_rect(c, x_split_right, y_split_right, w - x_split_right,
                   line_thick, C_MONDRIAN_BLACK);
    stbi_write_png(FILENAME, (int)w, (int)h, 4, pixels,
                   (int)w * sizeof(*pixels));
    free(pixels);
    return 0;
}
