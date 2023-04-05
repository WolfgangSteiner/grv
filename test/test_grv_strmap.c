#include "grv_test.h"
#include "grv_strmap.h"

GRV_TEST_BEGIN(grv_strmap_insert_find)
    grv_strmap map;
    grv_strmap_init(&map);

    grv_strmap_insert_cstr(&map, "key1", "value1");
    grv_strmap_insert_cstr(&map, "key2", "value2");
    grv_strmap_insert_cstr(&map, "key3", "value3");

    grv_strmap_entry* entry1 = grv_strmap_find_cstr(&map, "key1");
    grv_strmap_entry* entry2 = grv_strmap_find_cstr(&map, "key2");
    grv_strmap_entry* entry3 = grv_strmap_find_cstr(&map, "key3");

    GRV_TEST_ASSERT_TRUE(entry1 != NULL);
    GRV_TEST_ASSERT_TRUE(entry2 != NULL);
    GRV_TEST_ASSERT_TRUE(entry3 != NULL);

    GRV_TEST_ASSERT_TRUE(grv_str_eq_cstr(&entry1->key, "key1"));
    GRV_TEST_ASSERT_TRUE(grv_str_eq_cstr(&entry1->value, "value1"));
    GRV_TEST_ASSERT_TRUE(grv_str_eq_cstr(&entry2->key, "key2"));
    GRV_TEST_ASSERT_TRUE(grv_str_eq_cstr(&entry2->value, "value2"));
    GRV_TEST_ASSERT_TRUE(grv_str_eq_cstr(&entry3->key, "key3"));
    GRV_TEST_ASSERT_TRUE(grv_str_eq_cstr(&entry3->value, "value3"));
    grv_strmap_free(&map);
GRV_TEST_END()

int main() {
    GRV_TEST_INIT(grv_strmap);
    GRV_TEST_RUN(grv_strmap_insert_find);
}