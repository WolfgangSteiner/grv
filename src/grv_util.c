#include "grv/grv_util.h"
#include "grv/grv_memory.h"
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


int grv_util_get_terminal_width() {
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

u64 grv_util_generate_uid() {
  u64 result = 0;
  
  // get the number of microseconds since the epoch
  struct timeval tv;
  gettimeofday(&tv, NULL);
  u64 seconds = tv.tv_sec - GRV_UTIL_EPOCH;
  u64 miliseconds = tv.tv_usec / 1000;
  u64 timestamp = seconds * 1000 + miliseconds;

  u32 random_part;
  grv_util_random_bytes(&random_part, sizeof(random_part));

  result = (timestamp << GRV_UTIL_UID_NUM_RANDOM_BITS) | (random_part & ((1 << GRV_UTIL_UID_NUM_RANDOM_BITS) - 1));
  return result;
} 

void grv_util_random_bytes(void* buffer, size_t buffer_size) {
  getrandom(buffer, buffer_size, 0);
}

bool grv_cmd_available(grv_str_t cmd) {
  grv_str_t check_cmd = grv_str_new("which ");
  grv_str_append_str(&check_cmd, cmd);
  grv_str_append_cstr(&check_cmd, " > /dev/null 2>&1");
  int result = system(grv_str_cstr(check_cmd));
  grv_str_free(&check_cmd);
  return result == 0;
}

struct tm grv_local_time() {
  time_t t = time(NULL);
  return *localtime(&t);
}

grv_str_t grv_local_time_str() {
  struct tm tm = grv_local_time();
  grv_str_t result = grv_str_new_with_format("%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
  return result;
}

f32 grv_local_time_f32() {
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
        grv_str_iter_inc(&iter);
    }
  
    grv_str_free(&content);
    return result;
}

