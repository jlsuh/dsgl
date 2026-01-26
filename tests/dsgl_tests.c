#include <stdlib.h>

#include "dsgl.h"
#include "unity.h"

#define WIDTH 10
#define HEIGHT 10

void setUp(void)
{
}

void tearDown(void)
{
}

void Creating_a_canvas_with_null_pixels_returns_a_null_canvas(void)
{
    Dsgl_Canvas c = dsgl_create_canvas(NULL, WIDTH, HEIGHT);
    TEST_ASSERT_NULL(c.pixels);
    TEST_ASSERT_EQUAL_INT(0, c.width);
    TEST_ASSERT_EQUAL_INT(0, c.height);
}

void Creating_a_canvas_with_0_width_returns_a_null_canvas(void)
{
    Dsgl_Canvas c = dsgl_create_canvas(NULL, 0, HEIGHT);
    TEST_ASSERT_NULL(c.pixels);
    TEST_ASSERT_EQUAL_INT(0, c.width);
    TEST_ASSERT_EQUAL_INT(0, c.height);
}

void Creating_a_canvas_with_negative_width_returns_a_null_canvas(void)
{
    Dsgl_Canvas c = dsgl_create_canvas(NULL, -1, HEIGHT);
    TEST_ASSERT_NULL(c.pixels);
    TEST_ASSERT_EQUAL_INT(0, c.width);
    TEST_ASSERT_EQUAL_INT(0, c.height);
}

void Creating_a_canvas_with_0_height_returns_a_null_canvas(void)
{
    Dsgl_Canvas c = dsgl_create_canvas(NULL, WIDTH, 0);
    TEST_ASSERT_NULL(c.pixels);
    TEST_ASSERT_EQUAL_INT(0, c.width);
    TEST_ASSERT_EQUAL_INT(0, c.height);
}

void Creating_a_canvas_with_negative_height_returns_a_null_canvas(void)
{
    Dsgl_Canvas c = dsgl_create_canvas(NULL, WIDTH, -1);
    TEST_ASSERT_NULL(c.pixels);
    TEST_ASSERT_EQUAL_INT(0, c.width);
    TEST_ASSERT_EQUAL_INT(0, c.height);
}

void Creating_a_canvas_with_width_greater_than_maximum_dimension_returns_a_null_canvas(
    void)
{
    Dsgl_Canvas c = dsgl_create_canvas(NULL, DSGL_MAX_DIM + 1, HEIGHT);
    TEST_ASSERT_NULL(c.pixels);
    TEST_ASSERT_EQUAL_INT(0, c.width);
    TEST_ASSERT_EQUAL_INT(0, c.height);
}

void Creating_a_canvas_with_height_greater_than_maximum_dimension_returns_a_null_canvas(
    void)
{
    Dsgl_Canvas c = dsgl_create_canvas(NULL, WIDTH, DSGL_MAX_DIM + 1);
    TEST_ASSERT_NULL(c.pixels);
    TEST_ASSERT_EQUAL_INT(0, c.width);
    TEST_ASSERT_EQUAL_INT(0, c.height);
}

void Creating_a_canvas_with_maximum_dimensions_returns_an_initialized_canvas(
    void)
{
    uint32_t pixels;
    Dsgl_Canvas c = dsgl_create_canvas(&pixels, DSGL_MAX_DIM, DSGL_MAX_DIM);
    TEST_ASSERT_NOT_NULL(c.pixels);
    TEST_ASSERT_EQUAL_INT(DSGL_MAX_DIM, c.width);
}

void Creating_a_canvas_with_valid_parameters_returns_an_initialized_canvas(void)
{
    uint32_t pixels;
    Dsgl_Canvas c = dsgl_create_canvas(&pixels, WIDTH, HEIGHT);
    TEST_ASSERT_EQUAL_PTR(&pixels, c.pixels);
    TEST_ASSERT_EQUAL_INT(WIDTH, c.width);
    TEST_ASSERT_EQUAL_INT(HEIGHT, c.height);
}

int main(void)
{
    UNITY_BEGIN();
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
    return UNITY_END();
}
