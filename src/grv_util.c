#include "grv_util.h"
#include <stdio.h>

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