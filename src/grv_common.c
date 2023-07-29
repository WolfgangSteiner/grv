#include "grv/common.h"

size_t min_size_t(size_t a, size_t b) {
  return a < b ? a : b;
}

size_t max_size_t(size_t a, size_t b) {
  return a > b ? a : b;
}