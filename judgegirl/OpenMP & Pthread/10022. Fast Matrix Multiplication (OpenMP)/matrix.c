#include <omp.h>

#include "matrix.h"

static inline void trans(int N, unsigned long A[][2048]) {
    int x;
    for (int i = 0; i < N; i++) {
        for (int j = i+1; j < N; j++) {
            x = A[i][j];
            A[i][j] = A[j][i];
            A[j][i] = x;
        }
    }
}

void multiply(int N, unsigned long A[][2048], unsigned long B[][2048], unsigned long C[][2048]) {
    trans(N, B);
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            unsigned long sum = 0;
            for (int k = 0; k < N; k++)
                sum += A[i][k] * B[j][k];
            C[i][j] = sum;
        }
    }
}
