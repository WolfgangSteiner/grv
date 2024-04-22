#include "grvbld.h"
#include <stdbool.h>
#include <assert.h>

#define TEST_EQUAL_CSTR(STR, EXPECTED) do { \
    bool equal = grvbld_cstr_eq(STR, EXPECTED);\
    if (!equal) { \
        printf("[ERROR] Failed test at %s:%d: expected \"%s\", got \"%s\".", __FILE__, __LINE__, EXPECTED, STR);\
        assert(false); \
    } \
} while(false)

int main(void) {
    char* a = grvbld_cstr_new("this is a test");
    grvbld_cstr_rstrip(a);
    TEST_EQUAL_CSTR(a, a);
    char* b = grvbld_cstr_new("this is a test\n");
    grvbld_cstr_rstrip(b);
    TEST_EQUAL_CSTR(b, a);

    grvbld_strarr_t* files = grvbld_system("find test -name \\*.c | sort");
    for (int i = 0; i < files->size; ++i) {
        char* test_file_path = files->data[i];
        log_info("%s", test_file_path);
        free(test_file_path);
    }
    return 0;
}
