#include "grv/grv.h"
#include <stdio.h>


int main(int argc, char** argv) {
  for (int i = 0; i < 100; i++) {
    u64 uid;
    grv_util_random_bytes(&uid, sizeof(uid));
    grv_str_t uid_str = grv_str_from_u64(uid);
    printf("%s\n", grv_str_cstr(&uid_str));
  }
}