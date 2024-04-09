#include "grv/grv_str.h"
#include "grv/grv_test.h"
#include "grv/grv.h"

GRV_TEST_BEGIN_NEW(grv_str_to_snake_case)
    grv_str_t s = grv_str_ref("This is A test");
    grv_str_t snake = grv_str_to_snake_case(s);
    GRV_TEST_EQUAL_STR(snake, "this_is_a_test");

    s = grv_str_ref("ThisIsPascalCase");
    snake = grv_str_to_snake_case(s);
    GRV_TEST_EQUAL_STR(snake, "this_is_pascal_case");

    s = grv_str_ref("this Is some WeirdCase");
    snake = grv_str_to_snake_case(s);
    GRV_TEST_EQUAL_STR(snake, "this_is_some_weird_case");
GRV_TEST_END_NEW()
