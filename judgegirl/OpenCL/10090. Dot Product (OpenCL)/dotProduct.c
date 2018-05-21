#include <stdio.h>
#include <assert.h>
#include <omp.h>
#include <inttypes.h>
#include "utils.h"

int main(int argc, char *argv[]) {
    int N;
    uint32_t key1, key2;
    while (scanf("%d %" PRIu32 " %" PRIu32, &N, &key1, &key2) == 3) {
        uint32_t sum = 0;
#pragma omp parallel for schedule(static) reduction(+: sum)
        for (int i = 0; i < N; i++) {
            sum += encrypt(i, key1) * encrypt(i, key2);
        }

        printf("%" PRIu32 "\n", sum);
    }
    return 0;
}

