#include <time.h>
#include <stdio.h>
#include <stdint.h>

#define GRV_PROFILE(NAME, CODE, ITERATIONS) do { \
    clock_t start = clock(); \
    for (uint32_t ITER = 0; ITER < ITERATIONS; ++ITER) CODE; \
    clock_t end = clock(); \
    double time_ms = (double) (end - start) / CLOCKS_PER_SEC * 1000.0; \
    printf("%s: %.2f ms (%.2f us / iteration)\n", #NAME, time_ms, time_ms * 1000.0 / ITERATIONS); \
} while(0);
