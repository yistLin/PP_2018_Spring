#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include "utils.h"
 
#define MAXN 10000005
#define MAX_THREAD 2
uint32_t prefix_sum[MAXN];

// my define
typedef struct {
    int h, l;
    uint32_t key;
} SeriesArg;
SeriesArg arg[MAX_THREAD];
uint32_t enc_res[MAXN];

void *encrypt_series(void *void_ptr) {
    SeriesArg* arg = (SeriesArg*)void_ptr;
    for (int i = arg->l; i < arg->h; i++) {
        enc_res[i] = encrypt(i, arg->key);
    }
    return NULL;
}

int main() {
    int n;
    uint32_t key;
    pthread_t tid[MAX_THREAD];
    while (scanf("%d %" PRIu32, &n, &key) == 2) {
        uint32_t sum = 0;
        for (int i = 0; i < MAX_THREAD; i++) {
            int l_bnd = ((n+1) / MAX_THREAD) * i + 1,
                h_bnd = ((n+1) / MAX_THREAD) * (i+1) + 1;
            if (i == MAX_THREAD - 1)
                h_bnd = n + 1;
            arg[i].h = h_bnd;
            arg[i].l = l_bnd;
            arg[i].key = key;
            if (pthread_create(&tid[i], NULL, encrypt_series, &arg[i])) {
                fprintf(stderr, "Error creating thread\n");
                return 1;
            }
        }
        for (int i = 0; i < MAX_THREAD; i++) {
            pthread_join(tid[i], NULL);
        }
        for (int i = 1; i <= n; i++) {
            sum += enc_res[i];
            prefix_sum[i] = sum;
        }
        output(prefix_sum, n);
    }
    return 0;
}
