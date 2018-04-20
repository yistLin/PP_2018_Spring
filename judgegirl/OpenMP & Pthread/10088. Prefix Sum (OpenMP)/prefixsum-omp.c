#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <omp.h>

#include "utils.h"

#define MAXN 10000005
#define MAX_THREAD 4
#define MIN(X, Y) ((X) < (Y)) ? (X) : (Y)

uint32_t prefix_sum[MAXN];

int main() {
    int n;
    uint32_t key;

    while (scanf("%d %" PRIu32, &n, &key) == 2) {
        int blk_size = (n > MAX_THREAD) ? ((n + MAX_THREAD - 1) / MAX_THREAD) : 1;
        int nb_thread = (n > MAX_THREAD) ? MAX_THREAD : n;
        int partials[2][MAX_THREAD];
        int *ptr = &partials[0][0];

        #pragma omp parallel for
        for (int i = 0; i < nb_thread; i++) {
            uint32_t sum = 0;
            int left = blk_size * i + 1,
                right = MIN(blk_size * (i + 1), n);
            for (int j = left; j <= right; j++) {
                sum += encrypt(j, key);
                prefix_sum[j] = sum;
            }
            *(ptr+i) = prefix_sum[right];
        }

        int turns = 0, *next_ptr;
        for (int i = 1; i < nb_thread; i *= 2) {
            ptr = &partials[turns][0];
            next_ptr = &partials[(turns+1)%2][0];
            for (int j = 0; j < i; j++)
                *(next_ptr+j) = *(ptr+j);
            for (int j = i; j < nb_thread; j++) {
                *(next_ptr+j) = *(ptr+j) + *(ptr+j-i);
            }
            turns = (turns + 1) % 2;
        }

        ptr = &partials[turns][0];
        for (int i = 0; i < nb_thread; i++) {
            int right = MIN(blk_size * (i + 1), n);
            prefix_sum[right] = *(ptr+i);
        }

        #pragma omp parallel for
        for (int i = 1; i < nb_thread; i++) {
            int left = blk_size * i + 1,
                right = MIN(blk_size * (i + 1), n);

            int prev_sum = prefix_sum[left-1];
            // printf("prev_sum = %d\n", prev_sum);
            for (int j = left; j < right; j++) {
                prefix_sum[j] = prefix_sum[j] + prev_sum;
            }
        }

        // for (int i = 1; i <= n; i++)
        //     printf(" %" PRIu32, prefix_sum[i]);
        // printf("\n");

        output(prefix_sum, n);
    }

    return 0;
}
