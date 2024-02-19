#include "grv/grv_str.h"
#include "grv/grv_strarr.h"
#include "grv/grv_log.h"
#include "grv/grv_util.h"
#include "grv/grv_fs.h"
#include <stdlib.h>

#define CC "gcc"

int main(int argc, char** argv) {
    grv_strarr_t args = grv_strarr_new_from_cstrarr(argv+1, argc-1);
    grv_str_t cmd = grv_str_new(CC);

    bool dry_run = false;

    if (grv_strarr_contains_cstr(args, "-n")) {
        grv_strarr_remove_str(&args, grv_str_ref("-n"));
        dry_run = true;
    }

    if (grv_strarr_contains_cstr(args, "--dry-run")) {
        grv_strarr_remove_str(&args, grv_str_ref("--dry-run"));
        dry_run = true;
    }

    if (!grv_strarr_contains_cstr(args, "-o")) {
        // the last argument is assumed to be the program to be compiled
        grv_str_t* src_file = grv_strarr_back(args);
        if (src_file == 0 || !grv_str_ends_with_cstr(*src_file, ".c")) {
            grv_log_error("Usage: grvgcc [OPTIONS] <src_file>");
            exit(1);
        }
        grv_str_t dst_file = grv_str_substr(*src_file, 0, -3);
        dst_file = grv_fs_basename(dst_file);
        grv_str_append_cstr(&cmd, " -o");
        grv_str_append_str(&cmd, dst_file);
        grv_str_append_space(&cmd);
    }

    grv_str_append_cstr(&cmd, " -I. -Igrv/include -Iinclude -Wall -Wextra -pedantic ");
    grv_str_append_str(&cmd, grv_strarr_join(args, grv_str_ref(" ")));
    grv_str_append_cstr(&cmd, " -lgrv -lm");
    grv_log_info(grv_str_cstr(cmd));
    if (!dry_run) {
        grv_system(cmd);
    }
}
