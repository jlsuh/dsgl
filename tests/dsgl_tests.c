#include <inttypes.h>
#include <stdlib.h>

#include "dsgl.h"
#include "unity.h"

#define COLOR 0xAABBCCDD
#define WIDTH 10
#define HEIGHT 10

void assert_rect_drawn(Dsgl_Canvas c, int rx, int ry, int rw, int rh,
                       uint32_t color)
{
    for (int y = 0; y < c.height; y++) {
        for (int x = 0; x < c.width; x++) {
            int expected_color = 0;
            if (x >= rx && x < (rx + rw) && y >= ry && y < (ry + rh)) {
                expected_color = color;
            }
            char message[50];
            snprintf(message, sizeof(message), "Pixel mismatch at (%d, %d)", x,
                     y);
            TEST_ASSERT_EQUAL_HEX32_MESSAGE(expected_color,
                                            c.pixels[y * c.width + x], message);
        }
    }
}

void setUp(void)
{
}

void tearDown(void)
{
}

//////////////////// dsgl_create_canvas ////////////////////
void Creating_a_canvas_with_null_pixels_returns_a_null_canvas(void)
{
    Dsgl_Canvas c = dsgl_create_canvas(NULL, WIDTH, HEIGHT);
    TEST_ASSERT_NULL(c.pixels);
}

void Creating_a_canvas_with_0_width_returns_a_null_canvas(void)
{
    Dsgl_Canvas c = dsgl_create_canvas(NULL, 0, HEIGHT);
    TEST_ASSERT_NULL(c.pixels);
}

void Creating_a_canvas_with_negative_width_returns_a_null_canvas(void)
{
    Dsgl_Canvas c = dsgl_create_canvas(NULL, -1, HEIGHT);
    TEST_ASSERT_NULL(c.pixels);
}

void Creating_a_canvas_with_0_height_returns_a_null_canvas(void)
{
    Dsgl_Canvas c = dsgl_create_canvas(NULL, WIDTH, 0);
    TEST_ASSERT_NULL(c.pixels);
}

void Creating_a_canvas_with_negative_height_returns_a_null_canvas(void)
{
    Dsgl_Canvas c = dsgl_create_canvas(NULL, WIDTH, -1);
    TEST_ASSERT_NULL(c.pixels);
}

void Creating_a_canvas_with_valid_parameters_returns_an_initialized_canvas(void)
{
    uint32_t pixels;
    Dsgl_Canvas c = dsgl_create_canvas(&pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_PTR(&pixels, c.pixels);
    TEST_ASSERT_EQUAL_INT(WIDTH, c.width);
    TEST_ASSERT_EQUAL_INT(HEIGHT, c.height);
}

//////////////////// dsgl_fill_rect ////////////////////
void Filling_a_rectangle_on_a_canvas_with_null_pixels_returns_failure(void)
{
    Dsgl_Canvas c = DSGL_CANVAS_NULL;
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(&c, 0, 0, WIDTH, HEIGHT, 0xFFFFFFFF));
}

void Filling_a_rectangle_with_0_width_returns_failure(void)
{
    uint32_t pixels[100];
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(&c, 0, 0, 0, HEIGHT, 0xFFFFFFFF));
}

void Filling_a_rectangle_with_0_height_returns_failure(void)
{
    uint32_t pixels[100];
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(&c, 0, 0, WIDTH, 0, 0xFFFFFFFF));
}

void Filling_top_left_negative_dims_clipped_entirely(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(&c, 0, 0, -1, -1, COLOR));
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(&c, 0, 0, -1, 1, COLOR));
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(&c, 0, 0, 1, -1, COLOR));
}

void Filling_top_left_positive_dims_draws_origin(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS, dsgl_fill_rect(&c, 0, 0, 1, 1, COLOR));
    assert_rect_drawn(c, 0, 0, 1, 1, COLOR);
}

void Filling_bottom_left_upwards_draws_previous_row(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS,
                           dsgl_fill_rect(&c, 0, HEIGHT - 1, 1, -1, COLOR));
    assert_rect_drawn(c, 0, HEIGHT - 2, 1, 1, COLOR);
}

void Filling_bottom_left_downwards_draws_last_row(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS,
                           dsgl_fill_rect(&c, 0, HEIGHT - 1, 1, 1, COLOR));
    assert_rect_drawn(c, 0, HEIGHT - 1, 1, 1, COLOR);
}

void Filling_bottom_left_leftwards_clipped(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(&c, 0, HEIGHT - 1, -1, 1, COLOR));
}

void Filling_top_right_leftwards_draws_previous_col(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS,
                           dsgl_fill_rect(&c, WIDTH - 1, 0, -1, 1, COLOR));
    assert_rect_drawn(c, WIDTH - 2, 0, 1, 1, COLOR);
}

void Filling_top_right_rightwards_draws_last_col(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS,
                           dsgl_fill_rect(&c, WIDTH - 1, 0, 1, 1, COLOR));
    assert_rect_drawn(c, WIDTH - 1, 0, 1, 1, COLOR);
}

void Filling_top_right_upwards_clipped(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(&c, WIDTH - 1, 0, 1, -1, COLOR));
}

void Filling_bottom_right_diagonal_up_left(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(
        DSGL_SUCCESS, dsgl_fill_rect(&c, WIDTH - 1, HEIGHT - 1, -1, -1, COLOR));
    assert_rect_drawn(c, WIDTH - 2, HEIGHT - 2, 1, 1, COLOR);
}

void Filling_bottom_right_out_of_bounds_positive(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(&c, WIDTH, HEIGHT, 1, 1, COLOR));
}

void Filling_a_rectangle_extending_off_the_left_edge_clips_correctly(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS,
                           dsgl_fill_rect(&c, -1, 0, 2, 1, COLOR));
    assert_rect_drawn(c, 0, 0, 1, 1, COLOR);
}

void Filling_a_rectangle_entirely_outside_the_left_edge_returns_failure(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(&c, -1, 0, 1, 1, COLOR));
}

void Filing_a_rectangle_extending_off_the_top_edge_clips_correctly(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS,
                           dsgl_fill_rect(&c, 0, -1, 1, 2, COLOR));
    assert_rect_drawn(c, 0, 0, 1, 1, COLOR);
}

void Filling_a_rectangle_entirely_above_the_top_edge_returns_failure(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(&c, 0, -1, 1, 1, COLOR));
}

void Filling_a_rectangle_at_the_right_edge_boundary_succeeds(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS,
                           dsgl_fill_rect(&c, WIDTH - 1, 0, 1, 1, COLOR));
    assert_rect_drawn(c, WIDTH - 1, 0, 1, 1, COLOR);
}

void Filling_a_rectangle_entirely_past_the_right_edge_returns_failure(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(&c, WIDTH, 0, 1, 1, COLOR));
}

void Filling_a_single_pixel_at_the_right_edge_succeeds(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS,
                           dsgl_fill_rect(&c, WIDTH - 1, 0, 1, HEIGHT, COLOR));
    assert_rect_drawn(c, WIDTH - 1, 0, 1, HEIGHT, COLOR);
}

void Filling_a_rectangle_at_the_bottom_edge_boundary_succeeds(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS,
                           dsgl_fill_rect(&c, 0, HEIGHT - 1, 1, 1, COLOR));
    assert_rect_drawn(c, 0, HEIGHT - 1, 1, 1, COLOR);
}

void Filling_a_rectangle_entirely_below_the_bottom_edge_returns_failure(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(&c, 0, HEIGHT, 1, 1, COLOR));
}

void Filling_a_single_pixel_at_the_bottom_edge_succeeds(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS,
                           dsgl_fill_rect(&c, 0, HEIGHT - 1, WIDTH, 1, COLOR));
    assert_rect_drawn(c, 0, HEIGHT - 1, WIDTH, 1, COLOR);
}

void Filling_a_rectangle_larger_than_canvas_covers_entire_area(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(
        DSGL_SUCCESS,
        dsgl_fill_rect(&c, -50, -50, WIDTH + 100, HEIGHT + 100, COLOR));
    assert_rect_drawn(c, 0, 0, WIDTH, HEIGHT, COLOR);
}

void Run_dsgl_create_canvas_tests(void)
{
    RUN_TEST(Creating_a_canvas_with_null_pixels_returns_a_null_canvas);
    RUN_TEST(Creating_a_canvas_with_0_width_returns_a_null_canvas);
    RUN_TEST(Creating_a_canvas_with_negative_width_returns_a_null_canvas);
    RUN_TEST(Creating_a_canvas_with_0_height_returns_a_null_canvas);
    RUN_TEST(Creating_a_canvas_with_negative_height_returns_a_null_canvas);
    RUN_TEST(
        Creating_a_canvas_with_valid_parameters_returns_an_initialized_canvas);
}

void Run_dsgl_fill_rect_tests(void)
{
    RUN_TEST(Filling_a_rectangle_on_a_canvas_with_null_pixels_returns_failure);
    RUN_TEST(Filling_a_rectangle_with_0_width_returns_failure);
    RUN_TEST(Filling_a_rectangle_with_0_height_returns_failure);
    RUN_TEST(Filling_top_left_negative_dims_clipped_entirely);
    RUN_TEST(Filling_top_left_positive_dims_draws_origin);
    RUN_TEST(Filling_bottom_left_upwards_draws_previous_row);
    RUN_TEST(Filling_bottom_left_downwards_draws_last_row);
    RUN_TEST(Filling_bottom_left_leftwards_clipped);
    RUN_TEST(Filling_top_right_leftwards_draws_previous_col);
    RUN_TEST(Filling_top_right_rightwards_draws_last_col);
    RUN_TEST(Filling_top_right_upwards_clipped);
    RUN_TEST(Filling_bottom_right_diagonal_up_left);
    RUN_TEST(Filling_bottom_right_out_of_bounds_positive);
    RUN_TEST(Filling_a_rectangle_extending_off_the_left_edge_clips_correctly);
    RUN_TEST(
        Filling_a_rectangle_entirely_outside_the_left_edge_returns_failure);
    RUN_TEST(Filing_a_rectangle_extending_off_the_top_edge_clips_correctly);
    RUN_TEST(Filling_a_rectangle_entirely_above_the_top_edge_returns_failure);
    RUN_TEST(Filling_a_rectangle_at_the_right_edge_boundary_succeeds);
    RUN_TEST(Filling_a_rectangle_entirely_past_the_right_edge_returns_failure);
    RUN_TEST(Filling_a_single_pixel_at_the_right_edge_succeeds);
    RUN_TEST(Filling_a_rectangle_at_the_bottom_edge_boundary_succeeds);
    RUN_TEST(
        Filling_a_rectangle_entirely_below_the_bottom_edge_returns_failure);
    RUN_TEST(Filling_a_single_pixel_at_the_bottom_edge_succeeds);
    RUN_TEST(Filling_a_rectangle_larger_than_canvas_covers_entire_area);
}

int main(void)
{
    UNITY_BEGIN();
    Run_dsgl_create_canvas_tests();
    Run_dsgl_fill_rect_tests();
    return UNITY_END();
}
