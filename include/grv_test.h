#ifndef GRV_TEST_H
#define GRV_TEST_H

#include <stdio.h>
#include <string.h>
#include "grv_defines.h"

// print the name of the test group
#define GRV_TEST_INIT(NAME) \
  printf("\nTesting %s\n", #NAME);

#define GRV_TEST_BEGIN(NAME) \
  static void test_##NAME() { \
    int grv_test_total_count = 0; \
    int grv_test_failed_count = 0; \
    printf("Testing %s", #NAME); \
    fflush(stdout); 

#define GRV_TEST_END() \
    if (grv_test_failed_count == 0) printf(" ... passed.\n"); \
    else printf("\nfailed %d/%d.\n\n", grv_test_failed_count, grv_test_total_count); \
  }

#define GRV_TEST_RUN(NAME) \
  test_##NAME();

#define GRV_TEST_ASSERT_EQUAL(A, B) GRV_TEST_ASSERT_EQUAL_IMPL(A, B, __FILE__, __LINE__)
#define GRV_TEST_ASSERT_EQUAL_STR(A, B) GRV_TEST_ASSERT_EQUAL_STR_IMPL(A, B, __FILE__, __LINE__)

#define GRV_TEST_ASSERT_EQUAL_IMPL(A, B, FILE, LINE) \
    grv_test_total_count++; \
    if ((A) != (B)) { \
      grv_test_failed_count++; \
      printf("\n  [ERROR] %s:%d %s is %d, expected %d.", FILE, LINE, #A, (int)(A), (int)(B)); \
    }

#define GRV_TEST_ASSERT_EQUAL_STR_IMPL(A, B, FILE, LINE) \
    if (strcmp(A, B)) { \
      grv_test_failed_count++; \
      printf("\n  [ERROR] %s:%d %s is \"%s\", expected \"%s\".", FILE, LINE, #A, A, B); \
    }


#endif // GRV_TEST_H
