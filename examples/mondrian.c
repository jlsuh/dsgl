#include "dsgl.h"

#define WIDTH 600
#define HEIGHT 600

uint32_t pixels[WIDTH * HEIGHT];

#define C_MONDRIAN_RED RGBA(220, 20, 20, 255)
#define C_MONDRIAN_BLUE RGBA(0, 70, 165, 255)
#define C_MONDRIAN_YELLOW RGBA(250, 210, 0, 255)
#define C_MONDRIAN_WHITE RGBA(245, 245, 240, 255)
#define C_MONDRIAN_BLACK RGBA(15, 15, 15, 255)

void render(void)
{
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    dsgl_fill_rect(&c, 0, 0, WIDTH, HEIGHT, C_MONDRIAN_WHITE);
    int line_thick = 22;
    int x_axis = 140;
    int y_axis = 450;
    int y_split_left = 220;
    int x_split_right = 530;
    int y_split_right = 530;
    dsgl_fill_rect(&c, x_axis + line_thick, 0, WIDTH - (x_axis + line_thick),
                   y_axis, C_MONDRIAN_RED);
    dsgl_fill_rect(&c, 0, y_axis + line_thick, x_axis,
                   HEIGHT - (y_axis + line_thick), C_MONDRIAN_BLUE);
    dsgl_fill_rect(&c, x_split_right + line_thick, y_split_right + line_thick,
                   WIDTH - (x_split_right + line_thick),
                   HEIGHT - (y_split_right + line_thick), C_MONDRIAN_YELLOW);
    dsgl_fill_rect(&c, x_axis, 0, line_thick, HEIGHT, C_MONDRIAN_BLACK);
    dsgl_fill_rect(&c, 0, y_axis, WIDTH, line_thick, C_MONDRIAN_BLACK);
    dsgl_fill_rect(&c, 0, y_split_left, x_axis, line_thick, C_MONDRIAN_BLACK);
    dsgl_fill_rect(&c, x_split_right, y_axis, line_thick, HEIGHT - y_axis,
                   C_MONDRIAN_BLACK);
    dsgl_fill_rect(&c, x_split_right, y_split_right, WIDTH - x_split_right,
                   line_thick, C_MONDRIAN_BLACK);
}
