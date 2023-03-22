#include "grv_util.h"
#include <stdio.h>
#include <glob.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
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


grv_strarr grv_system(grv_str* cmd) {
  char* cmd_cstr = grv_str_copy_cstr(cmd);
  grv_strarr result = grv_system_cstr(cmd_cstr); 
  free(cmd_cstr);
  return result;
}

grv_strarr grv_system_cstr(char* cmd) {  
  grv_strarr result = grv_strarr_new();
  FILE* fp = popen(cmd, "r");
  if (fp == NULL) {
    return result;
  }

  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, fp)) != -1) {
    grv_str line_str = grv_str_new(line);
    grv_str_remove_trailing_newline(&line_str); 
    grv_strarr_push(&result, &line_str);
  }

  pclose(fp);
  if (line) {
    free(line);
  }

  return result;
}

grv_strarr grv_util_glob(grv_str* pattern) {
  // iterate over contents of directory
  grv_strarr result = grv_strarr_new();
  char* pattern_cstr = grv_str_copy_cstr(pattern);
  glob_t glob_result;
  glob(pattern_cstr, GLOB_TILDE, NULL, &glob_result);
  if (glob_result.gl_pathc == 0) {
    globfree(&glob_result);
    return result;
  }

  for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
    grv_str path = grv_str_new(glob_result.gl_pathv[i]);
    grv_strarr_push(&result, &path);
  }
  globfree(&glob_result);
  return result;
}