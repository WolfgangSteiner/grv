#ifndef GRV_TEST_H
#define GRV_TEST_H

#include <stdio.h>
#include <string.h>
#include "grv_defines.h"

#ifndef GRV_TEST_ALIGN_WIDTH
  #define GRV_TEST_ALIGN_WIDTH 64
#endif

#define GRV_TEST_RED(TEXT) "\x1B[31m" #TEXT "\x1B[0m"
#define GRV_TEST_GREEN(TEXT) "\x1B[32m" #TEXT "\x1B[0m"

#define GRV_TEST_ERROR_MSG_PREFIX "      " GRV_TEST_RED([ERROR]) " " 
#define GRV_TEST_NEWLINE() printf("\n");
#define GRV_TEST_ERROR(EXPR, FMT, A, B) \
  if (GRV_TEST_VERBOSITY) GRV_TEST_NEWLINE(); \
  printf(GRV_TEST_ERROR_MSG_PREFIX "%s:%d %s is " FMT " expected " FMT ".\n", __FILE__, __LINE__, EXPR, A, B);

//#define GRV_TEST_VERBOSE
#ifdef GRV_TEST_VERBOSE
  #define GRV_TEST_VERBOSITY 1
#else
  #define GRV_TEST_VERBOSITY 0
#endif

// print the name of the test group
#define GRV_TEST_INIT(NAME) \
  printf(GRV_TEST_GREEN([TEST])" %s\n", #NAME); \
  
#define GRV_TEST_BEGIN(NAME) \
  static void test_##NAME() { \
    int grv_test_total_count = 0; \
    (void)grv_test_total_count; \
    int grv_test_failed_count = 0; \
    char* grv_test_current = #NAME; \
    if (GRV_TEST_VERBOSITY) { \
      printf("       %s", grv_test_current); \
      fflush(stdout); \
    } \



#define GRV_TEST_END() \
    if (grv_test_failed_count == 0) { \
      grv_str padding = grv_str_repeat_char('.', GRV_TEST_ALIGN_WIDTH - strlen(grv_test_current) - 4);\
      if (GRV_TEST_VERBOSITY) printf(" %s " GRV_TEST_GREEN(ok) "\n", grv_str_cstr(&padding)); \
      grv_str_free(&padding); \
    } \
  }

#define GRV_TEST_RUN(NAME) \
  test_##NAME();

#define GRV_TEST_ASSERT_TRUE(A) \
    grv_test_total_count++; \
    if (!(A)) { \
      grv_test_failed_count++; \
      GRV_TEST_ERROR(#A, "%s", "false", "true"); \
    }

#define GRV_TEST_ASSERT_FALSE(A) \
    grv_test_total_count++; \
    if (A) { \
      grv_test_failed_count++; \
      GRV_TEST_ERROR(#A, "%s", "true", "false"); \
    }

#define GRV_TEST_ASSERT_EQUAL(A, B) GRV_TEST_ASSERT_EQUAL_IMPL(A, B, __FILE__, __LINE__, "%d")
#define GRV_TEST_ASSERT_EQUAL_HEX(A, B) GRV_TEST_ASSERT_EQUAL_IMPL(A, B, __FILE__, __LINE__, "%#x")
#define GRV_TEST_ASSERT_EQUAL_STR(A, B) GRV_TEST_ASSERT_EQUAL_STR_IMPL(A, B, __FILE__, __LINE__)

#define GRV_TEST_ASSERT_EQUAL_IMPL(A, B, FILE, LINE, FMT) \
    grv_test_total_count++; \
    if ((A) != (B)) { \
      grv_test_failed_count++; \
      GRV_TEST_ERROR(#A, FMT, (int)(A), (int)(B));\
    }

#define GRV_TEST_ASSERT_EQUAL_STR_IMPL(A, B, FILE, LINE) \
    if (strcmp(A, B)) { \
      grv_test_failed_count++; \
      GRV_TEST_ERROR(#A, "\"%s\"", A, B); \
    }

#define GRV_TEST_FAIL() \
    grv_test_failed_count++; \
    printf("\n  [ERROR] %s:%d Test failed", __FILE__, __LINE__); 


#endif // GRV_TEST_H
