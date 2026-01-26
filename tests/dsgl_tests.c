#include <stdlib.h>

#include "dsgl.h"
#include "unity.h"

void setUp(void)
{
}
void tearDown(void)
{
}

void test_canvas_creation(void)
{
    int w = 100;
    int h = 100;
    uint32_t *pixels = calloc(w * h, sizeof(*pixels));
    Dsgl_Canvas c = dsgl_create_canvas(pixels, w, h);
    TEST_ASSERT_NOT_NULL(c.pixels);
    TEST_ASSERT_EQUAL_INT(100, c.width);
    TEST_ASSERT_EQUAL_INT(100, c.height);
    free(pixels);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_canvas_creation);
    return UNITY_END();
}
