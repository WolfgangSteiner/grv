#include "grv/grv_common.h"
#include "grv/grv_util.h"
#include "grv/grv_base.h"
#include "grv/grv_memory.h"
#include "grv/grv_str.h"
#include <stdio.h>
#include <glob.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/random.h>
#endif


int grv_util_get_terminal_width(void) {
  #ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(h, &csbi);
    columns = csbi.dwSize.X;
    return columns;
  #else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
  #endif
}

int grv_system(grv_str_t cmd) {
    char* cmd_str = grv_str_copy_to_cstr(cmd);
    int result = system(cmd_str);
    grv_free(cmd_str);
    return result;
}

grv_strarr_t grv_system_with_capture(grv_str_t cmd) {
  char* cmd_cstr = grv_str_cstr(cmd);
  grv_strarr_t result = grv_system_with_capture_cstr(cmd_cstr); 
  grv_free(cmd_cstr);
  return result;
}

grv_strarr_t grv_system_with_capture_cstr(char* cmd) {  
  grv_strarr_t result = grv_strarr_new();
  FILE* fp = popen(cmd, "r");
  if (fp == NULL) {
    return result;
  }

  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, fp)) != -1) {
    grv_str_t line_str = grv_str_ref(line);
    line_str = grv_str_remove_trailing_newline(line_str); 
    grv_strarr_push(&result, grv_str_copy(line_str));
    grv_str_free(&line_str);
  }

  pclose(fp);
  if (line) {
    free(line);
  }

  return result;
}

pid_t grv_fork(grv_str_t cmd, grv_strarr_t* args) {
    pid_t pid = fork();
    if (pid == 0) {
        size_t num_args = args ? args->size : 0;
        char** arg_arr = grv_alloc_zeros(sizeof(char*) * (num_args + 2));
        arg_arr[0] = grv_str_copy_to_cstr(cmd);
        for (int i = 0; i < num_args; ++i) {
            arg_arr[i+1] = grv_str_copy_to_cstr(args->arr[i]);
        }
        execvp(arg_arr[0], arg_arr);
    }
    return pid;
}

grv_strarr_t grv_util_glob(grv_str_t pattern) {
  // iterate over contents of directory
  grv_strarr_t result = grv_strarr_new();
  char* pattern_cstr = grv_str_copy_to_cstr(pattern);
  glob_t glob_result;
  glob(pattern_cstr, GLOB_TILDE, NULL, &glob_result);
  if (glob_result.gl_pathc == 0) {
    globfree(&glob_result);
    return result;
  }

  for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
    grv_str_t path = grv_str_new(glob_result.gl_pathv[i]);
    grv_strarr_push(&result, path);
  }
  globfree(&glob_result);
  grv_free(pattern_cstr);
  return result;
}

#define GRV_UTIL_EPOCH 1679604975
#define GRV_UTIL_UID_NUM_RANDOM_BITS 20

u64 grv_util_generate_uid(void) {
    u64 result = 0;
    // get the number of microseconds since the epoch
    struct timeval tv;
    gettimeofday(&tv, NULL);
    u64 seconds = tv.tv_sec - GRV_UTIL_EPOCH;
    u64 miliseconds = tv.tv_usec / 1000;
    u64 timestamp = seconds * 1000 + miliseconds;
    u32 random_part = grv_random_u32();
    result = (timestamp << GRV_UTIL_UID_NUM_RANDOM_BITS) | (random_part & ((1 << GRV_UTIL_UID_NUM_RANDOM_BITS) - 1));
    return result;
} 

void grv_random_bytes(void* buffer, size_t buffer_size) {
    getrandom(buffer, buffer_size, 0);
}

u8 grv_random_u8(void) {
    u8 result = 0;
    getrandom(&result, sizeof(u8), 0);
    return result;
}

u32 grv_random_u32(void) {
    u32 result = 0;
    getrandom(&result, sizeof(u32), 0);
    return result;
}

u64 grv_random_u64(void) {
    u64 res = 0;
    grv_random_bytes(&res, sizeof(res));
    return res;
}

bool grv_cmd_available(grv_str_t cmd) {
  grv_str_t check_cmd = grv_str_new("which ");
  grv_str_append_str(&check_cmd, cmd);
  grv_str_append_cstr(&check_cmd, " > /dev/null 2>&1");
  int result = system(grv_str_cstr(check_cmd));
  grv_str_free(&check_cmd);
  return result == 0;
}

struct tm grv_local_time(void) {
  time_t t = time(NULL);
  return *localtime(&t);
}

grv_str_t grv_local_time_str(void) {
  struct tm tm = grv_local_time();
  grv_str_t result = grv_str_new_with_format("%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
  return result;
}

grv_str_t grv_local_datetime_str() {
  struct tm tm = grv_local_time();
  grv_str_t result = grv_str_new_with_format(
    "%d%02d%02dT%02d%02d%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  return result;
}

f32 grv_local_time_f32(void) {
  struct tm tm = grv_local_time();
  f32 result = (f32)tm.tm_hour + (f32)tm.tm_min / 60.0f + (f32)tm.tm_sec / 3600.0f;
  return result;
}

grv_strarr_t grv_readlines(grv_str_t filename) {
    grv_strarr_t result = grv_strarr_new();
    grv_str_t content = grv_read_file(filename);
    grv_str_iter_t iter = grv_str_iter_begin(&content);
    
    while (!grv_str_iter_is_end(&iter)) {
        grv_str_t line = grv_str_iter_match_up_to_char(&iter, '\n');
        grv_strarr_push_copy(&result, line);
        grv_str_iter_inc(&iter, 1);
    }
  
    grv_str_free(&content);
    return result;
}

char grv_query_user(grv_str_t msg, grv_str_t choices) {
    grv_assert(choices.size);
    char default_choice = 0;
    for (grv_str_size_t i = 0; i < choices.size; ++i) {
        char choice = choices.data[i];
        if (grv_char_is_upper(choice)) {
            grv_assert(default_choice == 0);
            default_choice = grv_char_to_lower(choice);
        }
    }

query:
    grv_str_t msg_str = grv_str_format(grv_str_ref("{str} [{str}]"), msg, choices);
    grv_str_print(msg_str);
    grv_str_free(&msg_str);
    char response[16];
    fgets(response, 16, stdin);
    char choice = response[0];
    if (choice == '\n') {
        if (default_choice) {
            return default_choice;
        } else {
            printf("Invalid choice. ");
            goto query;
        }
    } else if (grv_str_contains_char_insensitive(choices, choice)) {
            return grv_char_to_lower(choice);
    } else {
        printf("Invalid choice. ");
        goto query;
    }
}

