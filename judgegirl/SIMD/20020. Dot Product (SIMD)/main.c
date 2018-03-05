#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <immintrin.h>

static inline uint32_t rotate_left(uint32_t x, uint32_t n) {
    return  (x << n) | (x >> (32-n));
}

static inline uint32_t encrypt(uint32_t m, uint32_t key) {
    return (rotate_left(m, key&31) + key)^key;
}

static uint32_t f(int N, int off, uint32_t key1, uint32_t key2) {
    uint32_t sum = 0;
    uint32_t arr_a[8], arr_b[8];
    __m256i vec_a, vec_b, vec_c;
    int cntr = 0;
    for (int i = 0, j = off; i < N; i++, j++) {
        // sum += encrypt(j, key1) * encrypt(j, key2);
        arr_a[cntr] = encrypt(j, key1);
        arr_b[cntr] = encrypt(j, key2);
        cntr++;
        if (cntr == 8 || i == N - 1) {
            vec_a = _mm256_loadu_si256((__m256i const *)&(*arr_a));
            vec_b = _mm256_loadu_si256((__m256i const *)&(*arr_b));
            vec_c = _mm256_mullo_epi32(vec_a, vec_b);
            uint32_t* res = (uint32_t*)&vec_c;
            for (int k = 0; k < cntr; k++)
                sum += res[k];
            cntr = 0;
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
