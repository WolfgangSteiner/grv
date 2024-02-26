#include "grv/grv.h"

char* template = 
    "#include \"grv/grv_test.h\"\n"
    "#include \"grv/grv.h\"\n"
    "\n"
    "GRV_TEST_BEGIN_NEW(%s)\n"
    "\n"
    "GRV_TEST_END_NEW()\n";

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("Usage: new_test <test_name>\n");
    }
    
    char* name = argv[1];
    char* filename = grv_cstr_new_with_format("test/%s.c", name);
    if (grv_file_exists(grv_str_ref(filename))) {
        grv_log_error("File %s already exists", filename);
        exit(1);
    }

    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        grv_log_error("Could not open %s for writing.", filename);
        exit(1);
    }

    fprintf(fp, template, name);
    fclose(fp);
    return 0;
}
