#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include "utils.h"
 
#define MAXN 10000005
#define MAX_THREAD 2

// my define
typedef struct {
    int n, tid;
    uint32_t key;
} SeriesArg;
SeriesArg arg[MAX_THREAD];
uint32_t *enc_res[MAX_THREAD];

void *encrypt_series(void *void_ptr) {
    SeriesArg* arg = (SeriesArg*)void_ptr;
    int n = arg->n, tid = arg->tid;
    uint32_t key = arg->key, sum = 0;
    for (int i = 1; i <= n; i++) {
        sum += encrypt(i, key);
        enc_res[tid][i] = sum;
    }
    return NULL;
}

int main() {
    int n;
    uint32_t key;
    pthread_t tid[MAX_THREAD];
    int opened_thread = 0;
    for (int i = 0; i < MAX_THREAD; i++) {
        enc_res[i] = (uint32_t*)malloc(sizeof(uint32_t) * MAXN);
    }

    while (scanf("%d %" PRIu32, &n, &key) == 2) {
        arg[opened_thread].n = n;
        arg[opened_thread].key = key;
        arg[opened_thread].tid = opened_thread;
        if (pthread_create(&tid[opened_thread], NULL, encrypt_series, &arg[opened_thread])) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
        opened_thread++;

        if (opened_thread == MAX_THREAD) {
            for (int i = 0; i < MAX_THREAD; i++) {
                pthread_join(tid[i], NULL);
                output(enc_res[i], arg[i].n);
            }
            opened_thread = 0;
        }
    }
    if (opened_thread != 0) {
        for (int i = 0; i < opened_thread; i++) {
            pthread_join(tid[i], NULL);
            output(enc_res[i], arg[i].n);
        }
    }

    for (int i = 0; i < MAX_THREAD; i++) {
        free(enc_res[i]);
    }
    return 0;
}
