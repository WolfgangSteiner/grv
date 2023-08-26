#include <stdlib.h>
#include <stdio.h>
#include "grv/util.h"

int main(int argc, char** argv) {
  size_t alloc_size = 1024;
  size_t num_allocs = 1024 * 1024 * 100;
  for (int i = 0; i < num_allocs; i++) {
    //void* ptr = calloc(1, alloc_size);
    void* ptr = grv_alloc(alloc_size);
    grv_free(ptr);
  }
}