#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <immintrin.h>

static inline __m256i rotate_left(__m256i x, uint32_t n) {
    // return  (x << n) | (x >> (32-n));
    return _mm256_or_si256(_mm256_slli_epi32(x, n), _mm256_srli_epi32(x, 32-n));
}

// static inline __m256i encrypt(__m256i const * vec_a, uint32_t key) {
static inline __m256i encrypt(uint32_t a[], uint32_t key) {
    // return (rotate_left(m, key&31) + key)^key;
    for (int i = 0; i < 8; i++)
        printf("%d\n", a[i]);
    // __m256i m = _mm256_loadu_si256((__m256i const *)arr_a);
    __m256i int_mask = _mm256_setr_epi32(-1, -1, -1, -1, -1, -1, -1, -1);
    __m256i m = _mm256_maskload_epi32(a, int_mask);
    int* res = (int*)&m;
    for (int i = 0; i < 8; i++)
        printf("%d\n", res[i]);

    __m256i exp_key = _mm256_set_epi32(key, key, key, key, key, key, key, key);
    // res = (int*)&exp_key;
    // for (int i = 0; i < 8; i++)
    //     printf("%d\n", res[i]);

    __m256i ret = rotate_left(m, key & 31);
    res = (uint32_t*)&ret;
    for (int i = 0; i < 8; i++)
        printf("%d\n", res[i]);
    exit(1);
    return _mm256_xor_si256(_mm256_add_epi32(rotate_left(m, key & 31), exp_key), exp_key);
}

static uint32_t f(int N, int off, uint32_t key1, uint32_t key2) {
    uint32_t sum = 0;
    uint32_t arr_a[8];
    __m256i vec_a, vec_b;
    int cntr = 0;
    for (int i = 0, j = off; i < N; i++, j++) {
        // sum += encrypt(j, key1) * encrypt(j, key2);
        arr_a[cntr] = j;
        cntr++;
        if (cntr == 8 || i == N - 1) {
            vec_b = _mm256_mullo_epi32(encrypt(arr_a, key1), encrypt(arr_a, key2));
            uint32_t* res = (uint32_t*)&vec_b;
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
