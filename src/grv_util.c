#include "grv_util.h"
#include <stdio.h>
#include <glob.h>

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


grv_strarr_t grv_system(grv_str_t* cmd) {
  char* cmd_cstr = grv_str_copy_cstr(cmd);
  grv_strarr_t result = grv_system_cstr(cmd_cstr); 
  free(cmd_cstr);
  return result;
}

grv_strarr_t grv_system_cstr(char* cmd) {  
  grv_strarr_t result = grv_strarr_new();
  FILE* fp = popen(cmd, "r");
  if (fp == NULL) {
    return result;
  }

  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, fp)) != -1) {
    grv_str_t line_str = grv_str_new(line);
    grv_str_remove_trailing_newline(&line_str); 
    grv_strarr_push(&result, &line_str);
  }

  pclose(fp);
  if (line) {
    free(line);
  }

  return result;
}

grv_strarr_t grv_util_glob(grv_str_t* pattern) {
  // iterate over contents of directory
  grv_strarr_t result = grv_strarr_new();
  char* pattern_cstr = grv_str_copy_cstr(pattern);
  glob_t glob_result;
  glob(pattern_cstr, GLOB_TILDE, NULL, &glob_result);
  if (glob_result.gl_pathc == 0) {
    globfree(&glob_result);
    return result;
  }

  for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
    grv_str_t path = grv_str_new(glob_result.gl_pathv[i]);
    grv_strarr_push(&result, &path);
  }
  globfree(&glob_result);
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
size_t grv_util_calc_alloc_size(size_t size) {
  size_t mask = sizeof(void*) - 1;
  size_t result = (size + mask) & ~mask;
  #ifdef GRV_DEBUG_MEMORY
    result += sizeof(void*);
  #endif
  return result;
}
void* grv_alloc(u64 size) {
  size_t alloc_size = grv_util_calc_alloc_size(size);
  void* result = malloc(alloc_size);
  if (result == NULL) return result;
  #ifdef GRV_ZERO_MEMORY
    memset(result, 0, alloc_size);
  #elif defined(GRV_DEBUG_MEMORY)
    memset(result, 0xef, alloc_size);
    *(size_t*)result = alloc_size;
    result += sizeof(void*);
  #endif
  return result;
}
void* grv_realloc(void* ptr, u64 size) {
  size_t alloc_size = grv_util_calc_alloc_size(size);
  #ifdef GRV_DEBUG_MEMORY
    ptr -= sizeof(void*);
    size_t old_alloc_size = *((size_t*)ptr);
    if (old_alloc_size > alloc_size) {
      memset(ptr + alloc_size, 0xcd, old_alloc_size - alloc_size);
    }
  #endif
  void* result = realloc(ptr, alloc_size);
  if (result == NULL) return result;
  #ifdef GRV_ZERO_MEMORY
    memset(result, 0, alloc_size);
  #elif defined(GRV_DEBUG_MEMORY)
    if (old_alloc_size < alloc_size) {
      memset(result + old_alloc_size, 0xef, alloc_size - old_alloc_size);
    }
    *(size_t*)result = alloc_size;
    result += sizeof(void*);
  #endif
  return result;
}
void grv_free(void* ptr) {
  #ifdef GRV_DEBUG_MEMORY
    size_t alloc_size = *((size_t*)ptr - 1);
    memset(ptr - sizeof(size_t), 0xcd, alloc_size); 
    ptr -= sizeof(size_t);
  #endif
  free(ptr); 
}

void grv_free_prepare(void* ptr) {
  #ifdef GRV_DEBUG_MEMORY
    size_t alloc_size = *((size_t*)ptr - 1);
    memset(ptr - sizeof(size_t), 0xcd, alloc_size); 
  #endif
}