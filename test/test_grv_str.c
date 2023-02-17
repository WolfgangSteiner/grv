#include "grv_str.h"
#include "grv_test.h"
#include <stdlib.h>

char* lorem = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";

GRV_TEST_BEGIN(grv_str_new)
  grv_str s = grv_str_new("TEST");
  GRV_TEST_ASSERT_EQUAL(grv_str_get_start(&s), 0);
  GRV_TEST_ASSERT_EQUAL(grv_str_get_end(&s), 4);
  GRV_TEST_ASSERT_EQUAL(grv_str_get_capacity(&s), GRV_STR_SSO_CAPACITY);
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s), 4);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&s), true);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_get_buffer(&s), "TEST");
  
  grv_str s2 = grv_str_new(lorem);
  size_t lorem_length = strlen(lorem);
  size_t lorem_capacity = lorem_length + 1;
  size_t s2_capacity = 
    lorem_capacity % GRV_STR_ALLOC_GRANULARITY
    ? (lorem_capacity / GRV_STR_ALLOC_GRANULARITY + 1) * GRV_STR_ALLOC_GRANULARITY
    : lorem_capacity;

  GRV_TEST_ASSERT_EQUAL(grv_str_get_start(&s2), 0);
  GRV_TEST_ASSERT_EQUAL(grv_str_get_end(&s2), lorem_length);
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s2), lorem_length);
  GRV_TEST_ASSERT_EQUAL(grv_str_get_capacity(&s2), s2_capacity);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&s2), false);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_get_buffer(&s2), lorem);
GRV_TEST_END()


GRV_TEST_BEGIN(grv_str_cat)
  grv_str a = grv_str_new("Hello");
  grv_str b = grv_str_new("World");
  grv_str r = grv_str_cat(&a, &b);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_get_buffer(&r), "HelloWorld");
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&r), (int)strlen(grv_str_get_buffer(&r)));
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&r), true);

  grv_str longstr = grv_str_new(lorem); 
  grv_str r2 = grv_str_cat(&a, &longstr);
  char* r2_cstr = malloc(4096);
  strcpy(r2_cstr, grv_str_get_buffer(&a));
  strcat(r2_cstr, lorem);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&r2), false);
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&r2), strlen(r2_cstr));
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_get_buffer(&r2), r2_cstr);

  strcpy(r2_cstr, lorem);
  strcat(r2_cstr, grv_str_get_buffer(&b));
  r2 = grv_str_cat(&longstr, &b);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&r2), false);
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&r2), strlen(r2_cstr));
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_get_buffer(&r2), r2_cstr);

  strcpy(r2_cstr, lorem);
  strcat(r2_cstr, lorem);
  r2 = grv_str_cat(&longstr, &longstr);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&r2), false);
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&r2), strlen(r2_cstr));
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_get_buffer(&r2), r2_cstr);
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_resize)
  grv_str s1 = grv_str_new("Hello");
  GRV_TEST_ASSERT_EQUAL(grv_str_get_capacity(&s1), GRV_STR_SSO_CAPACITY);
  grv_str_resize(&s1, 10);
  GRV_TEST_ASSERT_EQUAL(grv_str_get_capacity(&s1), GRV_STR_SSO_CAPACITY);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&s1), true);
  grv_str_resize(&s1, 32);
  GRV_TEST_ASSERT_EQUAL(grv_str_get_capacity(&s1), GRV_STR_ALLOC_GRANULARITY);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&s1), false);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_get_buffer(&s1), "Hello");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_get_cstr)
  grv_str s1 = grv_str_new("Hello World");
  char* cstr1 = grv_str_get_cstr(&s1);
  GRV_TEST_ASSERT_EQUAL_STR(cstr1, "Hello World");

  grv_str s2 = grv_str_new(lorem);
  char* cstr2 = grv_str_get_cstr(&s2);
  GRV_TEST_ASSERT_EQUAL_STR(cstr2, lorem);

  s2.start = 6;
  s2.end = 11;
  cstr2 = grv_str_get_cstr(&s2);
  GRV_TEST_ASSERT_EQUAL_STR(cstr2, "ipsum");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_append_cstr)
  grv_str s1 = grv_str_new("Hello");
  grv_str_append_cstr(&s1, " ");
  grv_str_append_cstr(&s1, "World");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_get_buffer(&s1), "Hello World");
  grv_str_append_cstr(&s1, "0123456789");
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s1), 21);
  grv_str_append_cstr(&s1, "012345678");
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s1), 30);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&s1), true);
  grv_str_append_cstr(&s1, "9");
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s1), 31);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&s1), false);
  GRV_TEST_ASSERT_EQUAL(grv_str_get_capacity(&s1), GRV_STR_ALLOC_GRANULARITY);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_get_buffer(&s1), "Hello World01234567890123456789");
  grv_str_append_cstr(&s1, lorem);
  GRV_TEST_ASSERT_EQUAL(grv_str_get_capacity(&s1), (strlen(lorem) + 31 + (GRV_STR_ALLOC_GRANULARITY - 1)) / GRV_STR_ALLOC_GRANULARITY * GRV_STR_ALLOC_GRANULARITY );
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_ends_with) 
  grv_str s = grv_str_new("Hello World");
  GRV_TEST_ASSERT_EQUAL(grv_str_ends_with_cstr(&s, "World"), true);
  GRV_TEST_ASSERT_EQUAL(grv_str_ends_with_cstr(&s, "Worl"), false);
  s = grv_str_new(lorem);
  GRV_TEST_ASSERT_EQUAL(grv_str_ends_with_cstr(&s, "amet."), true);
  GRV_TEST_ASSERT_EQUAL(grv_str_ends_with_cstr(&s, "amet"), false);
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_starts_with) 
  grv_str s = grv_str_new("Hello World");
  GRV_TEST_ASSERT_EQUAL(grv_str_starts_with_cstr(&s, "Hello"), true);
  GRV_TEST_ASSERT_EQUAL(grv_str_starts_with_cstr(&s, "ello"), false);
  s = grv_str_new(lorem);
  GRV_TEST_ASSERT_EQUAL(grv_str_starts_with_cstr(&s, "Lorem"), true);
  GRV_TEST_ASSERT_EQUAL(grv_str_starts_with_cstr(&s, "orem"), false);
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_lstrip)
  grv_str s = grv_str_new(" \t\nHello");
  grv_str_lstrip(&s);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_get_cstr(&s), "Hello");
  grv_str_lstrip(&s);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_get_cstr(&s), "Hello");
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s), strlen("Hello"));
  s = grv_str_new("    ");
  grv_str_lstrip(&s);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_get_cstr(&s), "");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_rstrip)
  grv_str s = grv_str_new("Hello \t\n");
  grv_str_rstrip(&s);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_get_cstr(&s), "Hello");
  grv_str_rstrip(&s);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_get_cstr(&s), "Hello");
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s), strlen("Hello"));
  s = grv_str_new("    ");
  grv_str_rstrip(&s);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_get_cstr(&s), "");
GRV_TEST_END()


int main(void) {
  GRV_TEST_PERFORM(grv_str_new);
  GRV_TEST_PERFORM(grv_str_cat);
  GRV_TEST_PERFORM(grv_str_resize);
  GRV_TEST_PERFORM(grv_str_get_cstr);
  GRV_TEST_PERFORM(grv_str_append_cstr);
  GRV_TEST_PERFORM(grv_str_ends_with);
  GRV_TEST_PERFORM(grv_str_starts_with);
  GRV_TEST_PERFORM(grv_str_lstrip);
  GRV_TEST_PERFORM(grv_str_rstrip);
}
