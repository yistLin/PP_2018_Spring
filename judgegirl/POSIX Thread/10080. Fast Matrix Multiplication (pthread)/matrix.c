#include <stdlib.h>
#include <pthread.h>

#include "matrix.h"

/* Original
 * 3.in  1.84s user 0.01s system 99% cpu 1.865 total
 * 4.in  9.89s user 0.05s system 99% cpu 9.963 total
 */

#define MAX_THREAD 4

typedef struct {
    int N, l, r;
} Argu;

Argu args[MAX_THREAD];

unsigned long *a_d1_ptr[2048], *b_d1_ptr[2048], *c_d1_ptr[2048];

static inline int min(x, y) {
    return (x < y) ? x : y;
}

void *subtask(void *void_ptr) {
    Argu* arg = (Argu*)void_ptr;
    int N = arg->N, l = arg->l, r = arg->r;

    for (int i = l; i < r; i++) {
        for (int j = 0; j < N; j++) {
            unsigned long sum = 0;
            for (int k = 0; k < N; k++)
                sum += *(a_d1_ptr[i]+k) * *(b_d1_ptr[k]+j);
            *(c_d1_ptr[i]+j) = sum;
        }
    }
}

void multiply(int N, unsigned long A[][2048], unsigned long B[][2048], unsigned long C[][2048]) {
    pthread_t tid[MAX_THREAD];
    Argu *arg_ptr;
    for (int i = 0; i < N; i++) {
        a_d1_ptr[i] = &A[i][0];
        b_d1_ptr[i] = &B[i][0];
        c_d1_ptr[i] = &C[i][0];
    }
    int blk = (N + MAX_THREAD - 1) / MAX_THREAD;

    for (int i = 0; i < MAX_THREAD; i++) {
        arg_ptr = &args[i];
        arg_ptr->l = i * blk;
        arg_ptr->r = min((i+1) * blk, N);
        arg_ptr->N = N;
        if (pthread_create(&tid[i], NULL, subtask, arg_ptr)) {
            exit(1);
        }
    }
    for (int j = 0; j < MAX_THREAD; j++)
        pthread_join(tid[j], NULL);
}
