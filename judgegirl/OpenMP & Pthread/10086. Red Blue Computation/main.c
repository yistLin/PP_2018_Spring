#include <stdio.h>

#define MAXN 1000

typedef struct {
    int i, j;
} Elem;

char grids[MAXN][MAXN];
Elem r_stats[MAXN * MAXN], b_stats[MAXN * MAXN];

int main(int argc, char *argv[]) {
    int N, M;
    scanf("%d %d", &N, &M);

    int r_cnt = 0, b_cnt = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            scanf("%c", &grids[i][j]);
            if (grids[i][j] == 'R') {
                r_stats[r_cnt].i = i;
                r_stats[r_cnt].j = j;
                r_cnt++;
            }
            else if (grids[i][j] == 'B') {
                b_stats[b_cnt].i = i;
                b_stats[b_cnt].j = j;
                b_cnt++;
            }
        }

    return 0;
}
