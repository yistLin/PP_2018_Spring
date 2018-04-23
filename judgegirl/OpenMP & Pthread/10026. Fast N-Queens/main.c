#include <stdio.h>

#define MAXN 20

char board[MAXN][MAXN];

int solve(int N, int row, long long int col, long long int diag, long long int rdiag) {
    if (row == N)
        return 1;

    int nb_answers = 0;
    for (int i = 0; i < N; i++) {
        if (board[row][i] == '*')
            continue;
        int j = i - row + N - 1, k = row + i;
        if ((col & (1L << i)) &&
            (diag & (1L << j)) &&
            (rdiag & (1L << k))) {
            nb_answers += solve(N, row+1, col ^ (1L << i), diag ^ (1L << j), rdiag ^ (1L << k));
        }
    }

    return nb_answers;
}

int main(int argc, char* argv[]) {
    int N, case_cnt = 0;

    while (scanf("%d", &N) == 1) {
        case_cnt++;

        // input
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                scanf(" %c", &board[i][j]);

        int ret = 0, nb_answers = 0;
        long long int col, diag, rdiag;
        col = diag = rdiag = -1L;

        // solve
        #pragma omp parallel for firstprivate(ret, col, diag, rdiag) reduction(+: nb_answers)
        for (int i = 0; i < N; i++) {
            if (board[0][i] == '*')
                continue;
            int a = i - 0 + N - 1, b = 0 + i;
            if ((col & (1L << i)) &&
                (diag & (1L << a)) &&
                (rdiag & (1L << b))) {
                ret = solve(N, 1, col ^ (1L << i), diag ^ (1L << a), rdiag ^ (1L << b));
            }
            nb_answers += ret;
        }

        printf("Case %d: %d\n", case_cnt, nb_answers);
    }
    return 0;
}
