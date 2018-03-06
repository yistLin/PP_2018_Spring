#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <immintrin.h>

static inline __m256i rotate_left(__m256i x, uint32_t n) {
    return _mm256_or_si256(_mm256_slli_epi32(x, n), _mm256_srli_epi32(x, 32-n));
}

static inline __m256i encrypt(__m256i m, uint32_t key) {
    return _mm256_xor_si256(_mm256_add_epi32(rotate_left(m, key & 31), _mm256_set1_epi32(key)), _mm256_set1_epi32(key));
}

static inline uint32_t ori_rotate_left(uint32_t x, uint32_t n) {
    return (x << n) | (x >> (32-n));
}

static inline uint32_t ori_encrypt(uint32_t m, uint32_t key) {
    return (ori_rotate_left(m, key & 31) + key)^key;
}

static uint32_t f(int N, int off, uint32_t key1, uint32_t key2) {
    uint32_t sum = 0;
    int i = 0, j = off;
    __m256i vec_a, vec_b, vec_c = _mm256_setzero_si256();
    for (; i+8 <= N; i+=8, j+=8) {
        vec_a = _mm256_setr_epi32(j, j+1, j+2, j+3, j+4, j+5, j+6, j+7);
        vec_b = _mm256_mullo_epi32(encrypt(vec_a, key1), encrypt(vec_a, key2));
        vec_c = _mm256_add_epi32(vec_c, vec_b);
    }
    uint32_t* res = (uint32_t*)&vec_c;
    for (int k = 0; k < 8; k++)
        sum += res[k];

    if (N % 8 != 0) {
        for (; i < N; i++, j++) {
            sum += ori_encrypt(j, key1) * ori_encrypt(j, key2);
        }
    }
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
