#include <inttypes.h>
#include <stdlib.h>

#include "dsgl.h"
#include "unity.h"

#define COLOR 0xAABBCCDD
#define WIDTH 10
#define HEIGHT 10

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

void Creating_a_canvas_with_width_greater_than_maximum_dimension_returns_a_null_canvas(
    void)
{
    Dsgl_Canvas c = dsgl_create_canvas(NULL, DSGL_MAX_DIM + 1, HEIGHT);
    TEST_ASSERT_NULL(c.pixels);
}

void Creating_a_canvas_with_height_greater_than_maximum_dimension_returns_a_null_canvas(
    void)
{
    Dsgl_Canvas c = dsgl_create_canvas(NULL, WIDTH, DSGL_MAX_DIM + 1);
    TEST_ASSERT_NULL(c.pixels);
}

void Creating_a_canvas_with_maximum_dimensions_returns_an_initialized_canvas(
    void)
{
    uint32_t pixels;
    Dsgl_Canvas c = dsgl_create_canvas(&pixels, DSGL_MAX_DIM, DSGL_MAX_DIM);
    TEST_ASSERT_NOT_NULL(c.pixels);
    TEST_ASSERT_EQUAL_INT(DSGL_MAX_DIM, c.width);
    TEST_ASSERT_EQUAL_INT(DSGL_MAX_DIM, c.height);
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
                           dsgl_fill_rect(c, 0, 0, WIDTH, HEIGHT, 0xFFFFFFFF));
}

void Filling_a_rectangle_with_0_width_returns_failure(void)
{
    uint32_t pixels[100];
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(c, 0, 0, 0, HEIGHT, 0xFFFFFFFF));
}

void Filling_a_rectangle_with_negative_width_returns_failure(void)
{
    uint32_t pixels[100];
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(c, 0, 0, -1, HEIGHT, 0xFFFFFFFF));
}

void Filling_a_rectangle_with_0_height_returns_failure(void)
{
    uint32_t pixels[100];
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(c, 0, 0, WIDTH, 0, 0xFFFFFFFF));
}

void Filling_a_rectangle_with_negative_height_returns_failure(void)
{
    uint32_t pixels[100];
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(c, 0, 0, WIDTH, -1, 0xFFFFFFFF));
}

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

////////// left //////////
void Filling_a_rectangle_extending_off_the_left_edge_clips_correctly(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS, dsgl_fill_rect(c, -1, 0, 2, 1, COLOR));
    assert_rect_drawn(c, 0, 0, 1, 1, COLOR);
}

void Filling_a_rectangle_entirely_outside_the_left_edge_returns_failure(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE, dsgl_fill_rect(c, -1, 0, 1, 1, COLOR));
}

////////// top //////////
void Filing_a_rectangle_extending_off_the_top_edge_clips_correctly(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS, dsgl_fill_rect(c, 0, -1, 1, 2, COLOR));
    assert_rect_drawn(c, 0, 0, 1, 1, COLOR);
}

void Filling_a_rectangle_entirely_above_the_top_edge_returns_failure(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE, dsgl_fill_rect(c, 0, -1, 1, 1, COLOR));
}

////////// right //////////
void Filling_a_rectangle_at_the_right_edge_boundary_succeeds(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS,
                           dsgl_fill_rect(c, WIDTH - 1, 0, 1, 1, COLOR));
    assert_rect_drawn(c, WIDTH - 1, 0, 1, 1, COLOR);
}

void Filling_a_rectangle_entirely_past_the_right_edge_returns_failure(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(c, WIDTH, 0, 1, 1, COLOR));
}

void Filling_a_single_pixel_at_the_right_edge_succeeds(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS,
                           dsgl_fill_rect(c, WIDTH - 1, 0, 1, HEIGHT, COLOR));
    assert_rect_drawn(c, WIDTH - 1, 0, 1, HEIGHT, COLOR);
}

////////// bottom //////////
void Filling_a_rectangle_at_the_bottom_edge_boundary_succeeds(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS,
                           dsgl_fill_rect(c, 0, HEIGHT - 1, 1, 1, COLOR));
    assert_rect_drawn(c, 0, HEIGHT - 1, 1, 1, COLOR);
}

void Filling_a_rectangle_entirely_below_the_bottom_edge_returns_failure(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_FAILURE,
                           dsgl_fill_rect(c, 0, HEIGHT, 1, 1, COLOR));
}

void Filling_a_single_pixel_at_the_bottom_edge_succeeds(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(DSGL_SUCCESS,
                           dsgl_fill_rect(c, 0, HEIGHT - 1, WIDTH, 1, COLOR));
    assert_rect_drawn(c, 0, HEIGHT - 1, WIDTH, 1, COLOR);
}

////////// full area //////////
void Filling_a_rectangle_larger_than_canvas_covers_entire_area(void)
{
    uint32_t pixels[WIDTH * HEIGHT] = {0};
    Dsgl_Canvas c = dsgl_create_canvas(pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_INT8(
        DSGL_SUCCESS,
        dsgl_fill_rect(c, -50, -50, WIDTH + 100, HEIGHT + 100, COLOR));
    assert_rect_drawn(c, 0, 0, WIDTH, HEIGHT, COLOR);
}

int main(void)
{
    UNITY_BEGIN();

    //////////////////// dsgl_create_canvas ////////////////////
    RUN_TEST(Creating_a_canvas_with_null_pixels_returns_a_null_canvas);
    RUN_TEST(Creating_a_canvas_with_0_width_returns_a_null_canvas);
    RUN_TEST(Creating_a_canvas_with_negative_width_returns_a_null_canvas);
    RUN_TEST(Creating_a_canvas_with_0_height_returns_a_null_canvas);
    RUN_TEST(Creating_a_canvas_with_negative_height_returns_a_null_canvas);
    RUN_TEST(
        Creating_a_canvas_with_width_greater_than_maximum_dimension_returns_a_null_canvas);
    RUN_TEST(
        Creating_a_canvas_with_height_greater_than_maximum_dimension_returns_a_null_canvas);
    RUN_TEST(
        Creating_a_canvas_with_maximum_dimensions_returns_an_initialized_canvas);
    RUN_TEST(
        Creating_a_canvas_with_valid_parameters_returns_an_initialized_canvas);

    //////////////////// dsgl_fill_rect ////////////////////
    RUN_TEST(Filling_a_rectangle_on_a_canvas_with_null_pixels_returns_failure);
    RUN_TEST(Filling_a_rectangle_with_0_width_returns_failure);
    RUN_TEST(Filling_a_rectangle_with_negative_width_returns_failure);
    RUN_TEST(Filling_a_rectangle_with_0_height_returns_failure);
    RUN_TEST(Filling_a_rectangle_with_negative_height_returns_failure);

    ////////// left //////////
    RUN_TEST(Filling_a_rectangle_extending_off_the_left_edge_clips_correctly);
    RUN_TEST(
        Filling_a_rectangle_entirely_outside_the_left_edge_returns_failure);

    ////////// top //////////
    RUN_TEST(Filing_a_rectangle_extending_off_the_top_edge_clips_correctly);
    RUN_TEST(Filling_a_rectangle_entirely_above_the_top_edge_returns_failure);

    ////////// right //////////
    RUN_TEST(Filling_a_rectangle_at_the_right_edge_boundary_succeeds);
    RUN_TEST(Filling_a_rectangle_entirely_past_the_right_edge_returns_failure);
    RUN_TEST(Filling_a_single_pixel_at_the_right_edge_succeeds);

    ////////// bottom //////////
    RUN_TEST(Filling_a_rectangle_at_the_bottom_edge_boundary_succeeds);
    RUN_TEST(
        Filling_a_rectangle_entirely_below_the_bottom_edge_returns_failure);
    RUN_TEST(Filling_a_single_pixel_at_the_bottom_edge_succeeds);

    ////////// full area //////////
    RUN_TEST(Filling_a_rectangle_larger_than_canvas_covers_entire_area);

    return UNITY_END();
}
