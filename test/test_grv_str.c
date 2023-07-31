#include "grv_str.h"
#include "grv/test.h"
#include <stdlib.h>

char* lorem = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";

GRV_TEST_BEGIN(grv_str_new)
  grv_str s = grv_str_new("TEST");
  GRV_TEST_ASSERT_EQUAL(grv_str_get_start(&s), 0);
  GRV_TEST_ASSERT_EQUAL(grv_str_get_end(&s), 4);
  GRV_TEST_ASSERT_EQUAL(grv_str_capacity(&s), GRV_STR_SSO_CAPACITY);
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s), 4);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&s), true);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&s), "TEST");
  
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
  GRV_TEST_ASSERT_EQUAL(grv_str_capacity(&s2), s2_capacity);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&s2), false);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&s2), lorem);
GRV_TEST_END()


GRV_TEST_BEGIN(grv_str_cat)
  grv_str a = grv_str_new("Hello");
  grv_str b = grv_str_new("World");
  grv_str r = grv_str_cat(&a, &b);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&r), "HelloWorld");
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&r), (int)strlen(grv_str_cstr(&r)));
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&r), true);

  grv_str longstr = grv_str_new(lorem); 
  grv_str r2 = grv_str_cat(&a, &longstr);
  char* r2_cstr = malloc(4096);
  grv_strcpy(r2_cstr, 4096, grv_str_cstr(&a));
  grv_strcat(r2_cstr, 4096, lorem);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&r2), false);
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&r2), strlen(r2_cstr));
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&r2), r2_cstr);

  grv_strcpy(r2_cstr, 4096, lorem);
  grv_strcat(r2_cstr, 4096, grv_str_cstr(&b));
  r2 = grv_str_cat(&longstr, &b);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&r2), false);
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&r2), strlen(r2_cstr));
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&r2), r2_cstr);

  grv_strcpy(r2_cstr, 4096, lorem);
  grv_strcat(r2_cstr, 4096, lorem);
  r2 = grv_str_cat(&longstr, &longstr);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&r2), false);
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&r2), strlen(r2_cstr));
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&r2), r2_cstr);
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_cat_cstr_cstr)
  grv_str r = grv_str_cat_cstr_cstr("Hello", "World");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&r), "HelloWorld");
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&r), strlen("HelloWorld"));
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&r), true);
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_resize)
  grv_str s1 = grv_str_new("Hello");
  GRV_TEST_ASSERT_EQUAL(grv_str_capacity(&s1), GRV_STR_SSO_CAPACITY);
  grv_str_resize(&s1, 10);
  GRV_TEST_ASSERT_EQUAL(grv_str_capacity(&s1), GRV_STR_SSO_CAPACITY);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&s1), true);
  grv_str_resize(&s1, 32);
  GRV_TEST_ASSERT_EQUAL(grv_str_capacity(&s1), GRV_STR_ALLOC_GRANULARITY);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&s1), false);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&s1), "Hello");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_append)
  grv_str s1 = grv_str_new("Hello");
  grv_str s2 = grv_str_new("World");
  grv_str_append(&s1, &s2);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&s1), "HelloWorld");
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s1), (int)strlen(grv_str_cstr(&s1)));
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&s1), true);

  char* r2_cstr = malloc(4096);
  grv_strcpy(r2_cstr, 4096, grv_str_cstr(&s1));
  grv_strcat(r2_cstr, 4096, lorem);
  grv_str longstr = grv_str_new(lorem); 
  grv_str_append(&s1, &longstr);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&s1), false);
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s1), strlen(r2_cstr));
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&s1), r2_cstr);
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_copy_cstr)
  grv_str s1 = grv_str_new("Hello World");
  char* cstr1 = grv_str_copy_cstr(&s1);
  GRV_TEST_ASSERT_EQUAL_STR(cstr1, "Hello World");

  grv_str s2 = grv_str_new(lorem);
  char* cstr2 = grv_str_copy_cstr(&s2);
  GRV_TEST_ASSERT_EQUAL_STR(cstr2, lorem);

  s2.start = 6;
  s2.end = 11;
  cstr2 = grv_str_copy_cstr(&s2);
  GRV_TEST_ASSERT_EQUAL_STR(cstr2, "ipsum");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_append_cstr)
  grv_str s1 = grv_str_new("Hello");
  grv_str_append_cstr(&s1, " ");
  grv_str_append_cstr(&s1, "World");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&s1), "Hello World");
  grv_str_append_cstr(&s1, "0123456789");
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s1), 21);
  grv_str_append_cstr(&s1, "012345678");
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s1), 30);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&s1), true);
  grv_str_append_cstr(&s1, "9");
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s1), 31);
  GRV_TEST_ASSERT_EQUAL(grv_str_is_short(&s1), false);
  GRV_TEST_ASSERT_EQUAL(grv_str_capacity(&s1), GRV_STR_ALLOC_GRANULARITY);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&s1), "Hello World01234567890123456789");
  grv_str_append_cstr(&s1, lorem);
  GRV_TEST_ASSERT_EQUAL(grv_str_capacity(&s1), (strlen(lorem) + 31 + (GRV_STR_ALLOC_GRANULARITY - 1)) / GRV_STR_ALLOC_GRANULARITY * GRV_STR_ALLOC_GRANULARITY );
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_append_char)

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
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&s), "Hello");
  grv_str_lstrip(&s);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&s), "Hello");
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s), strlen("Hello"));
  s = grv_str_new("    ");
  grv_str_lstrip(&s);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&s), "");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_rstrip)
  grv_str s = grv_str_new("Hello \t\n");
  grv_str_rstrip(&s);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&s), "Hello");
  grv_str_rstrip(&s);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&s), "Hello");
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s), strlen("Hello"));
  s = grv_str_new("    ");
  grv_str_rstrip(&s);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&s), "");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_join)
  grv_str a = grv_str_new("Hello");
  grv_str b = grv_str_new("World");
  grv_str c = grv_str_join(&a, &b, " ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&c), "Hello World");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_copy_substr)
  grv_str s = grv_str_new("Hello World");
  grv_str t = grv_str_copy_substr(&s, 0, 5);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "Hello");
  t = grv_str_copy_substr(&s, 6, 11);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "World");
  t = grv_str_copy_substr(&s, 0, 11);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "Hello World");
  t = grv_str_copy_substr(&s, 0, 0);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "");
  t = grv_str_copy_substr(&s, 0, 1);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "H");
  t = grv_str_copy_substr(&s, 0, 2);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "He");
  t = grv_str_copy_substr(&s, 0, 3);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "Hel");
  t = grv_str_copy_substr(&s, 0, 4);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "Hell");
  t = grv_str_copy_substr(&s, 0, 5);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "Hello");
  t = grv_str_copy_substr(&s, 0, 6);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "Hello ");
  t = grv_str_copy_substr(&s, 0, 7);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "Hello W");
  t = grv_str_copy_substr(&s, 0, 8);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "Hello Wo");
  t = grv_str_copy_substr(&s, 0, 9);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "Hello Wor");
  t = grv_str_copy_substr(&s, 0, 10);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "Hello Worl");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_lchop)
  grv_str s = grv_str_new("Hello World");
  grv_str_lchop(&s, 5);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&s), " World");
  grv_str_lchop(&s, 1);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&s), "World");
  grv_str_lchop(&s, 5);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&s), "");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_rchop)
  grv_str s = grv_str_new("Hello World");
  grv_str_rchop(&s, 5);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&s), "Hello ");
  grv_str_rchop(&s, 1);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&s), "Hello");
  grv_str_rchop(&s, 5);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&s), "");
GRV_TEST_END()


GRV_TEST_BEGIN(grv_str_copy)
  grv_str a = grv_str_new("Hello");
  grv_str b = grv_str_copy(&a);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&a), grv_str_cstr(&b));
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_center)
  grv_str s = grv_str_new("Hello World");
  grv_str t = grv_str_copy(&s);
  grv_str_center(&t, 20, ' ');
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "    Hello World     ");
  t = grv_str_copy(&s);
  grv_str_center(&t, 21, ' ');
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_cstr(&t), "     Hello World     ");
GRV_TEST_END()


GRV_TEST_BEGIN(grv_str_split_head_from_front)
  grv_str s = grv_str_new("Hello World");
  grv_str t = grv_str_split_head_from_front(&s, " ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&t), "Hello");
  s = grv_str_new("Hello / to / my / World");
  t = grv_str_split_head_from_front(&s, " / ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&t), "Hello");
GRV_TEST_END()


GRV_TEST_BEGIN(grv_str_split_head_from_back) 
  grv_str s = grv_str_new("Hello World");
  grv_str t = grv_str_split_head_from_back(&s, " ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&t), "Hello");
  s = grv_str_new("Hello To My World");
  t = grv_str_split_head_from_back(&s, " ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&t), "Hello To My");
  s = grv_str_new("Hello / to / my / World");
  t = grv_str_split_head_from_back(&s, " / ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&t), "Hello / to / my");
  s = grv_str_new("Hello / to / my / World / ");
  t = grv_str_split_head_from_back(&s, " / ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&t), "Hello / to / my / World");
GRV_TEST_END()


GRV_TEST_BEGIN(grv_str_split_tail_from_back)
  grv_str s = grv_str_new("Hello World");
  grv_str t = grv_str_split_tail_from_back(&s, " ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&t), "World");
  s = grv_str_new("Hello / to / my / World");
  t = grv_str_split_tail_from_back(&s, " / ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&t), "World");
  s = grv_str_new(" / Hello");
  t = grv_str_split_tail_from_back(&s, " / ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&t), "Hello");
  s = grv_str_new("Hello / ");
  t = grv_str_split_tail_from_back(&s, " / ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&t), "");
  s = grv_str_new("Hello /  ");
  t = grv_str_split_tail_from_back(&s, " / ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&t), " ");
  s = grv_str_new("Volume: front-left: 23253 /  35% / -27.00 dB,   front-right: 23253 /  35% / -27.00 dB");
  t = grv_str_split_tail_from_back(&s, " / ");
  GRV_TEST_ASSERT_EQUAL(grv_str_len(&s), 85);
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&t), "-27.00 dB");
GRV_TEST_END()  

GRV_TEST_BEGIN(grv_str_prepend_cstr)
  grv_str s = grv_str_new("World");
  grv_str_prepend_cstr(&s, "Hello ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&s), "Hello World");
  grv_str_prepend_cstr(&s, "Hello ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&s), "Hello Hello World");
  grv_str_prepend_cstr(&s, "Hello Hello Hello ");
  GRV_TEST_ASSERT_EQUAL_STR(grv_str_copy_cstr(&s), "Hello Hello Hello Hello Hello World");
  GRV_TEST_ASSERT_FALSE(grv_str_is_short(&s));
GRV_TEST_END()


int main(void) {
  GRV_TEST_INIT(grv_str);
  GRV_TEST_RUN(grv_str_new);
  GRV_TEST_RUN(grv_str_cat);
  GRV_TEST_RUN(grv_str_cat_cstr_cstr);
  GRV_TEST_RUN(grv_str_resize);
  GRV_TEST_RUN(grv_str_append)
  GRV_TEST_RUN(grv_str_copy_cstr);
  GRV_TEST_RUN(grv_str_append_cstr);
  GRV_TEST_RUN(grv_str_append_char);
  GRV_TEST_RUN(grv_str_ends_with);
  GRV_TEST_RUN(grv_str_starts_with);
  GRV_TEST_RUN(grv_str_lstrip);
  GRV_TEST_RUN(grv_str_rstrip);
  GRV_TEST_RUN(grv_str_join);
  GRV_TEST_RUN(grv_str_copy_substr);
  GRV_TEST_RUN(grv_str_lchop);
  GRV_TEST_RUN(grv_str_rchop);
  GRV_TEST_RUN(grv_str_copy);
  GRV_TEST_RUN(grv_str_center);
  GRV_TEST_RUN(grv_str_split_head_from_front);
  GRV_TEST_RUN(grv_str_split_head_from_back);
  GRV_TEST_RUN(grv_str_split_tail_from_back);
  GRV_TEST_RUN(grv_str_prepend_cstr);
}
