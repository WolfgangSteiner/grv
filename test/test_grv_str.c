#include "grv/grv_str.h"
#include "grv/grv_test.h"
#include "grv/grv_cstr.h"
#include <stdlib.h>

char* lorem = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";

GRV_TEST_BEGIN(grv_str_new)
  grv_str_t s = grv_str_ref("TEST");
  GRV_TEST_EQUAL_SIZE(s.size, 4);
  GRV_TEST_FALSE(s.owns_data);
  GRV_TEST_TRUE(s.is_valid);
  GRV_TEST_EQUAL_STR(s, grv_str_ref("TEST"));
  
  grv_str_t s2 = str_new(lorem);
  size_t lorem_length = strlen(lorem);
  GRV_TEST_EQUAL_SIZE(s2.size, lorem_length);
  GRV_TEST_TRUE(s2.owns_data);
  GRV_TEST_TRUE(s2.is_valid);
  GRV_TEST_EQUAL_STR(s2, grv_str_ref(lorem));
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_free)
  grv_str_t s = grv_str_new(lorem);
  grv_str_free(&s);
  GRV_TEST_FALSE(s.is_valid);
  GRV_TEST_FALSE(s.owns_data);
  GRV_TEST_EQUAL_SIZE(s.size, 0);
  GRV_TEST_EQUAL_PTR(s.data, NULL);
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_cat)
  grv_str_t a = grv_str_ref("Hello");
  grv_str_t b = grv_str_ref("World");
  grv_str_t r = grv_str_cat(a, b);
  GRV_TEST_EQUAL_STR(r, grv_str_ref("HelloWorld"));
  GRV_TEST_EQUAL_SIZE(r.size, a.size + b.size);
  GRV_TEST_TRUE(r.is_valid);
  GRV_TEST_TRUE(r.owns_data);

  grv_str_t longstr = grv_str_ref(lorem); 
  grv_str_t r2 = grv_str_cat(a, longstr);
  char* r2_cstr = grv_cstr_cat(grv_str_cstr(a), lorem);
  GRV_TEST_EQUAL_SIZE(r2.size, strlen(r2_cstr));
  GRV_TEST_EQUAL_STR(r2, grv_str_ref(r2_cstr));
  GRV_TEST_TRUE(r2.owns_data);
  GRV_TEST_TRUE(r2.is_valid);
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_append)
  grv_str_t s1 = grv_str_ref("Hello");
  grv_str_t s2 = grv_str_ref("World");
  grv_str_append(&s1, s2);
  GRV_TEST_EQUAL_STR(s1, grv_str_ref("HelloWorld"));
  GRV_TEST_EQUAL_SIZE(s1.size, 10);
  GRV_TEST_TRUE(s1.is_valid);
  GRV_TEST_TRUE(s1.owns_data);

  char* r2_cstr = grv_cstr_cat(lorem, "HelloWorld");
  grv_str_t longstr = grv_str_new(lorem); 
  grv_str_append(&longstr, s1);
  GRV_TEST_EQUAL_SIZE(grv_str_len(longstr), strlen(r2_cstr));
  GRV_TEST_EQUAL_STR(longstr, grv_str_ref(r2_cstr));
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_append_char)
  grv_str_t s = grv_str_new("");
  grv_str_append_char(&s, 'H');
  GRV_TEST_EQUAL_STR(s, grv_str_ref("H"));
  grv_str_append_char(&s, 'e');
  GRV_TEST_EQUAL_STR(s, grv_str_ref("He"));
  grv_str_append_char(&s, 'l');
  GRV_TEST_EQUAL_STR(s, grv_str_ref("Hel"));
  grv_str_append_char(&s, 'l');
  GRV_TEST_EQUAL_STR(s, grv_str_ref("Hell"));
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_prepend)
  grv_str_t s = grv_str_new("World");
  grv_str_prepend(&s, grv_str_ref("Hello "));
  GRV_TEST_EQUAL_STR(s, grv_str_ref("Hello World"));
  grv_str_prepend(&s, grv_str_ref("Hello "));
  GRV_TEST_EQUAL_STR(s, grv_str_ref("Hello Hello World"));
  grv_str_prepend(&s, grv_str_ref("Hello Hello Hello "));
  GRV_TEST_EQUAL_STR(s, grv_str_ref("Hello Hello Hello Hello Hello World"));
  grv_str_free(&s);
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_prepend_char)
  grv_str_t s = grv_str_new("World");
  grv_str_prepend_char(&s, 'A');
  GRV_TEST_EQUAL_STR(s, grv_str_ref("AWorld"));
GRV_TEST_END()

#if 0
GRV_TEST_BEGIN(grv_str_copy_cstr)
  grv_str_t s1 = grv_str_new("Hello World");
  char* cstr1 = grv_str_copy_cstr(&s1);
  GRV_TEST_EQUAL_STR(cstr1, "Hello World");

  grv_str_t s2 = grv_str_new(lorem);
  char* cstr2 = grv_str_copy_cstr(&s2);
  GRV_TEST_EQUAL_STR(cstr2, lorem);

  s2.start = 6;
  s2.end = 11;
  cstr2 = grv_str_copy_cstr(&s2);
  GRV_TEST_EQUAL_STR(cstr2, "ipsum");
GRV_TEST_END()



GRV_TEST_BEGIN(grv_str_ends_with) 
  grv_str_t s = grv_str_new("Hello World");
  GRV_TEST_EQUAL(grv_str_ends_with_cstr(&s, "World"), true);
  GRV_TEST_EQUAL(grv_str_ends_with_cstr(&s, "Worl"), false);
  s = grv_str_new(lorem);
  GRV_TEST_EQUAL(grv_str_ends_with_cstr(&s, "amet."), true);
  GRV_TEST_EQUAL(grv_str_ends_with_cstr(&s, "amet"), false);
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_starts_with) 
  grv_str_t s = grv_str_new("Hello World");
  GRV_TEST_EQUAL(grv_str_starts_with_cstr(&s, "Hello"), true);
  GRV_TEST_EQUAL(grv_str_starts_with_cstr(&s, "ello"), false);
  s = grv_str_new(lorem);
  GRV_TEST_EQUAL(grv_str_starts_with_cstr(&s, "Lorem"), true);
  GRV_TEST_EQUAL(grv_str_starts_with_cstr(&s, "orem"), false);
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_lstrip)
  grv_str_t s = grv_str_new(" \t\nHello");
  grv_str_lstrip(&s);
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&s), "Hello");
  grv_str_lstrip(&s);
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&s), "Hello");
  GRV_TEST_EQUAL(grv_str_len(&s), strlen("Hello"));
  s = grv_str_new("    ");
  grv_str_lstrip(&s);
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&s), "");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_rstrip)
  grv_str_t s = grv_str_new("Hello \t\n");
  grv_str_rstrip(&s);
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&s), "Hello");
  grv_str_rstrip(&s);
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&s), "Hello");
  GRV_TEST_EQUAL(grv_str_len(&s), strlen("Hello"));
  s = grv_str_new("    ");
  grv_str_rstrip(&s);
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&s), "");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_join)
  grv_str_t a = grv_str_new("Hello");
  grv_str_t b = grv_str_new("World");
  grv_str_t c = grv_str_join(&a, &b, " ");
  GRV_TEST_EQUAL_STR(grv_str_cstr(&c), "Hello World");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_copy_substr)
  grv_str_t s = grv_str_new("Hello World");
  grv_str_t t = grv_str_copy_substr(&s, 0, 5);
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "Hello");
  t = grv_str_copy_substr(&s, 6, 11);
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "World");
  t = grv_str_copy_substr(&s, 0, 11);
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "Hello World");
  t = grv_str_copy_substr(&s, 0, 0);
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "");
  t = grv_str_copy_substr(&s, 0, 1);
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "H");
  t = grv_str_copy_substr(&s, 0, 2);
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "He");
  t = grv_str_copy_substr(&s, 0, 3);
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "Hel");
  t = grv_str_copy_substr(&s, 0, 4);
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "Hell");
  t = grv_str_copy_substr(&s, 0, 5);
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "Hello");
  t = grv_str_copy_substr(&s, 0, 6);
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "Hello ");
  t = grv_str_copy_substr(&s, 0, 7);
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "Hello W");
  t = grv_str_copy_substr(&s, 0, 8);
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "Hello Wo");
  t = grv_str_copy_substr(&s, 0, 9);
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "Hello Wor");
  t = grv_str_copy_substr(&s, 0, 10);
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "Hello Worl");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_lchop)
  grv_str_t s = grv_str_new("Hello World");
  grv_str_lchop(&s, 5);
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&s), " World");
  grv_str_lchop(&s, 1);
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&s), "World");
  grv_str_lchop(&s, 5);
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&s), "");
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_rchop)
  grv_str_t s = grv_str_new("Hello World");
  grv_str_rchop(&s, 5);
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&s), "Hello ");
  grv_str_rchop(&s, 1);
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&s), "Hello");
  grv_str_rchop(&s, 5);
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&s), "");
GRV_TEST_END()


GRV_TEST_BEGIN(grv_str_copy)
  grv_str_t a = grv_str_new("Hello");
  grv_str_t b = grv_str_copy(&a);
  GRV_TEST_EQUAL_STR(grv_str_cstr(&a), grv_str_cstr(&b));
GRV_TEST_END()

GRV_TEST_BEGIN(grv_str_center)
  grv_str_t s = grv_str_new("Hello World");
  grv_str_t t = grv_str_copy(&s);
  grv_str_center(&t, 20, ' ');
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "    Hello World     ");
  t = grv_str_copy(&s);
  grv_str_center(&t, 21, ' ');
  GRV_TEST_EQUAL_STR(grv_str_cstr(&t), "     Hello World     ");
GRV_TEST_END()


GRV_TEST_BEGIN(grv_str_split_head_from_front)
  grv_str_t s = grv_str_new("Hello World");
  grv_str_t t = grv_str_split_head_from_front(&s, " ");
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&t), "Hello");
  s = grv_str_new("Hello / to / my / World");
  t = grv_str_split_head_from_front(&s, " / ");
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&t), "Hello");
GRV_TEST_END()


GRV_TEST_BEGIN(grv_str_split_head_from_back) 
  grv_str_t s = grv_str_new("Hello World");
  grv_str_t t = grv_str_split_head_from_back(&s, " ");
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&t), "Hello");
  s = grv_str_new("Hello To My World");
  t = grv_str_split_head_from_back(&s, " ");
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&t), "Hello To My");
  s = grv_str_new("Hello / to / my / World");
  t = grv_str_split_head_from_back(&s, " / ");
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&t), "Hello / to / my");
  s = grv_str_new("Hello / to / my / World / ");
  t = grv_str_split_head_from_back(&s, " / ");
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&t), "Hello / to / my / World");
GRV_TEST_END()


GRV_TEST_BEGIN(grv_str_split_tail_from_back)
  grv_str_t s = grv_str_new("Hello World");
  grv_str_t t = grv_str_split_tail_from_back(&s, " ");
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&t), "World");
  s = grv_str_new("Hello / to / my / World");
  t = grv_str_split_tail_from_back(&s, " / ");
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&t), "World");
  s = grv_str_new(" / Hello");
  t = grv_str_split_tail_from_back(&s, " / ");
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&t), "Hello");
  s = grv_str_new("Hello / ");
  t = grv_str_split_tail_from_back(&s, " / ");
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&t), "");
  s = grv_str_new("Hello /  ");
  t = grv_str_split_tail_from_back(&s, " / ");
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&t), " ");
  s = grv_str_new("Volume: front-left: 23253 /  35% / -27.00 dB,   front-right: 23253 /  35% / -27.00 dB");
  t = grv_str_split_tail_from_back(&s, " / ");
  GRV_TEST_EQUAL(grv_str_len(&s), 85);
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&t), "-27.00 dB");

  t = grv_str_split_tail_from_back(&s, "xyz");
  GRV_TEST_EQUAL_STR(grv_str_copy_cstr(&t), "");
GRV_TEST_END()  

#endif

int main(void) {
  GRV_TEST_INIT(grv_str_t);
  GRV_TEST_RUN(grv_str_new);
  GRV_TEST_RUN(grv_str_free);
  GRV_TEST_RUN(grv_str_cat);
  GRV_TEST_RUN(grv_str_append)
  GRV_TEST_RUN(grv_str_append_char);
  GRV_TEST_RUN(grv_str_prepend);
  GRV_TEST_RUN(grv_str_prepend_char);
  
  #if 0
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
  #endif
  GRV_TEST_FINISH();
}
