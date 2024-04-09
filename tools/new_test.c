#include "grv/grv.h"
#include "grv/grv_fs.h"
#include "grv/grv_str.h"

char* template = 
    "#include \"grv/grv_test.h\"\n"
    "#include \"grv/grv.h\"\n"
    "\n"
    "GRV_TEST_BEGIN_NEW(%s)\n"
    "   GRV_TEST_FAIL()\n"
    "GRV_TEST_END_NEW()\n";

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("Usage: new_test <test_name>\n");
    }
    
    char* name = argv[1];
    char* filename = grv_cstr_new_with_format("test/%s.c", name);
    if (grv_file_exists(grv_str_ref(filename))) {
        grv_str_t msg = grv_str_new_with_format("File %s already exists", filename);
        grv_log_error(msg);
        grv_str_free(&msg);
        exit(1);
    }

    grv_str_t path = grv_path_dirname(grv_str_ref(filename));
    grv_make_path(path);

    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        grv_str_t msg = grv_str_new_with_format("Could not open %s for writing.", filename);
        grv_log_error(msg);
        grv_str_free(&msg);
        exit(1);
    }

    grv_str_t test_func_name = grv_path_basename(grv_str_ref(name));

    grv_str_t msg = grv_str_new_with_format("Created new test file: %s", filename);
    grv_log_info(msg);
    grv_str_free(&msg);
    fprintf(fp, template, test_func_name);
    fclose(fp);
    return 0;
}
