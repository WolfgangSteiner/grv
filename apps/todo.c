#include "grv/grv.h"
#include "grv/grv_util.h"

bool add_todo(int argc, char** argv) {
    if (argc < 1) {
        printf("Usage: todo add <todo>\n");
        return false;
    }

    u64 uid = grv_util_generate_uid();
    grv_str_t filename = grv_str_new_with_format("%s/.todo/%llu.txt", getenv("HOME"), uid);

    grv_file_exists(filename);


    FILE* todo_file = fopen(todo_path, "a");
    if (!todo_file) {
        return false;
    }

    fwrite(todo, 1, grv_cstr_len(todo), todo_file);
    fclose(todo_file);
    return true;
}


int main(int argc, char** argv) {
    if (argc < 2) {
        print_todos();
        return 0;
    } 

    grv_str_t cmd = grv_str_ref(argv[1]);
    if (grv_str_eq(cmd, "add")) {
        bool success = add_todo(argc - 2, argv + 2);
    }    
}