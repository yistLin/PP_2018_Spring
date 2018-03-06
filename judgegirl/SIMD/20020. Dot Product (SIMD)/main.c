#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <immintrin.h>

// static inline __m256i rotate_left(__m256i x, uint32_t n) {
//     return _mm256_or_si256(_mm256_slli_epi32(x, n), _mm256_srli_epi32(x, 32-n));
// }

// static inline __m256i encrypt(__m256i m, uint32_t key) {
//     return _mm256_xor_si256(_mm256_add_epi32(rotate_left(m, key & 31), _mm256_set1_epi32(key)), _mm256_set1_epi32(key));
// }

static inline uint32_t rotate_left(uint32_t x, uint32_t n) {
    return  (x << n) | (x >> (32-n));
}

static inline uint32_t encrypt(uint32_t m, uint32_t key) {
    return (rotate_left(m, key&31) + key)^key;
}

static uint32_t f(int N, int off, uint32_t key1, uint32_t key2) {
    uint32_t sum = 0;
    uint32_t arr_a[8], arr_b[8];
    __m256i vec_a, vec_b, vec_c = _mm256_setzero_si256();
    int cntr = 0;
    for (int i = 0, j = off; i < N; i++, j++) {
        // sum += encrypt(j, key1) * encrypt(j, key2);
        arr_a[cntr] = encrypt(j, key1);
        arr_b[cntr] = encrypt(j, key2);
        cntr++;
        if (cntr == 8) {
            vec_a = _mm256_loadu_si256((__m256i const *)arr_a);
            vec_b = _mm256_loadu_si256((__m256i const *)arr_b);
            vec_b = _mm256_mullo_epi32(vec_a, vec_b);
            vec_c = _mm256_add_epi32(vec_c, vec_b);
            cntr = 0;
        }
        else if (i == N - 1) {
            __m256i m = _mm256_setzero_si256();
            for (int j = 0; j < cntr; j++)
                m = _mm256_insert_epi32(m, -1, 8-j);
            vec_a = _mm256_maskload_epi32((int const *)arr_a, m);
            vec_b = _mm256_maskload_epi32((int const *)arr_b, m);
            vec_b = _mm256_mullo_epi32(vec_a, vec_b);
            vec_c = _mm256_add_epi32(vec_c, vec_b);
        }
    }
    uint32_t* res = (uint32_t*)&vec_c;
    for (int k = 0; k < 8; k++)
        sum += res[k];
    return sum;
}

int main() {
    int N;
    uint32_t key1, key2;
    while (scanf("%d %" PRIu32 " %" PRIu32, &N, &key1, &key2) == 3) {
        uint32_t sum = f(N, 0, key1, key2);
        printf("%" PRIu32 "\n", sum);
    }
    return 0;
}
