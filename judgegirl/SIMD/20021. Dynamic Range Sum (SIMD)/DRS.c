#include "DRS.h"

#include <immintrin.h>

int32_t search_range(Rect rect, int32_t x[], int32_t y[], int32_t w[], int32_t n) {
    int32_t ret = 0, *res_int;
    __m256i vec_x, vec_y, res;
    __m256i lx = _mm256_set1_epi32(rect.lx),
            rx = _mm256_set1_epi32(rect.rx),
            ly = _mm256_set1_epi32(rect.ly),
            ry = _mm256_set1_epi32(rect.ry);

    for (int i = 0; i < n; i+=8) {
        vec_x = _mm256_loadu_si256((__m256i const *)(x+i));
        vec_y = _mm256_loadu_si256((__m256i const *)(y+i));

        res = _mm256_and_si256(_mm256_and_si256(_mm256_or_si256(_mm256_cmpgt_epi32(vec_x, lx),
                                                                _mm256_cmpeq_epi32(vec_x, lx)),
                                                _mm256_or_si256(_mm256_cmpgt_epi32(rx, vec_x),
                                                                _mm256_cmpeq_epi32(rx, vec_x))),
                               _mm256_and_si256(_mm256_or_si256(_mm256_cmpgt_epi32(vec_y, ly),
                                                                _mm256_cmpeq_epi32(vec_y, ly)),
                                                _mm256_or_si256(_mm256_cmpgt_epi32(ry, vec_y),
                                                                _mm256_cmpeq_epi32(ry, vec_y))));

        res_int = (int32_t*)&res;
        for (int j = 0; j < 8 && (i+j) < n; j++) {
            if (res_int[j]) {
                ret += w[i+j];
            }
        }
    }
    return ret;
}
