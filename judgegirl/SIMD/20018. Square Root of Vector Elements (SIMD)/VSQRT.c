#include "VSQRT.h"

#include <math.h>
#include <x86intrin.h>

void sqrt2(float* begin, float* end) {
    int i, j, N = end - begin;
    __m256 vec256;
    for (i = 0; i < N; i+=8) {
        vec256 = _mm256_loadu_ps(begin+i);
        vec256 = _mm256_sqrt_ps(vec256);
        float* res = (float*)&vec256;
        for (j = 0; j < 8 && i+j != N; j++)
            *(begin+i+j) = res[j];
    }
}

