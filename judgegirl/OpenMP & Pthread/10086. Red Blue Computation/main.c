#include <stdio.h>
#include <string.h>

#define MAXN 1005
#define CLIP(X, Y) ((X) == (Y) ? 0 : (X)) 

char grids[2][MAXN][MAXN];

int main(int argc, char *argv[]) {
    int N, M;
    scanf("%d %d", &N, &M);

    for (int i = 0; i < N; i++)
        scanf("%s", grids[0][i]);

    for (int i = 0; i < M; i++) {
        #pragma omp parallel
        {
            #pragma omp for
            for (int i = 0; i < N; i++) {
                memcpy((void *)&(grids[1][i][0]), (const void *)&(grids[0][i][0]), N);
            }
            #pragma omp for
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    int jj = CLIP(j+1, N);
                    if (grids[0][i][j] == 'R' && grids[0][i][jj] == 'W') {
                        grids[1][i][j] = 'W';
                        grids[1][i][jj] = 'R';
                    }
                }
            }
            #pragma omp for
            for (int i = 0; i < N; i++) {
                memcpy((void *)&(grids[0][i][0]), (const void *)&(grids[1][i][0]), N);
            }
            #pragma omp for
            for (int i = 0; i < N; i++) {
                int ii = CLIP(i+1, N);
                for (int j = 0; j < N; j++) {
                    if (grids[1][i][j] == 'B' && grids[1][ii][j] == 'W') {
                        grids[0][i][j] = 'W';
                        grids[0][ii][j] = 'B';
                    }
                }
            }
        }
    }

    for (int i = 0; i < N; i++)
        printf("%s\n", grids[0][i]);

    return 0;
}
