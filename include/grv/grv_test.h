#ifndef GRV_TEST_H
#define GRV_TEST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "grv_base.h"
#include "grv_str.h"
#include "grv_cstr.h"
#include "grv_memory.h"

typedef struct {
  grv_str_t name;
  int total_count;
  int failed_count;
} _grv_test_t;
  
#ifndef GRV_TEST_ALIGN_WIDTH
  #define GRV_TEST_ALIGN_WIDTH 64
#endif

#define GRV_TEST_RED(TEXT) "\x1B[31m" #TEXT "\x1B[0m"
#define GRV_TEST_GREEN(TEXT) "\x1B[32m" #TEXT "\x1B[0m"

#define GRV_TEST_ERROR_MSG_PREFIX "          " GRV_TEST_RED([ERROR]) " " 
#define GRV_TEST_NEWLINE() puts("")
#define GRV_TEST_ERROR(EXPR, FMT, A, B) \
  if (GRV_TEST_VERBOSITY) GRV_TEST_NEWLINE(); \
  printf(GRV_TEST_ERROR_MSG_PREFIX "%s:%d %s is " FMT " expected " FMT ".\n", __FILE__, __LINE__, EXPR, A, B);

#ifndef GRV_TEST_VERBOSITY
  #define GRV_TEST_VERBOSITY 1
#endif

static inline void _put_str(grv_str_t s) { 
  for (int i = 0; i < s.size; i++) {
    putchar(s.data[i]);
  }
  fflush(stdout);
}
static inline void _put(char* s) { printf("%s", s); }
static inline void _newline(void) { puts(""); }
static inline void _grv_test_begin(_grv_test_t test) {
  if (GRV_TEST_VERBOSITY) {
    _put("       "); 
    _put_str(test.name); 
    fflush(stdout); 
  }
}
static inline void _grv_test_begin_new(_grv_test_t test) {
  if (GRV_TEST_VERBOSITY) {
    _put(GRV_TEST_GREEN([TEST]));
    _put(" ");
    _put_str(test.name); 
    fflush(stdout); 
  }
}

// print the name of the .est group
#define GRV_TEST_INIT(NAME) \
  bool success = true; \
  _put(GRV_TEST_GREEN([TEST]));\
  _put(" "); \
  _put(#NAME); \
  _newline();


#define GRV_TEST_FINISH() \
  return success ? 0 : 1;
  

#define GRV_TEST_BEGIN(NAME) \
  static bool test_##NAME(void) { \
    _grv_test_t _test = { .name = grv_str_ref(#NAME) }; \
    _grv_test_begin(_test); \

static inline bool _grv_test_end(_grv_test_t test) {
  bool success = test.failed_count == 0;
  if (success) {
    if (GRV_TEST_VERBOSITY) {
      int padding_count = GRV_TEST_ALIGN_WIDTH - (i32)(test.name.size) - 4;
      grv_str_t padding = grv_str_repeat_char('.', padding_count);
      printf(" %s " GRV_TEST_GREEN(ok) "\n", grv_str_cstr(padding));
      grv_str_free(&padding);
    }
  }

  return success;
}

#define GRV_TEST_END() return _grv_test_end(_test); } 

#define GRV_TEST_BEGIN_NEW(NAME) \
  bool test_##NAME(void); \
  static bool execute_test(void) { \
    return test_##NAME(); \
  } \
  bool test_##NAME(void) { \
    _grv_test_t _test = { .name = grv_str_ref(#NAME) }; \
    _grv_test_begin_new(_test); \

#define GRV_TEST_END_NEW() \
  return _grv_test_end(_test); }\
  int main(void) { \
    bool success = execute_test(); \
    return success ? 0 : 1; \
  }

#define GRV_TEST_RUN(NAME) \
  success = success && test_##NAME();

#define GRV_TEST_EQUAL_PTR(ACTUAL, EXPECTED) _grv_test_equal_ptr(&_test, ACTUAL, EXPECTED, __FILE__, __LINE__)  
#define GRV_TEST_NULLPTR(ACTUAL) _grv_test_nullptr(&_test, ACTUAL, __FILE__, __LINE__)
#define GRV_TEST_NOT_NULLPTR(ACTUAL) _grv_test_not_nullptr(&_test, ACTUAL, __FILE__, __LINE__) 
#define GRV_TEST_TRUE(ACTUAL) _grv_test_true(&_test, #ACTUAL, ACTUAL, __FILE__, __LINE__) 
#define GRV_TEST_FALSE(ACTUAL) _grv_test_false(&_test, #ACTUAL, ACTUAL, __FILE__, __LINE__) 
#define GRV_TEST_CLOSE_F32(A, B, EPSILON) _grv_test_close_f32(&_test, #A, A, B, EPSILON, __FILE__, __LINE__) 
#define GRV_TEST_EQUAL_F32(A, B) _grv_test_equal_f32(&_test, #A, A, B, __FILE__, __LINE__)
#define GRV_TEST_FAIL() _grv_test_fail(&_test, __FILE__, __LINE__)
#define GRV_TEST_EQUAL_INT(A, B) _grv_test_equal_int(&_test, #A, A, B, __FILE__, __LINE__)
#define GRV_TEST_NOT_EQUAL_INT(A, B) _grv_test_not_equal_int(&_test, #A, A, B, __FILE__, __LINE__)
#define GRV_TEST_EQUAL_UINT(A, B) _grv_test_equal_uint(&_test, #A, A, B, __FILE__, __LINE__)
#define GRV_TEST_EQUAL_SIZE(A, B) _grv_test_equal_int(&_test, #A, A, B, __FILE__, __LINE__)

#define GRV_TEST_EQUAL_STR(A, B) _Generic((B) \
  , grv_str_t: _grv_test_equal_str  \
  , char*: _grv_test_equal_cstr \
) (&_test, #A, A, B, __FILE__, __LINE__)

#define GRV_TEST_NOT_EQUAL_STR(A, B) _Generic((B) \
  , grv_str_t: _grv_test_not_equal_str  \
  , char*: _grv_test_not_equal_cstr \
) (&_test, #A, A, B, __FILE__, __LINE__)


static inline void grv_test_print_error(_grv_test_t* test, char* msg, char* file, int line) {
  if (GRV_TEST_VERBOSITY && test->failed_count == 1) puts("");
  printf(GRV_TEST_ERROR_MSG_PREFIX "%s:%d %s\n", file, line, msg);
}

static inline void _grv_test_equal_ptr(_grv_test_t* test, void* actual, void* expected, char* file, int line) {
  test->total_count++;
  if (actual != expected) {
    test->failed_count++;
    char* msg = grv_cstr_new_with_format("Pointer values are not equal. Expected %p, got %p.", expected, actual);
    grv_test_print_error(test, msg, file, line);
  }
}

static inline void _grv_test_nullptr(_grv_test_t* test, void* actual, char* file, int line) {
  test->total_count++;
  if (actual != NULL) {
    test->failed_count++;
    char* msg = grv_cstr_new_with_format("Pointer value is not NULL. Expected NULL, got %p.", actual);
    grv_test_print_error(test, msg, file, line);
  }
}

static inline void _grv_test_not_nullptr(_grv_test_t* test, void* actual, char* file, int line) {
  test->total_count++;
  if (actual == NULL) {
    test->failed_count++;
    char* msg = grv_cstr_new_with_format("Pointer value is NULL. Expected non-NULL value.");
    grv_test_print_error(test, msg, file, line);
  }
}

static inline void _grv_test_true(_grv_test_t* test, char* expression, bool a, char* file, int line) {
  test->total_count++;
  if (!a) {
    test->failed_count++;
    char* msg = grv_cstr_new_with_format("%s is false, expected true.", expression);
    grv_test_print_error(test, msg, file, line);
    grv_free(msg);
  }
}

static inline void _grv_test_false(_grv_test_t* test, char* expression, bool a, char* file, int line) {
  test->total_count++;
  if (a) {
    test->failed_count++;
    char* msg = grv_cstr_new_with_format("%s is true, expected false.", expression);
    grv_test_print_error(test, msg, file, line);
    grv_free(msg);
  }
}

static inline void _grv_test_equal_f32(_grv_test_t* test, char* expression, f32 actual, f32 expected, char* file, int line) {
  test->total_count++;
  if (actual != expected) {
    test->failed_count++;
    char* msg = grv_cstr_new_with_format("%s is %f, expected %f.", expression, actual, expected);
    grv_test_print_error(test, msg, file, line);
    grv_free(msg);
  }
}

static inline void _grv_test_close_f32(_grv_test_t* test, char* expression, f32 actual, f32 expected, f32 epsilon, char* file, int line) {
  test->total_count++;
  if (fabs(actual - expected) > epsilon) {
    test->failed_count++;
    char* msg = grv_cstr_new_with_format("%s is %f, expected %f.", expression, actual, expected);
    grv_test_print_error(test, msg, file, line);
    grv_free(msg);
  }
}

static inline void _grv_test_equal_str(_grv_test_t* test, char* expression, grv_str_t actual, grv_str_t expected, char* file, int line) {
  test->total_count++;
  if (!grv_str_eq(actual, expected)) {
    test->failed_count++;
    char* actual_cstr = grv_str_copy_to_cstr(actual);
    char* expected_cstr = grv_str_copy_to_cstr(expected);
    char* msg = grv_cstr_new_with_format("%s is \"%s\", expected \"%s\".", expression, actual_cstr, expected_cstr);
    grv_test_print_error(test, msg, file, line);
    grv_free(actual_cstr);
    grv_free(expected_cstr);
    grv_free(msg);
  }
}
static inline void _grv_test_equal_cstr(_grv_test_t* test, char* expression, grv_str_t actual, char* expected, char* file, int line) {
  _grv_test_equal_str(test, expression, actual, grv_str_ref(expected), file, line);
}

static inline void _grv_test_not_equal_str(_grv_test_t* test, char* expression, grv_str_t actual, grv_str_t expected, char* file, int line) {
  test->total_count++;
  if (grv_str_eq(actual, expected)) {
    test->failed_count++;
    char* actual_cstr = grv_str_copy_to_cstr(actual);
    char* msg = grv_cstr_new_with_format("%s is \"%s\", expected different value.", expression, actual_cstr);
    grv_test_print_error(test, msg, file, line);
    grv_free(actual_cstr);
    grv_free(msg);
  }
}

static inline void _grv_test_not_equal_cstr(_grv_test_t* test, char* expression, grv_str_t actual, char* expected, char* file, int line) {
  _grv_test_not_equal_str(test, expression, actual, grv_str_ref(expected), file, line);
}

static inline void _grv_test_fail(_grv_test_t* test, char* file, int line) {
  test->total_count++;
  test->failed_count++;
  grv_test_print_error(test, "Test failed.", file, line);
}

static inline void _grv_test_equal_int(_grv_test_t* test, char* expression, i64 actual, i64 expected, char* file, int line) {
  test->total_count++;
  if (actual != expected) {
    test->failed_count++;
    char* msg = grv_cstr_new_with_format("%s is %lld, expected %lld.", expression, actual, expected);
    grv_test_print_error(test, msg, file, line);
    grv_free(msg);
  }
}

static inline void _grv_test_not_equal_int(_grv_test_t* test, char* expression, i64 actual, i64 expected, char* file, int line) {
  test->total_count++;
  if (actual == expected) {
    test->failed_count++;
    char* msg = grv_cstr_new_with_format("%s is %lld, expected different value.", expression, actual);
    grv_test_print_error(test, msg, file, line);
    grv_free(msg);
  }
}

static inline void _grv_test_equal_uint(_grv_test_t* test, char* expression, u64 actual, u64 expected, char* file, int line) {
  test->total_count++;
  if (actual != expected) {
    test->failed_count++;
    char* msg = grv_cstr_new_with_format("%s is %llx, expected %llx.", expression, actual, expected);
    grv_test_print_error(test, msg, file, line);
    free(msg);
  }
}

#define GRV_TEST_ASSERT_NOT_EQUAL(A, B) GRV_TEST_ASSERT_NOT_EQUAL_IMPL(A, B, __FILE__, __LINE__, "%d")
#define GRV_TEST_ASSERT_NOT_EQUAL_HEX(A, B) GRV_TEST_ASSERT_NOT_EQUAL_IMPL(A, B, __FILE__, __LINE__, "%#x")

#endif // GRV_TEST_H
