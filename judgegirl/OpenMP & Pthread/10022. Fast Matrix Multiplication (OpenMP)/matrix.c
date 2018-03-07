#include <stdlib.h>

#include "matrix.h"

/* Original
 * 3.in  1.84s user 0.01s system 99% cpu 1.865 total
 * 4.in  9.89s user 0.05s system 99% cpu 9.963 total
 */

void multiply(int N, unsigned long A[][2048], unsigned long B[][2048], unsigned long C[][2048]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            unsigned long sum = 0;
            for (int k = 0; k < N; k++)
                sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    }
}
