#include "grv/grv_test.h"
#include "grv/grv_strmap.h"

GRV_TEST_BEGIN(grv_strmap_insert_find)
    grv_strmap_t map;
    grv_strmap_init(&map);

    grv_strmap_insert(&map, "key1", "value1");
    grv_strmap_insert(&map, "key2", "value2");
    grv_strmap_insert(&map, "key3", "value3");

    grv_strmap_entry_t* entry1 = grv_strmap_find(&map, "key1");
    grv_strmap_entry_t* entry2 = grv_strmap_find(&map, "key2");
    grv_strmap_entry_t* entry3 = grv_strmap_find(&map, "key3");
    grv_strmap_entry_t* entry4 = grv_strmap_find(&map, "key4");

    GRV_TEST_NOT_NULLPTR(entry1);
    GRV_TEST_NOT_NULLPTR(entry2);
    GRV_TEST_NOT_NULLPTR(entry3);
    GRV_TEST_NULLPTR(entry4);

    GRV_TEST_EQUAL_STR(entry1->key, "key1");
    GRV_TEST_EQUAL_STR(entry1->value, "value1");
    GRV_TEST_EQUAL_STR(entry2->key, "key2");
    GRV_TEST_EQUAL_STR(entry2->value, "value2");
    GRV_TEST_EQUAL_STR(entry3->key, "key3");
    GRV_TEST_EQUAL_STR(entry3->value, "value3");
    grv_strmap_free(&map);
GRV_TEST_END()

int main(void) {
    GRV_TEST_INIT(grv_strmap);
    GRV_TEST_RUN(grv_strmap_insert_find);
    GRV_TEST_FINISH();
}
