#include "dsgl.h"

#define C_MONDRIAN_RED RGBA(220, 20, 20, 255)
#define C_MONDRIAN_BLUE RGBA(0, 70, 165, 255)
#define C_MONDRIAN_YELLOW RGBA(250, 210, 0, 255)
#define C_MONDRIAN_WHITE RGBA(245, 245, 240, 255)
#define C_MONDRIAN_BLACK RGBA(15, 15, 15, 255)

Dsgl_Canvas render(uint32_t *pixels, int width, int height)
{
    Dsgl_Canvas c = dsgl_create_canvas(pixels, width, height);
    dsgl_fill_rect(c, 0, 0, width, height, C_MONDRIAN_WHITE);
    int line_thick = 22;
    int x_axis = 140;
    int y_axis = 450;
    int y_split_left = 220;
    int x_split_right = 530;
    int y_split_right = 530;
    dsgl_fill_rect(c, x_axis + line_thick, 0, width - (x_axis + line_thick),
                   y_axis, C_MONDRIAN_RED);
    dsgl_fill_rect(c, 0, y_axis + line_thick, x_axis,
                   height - (y_axis + line_thick), C_MONDRIAN_BLUE);
    dsgl_fill_rect(c, x_split_right + line_thick, y_split_right + line_thick,
                   width - (x_split_right + line_thick),
                   height - (y_split_right + line_thick), C_MONDRIAN_YELLOW);
    dsgl_fill_rect(c, x_axis, 0, line_thick, height, C_MONDRIAN_BLACK);
    dsgl_fill_rect(c, 0, y_axis, width, line_thick, C_MONDRIAN_BLACK);
    dsgl_fill_rect(c, 0, y_split_left, x_axis, line_thick, C_MONDRIAN_BLACK);
    dsgl_fill_rect(c, x_split_right, y_axis, line_thick, height - y_axis,
                   C_MONDRIAN_BLACK);
    dsgl_fill_rect(c, x_split_right, y_split_right, width - x_split_right,
                   line_thick, C_MONDRIAN_BLACK);
    return c;
}
