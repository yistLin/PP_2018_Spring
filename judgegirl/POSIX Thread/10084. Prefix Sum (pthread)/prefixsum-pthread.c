#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include "utils.h"

#define MAXN 10000005
#define MAX_THREAD 6

#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

int boundaries[MAX_THREAD+1], nb_threads, n;
uint32_t key, prefix_sum[MAXN];

void *sum_partial_prefix(void *arg) {
    int idx = *((int *)arg);
    int left = boundaries[idx], right = boundaries[idx+1];

    prefix_sum[left] = encrypt(left, key);
    for (int i = left+1; i < right; i++)
        prefix_sum[i] = prefix_sum[i-1] + encrypt(i, key);

    pthread_exit(NULL);
}

void *sum_prefix(void *arg) {
    int idx = *((int *)arg);
    int left = boundaries[idx], right = boundaries[idx+1];
    int psum = prefix_sum[left-1];

    for (int i = left; i < right-1; i++)
        prefix_sum[i] += psum;

    pthread_exit(NULL);
}

int main() {
    pthread_t tids[MAX_THREAD];
    int idx[MAX_THREAD];

    while (scanf("%d %" PRIu32, &n, &key) == 2) {
        // determine number of threads
        if (n < MAX_THREAD)
            nb_threads = n;
        else
            nb_threads = MAX_THREAD;

        // slice the data
        int block_size = (n + nb_threads - 1) / nb_threads;
        for (int i = 0; i < nb_threads; i++) {
            boundaries[i] = MIN(i * block_size + 1, n);
        }
        boundaries[nb_threads] = n + 1;

#ifdef DEBUG
        printf("boundaries:");
        for (int i = 0; i < nb_threads+1; i++)
            printf(" %d", boundaries[i]);
        printf("\n");
#endif

        // sum partial prefix
        for (int i = 0; i < nb_threads; i++) {
            idx[i] = i;
            int rc = pthread_create(&tids[i], NULL, sum_partial_prefix, (void *)&idx[i]);
            assert(rc == 0);
        }
        for (int i = 0; i < nb_threads; i++)
            pthread_join(tids[i], NULL);

#ifdef DEBUG
        printf("sum partial prefix:");
        for (int i = 1; i <= n; i++)
            printf(" %" PRIu32, prefix_sum[i]);
        printf("\n");
#endif

        // accumulate partial sum
        for (int i = 1; i < nb_threads; i++) {
            prefix_sum[boundaries[i+1]-1] += prefix_sum[boundaries[i]-1];
        }

#ifdef DEBUG
        printf("accumulate partial sum:");
        for (int i = 1; i <= n; i++)
            printf(" %" PRIu32, prefix_sum[i]);
        printf("\n");
#endif

        // calc prefix sum
        for (int i = 1; i < nb_threads; i++) {
            int rc = pthread_create(&tids[i], NULL, sum_prefix, (void *)&idx[i]);
            assert(rc == 0);
        }
        for (int i = 1; i < nb_threads; i++)
            pthread_join(tids[i], NULL);

#ifdef DEBUG
        printf("sum prefix:");
        for (int i = 1; i <= n; i++)
            printf(" %" PRIu32, prefix_sum[i]);
        printf("\n");
#endif

        output(prefix_sum, n);
    }

    return 0;
}
