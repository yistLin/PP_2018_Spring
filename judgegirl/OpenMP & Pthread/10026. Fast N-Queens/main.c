#include <stdio.h>

#define MAXN 20

char board[MAXN][MAXN];

int solve(int N, int row, long long int col, long long int diag, long long int rdiag) {
    if (row == N)
        return 1;

    int nb_answers = 0;
    int j = 0 - row + N - 1, k = row + 0;
    long long int pos = col & (diag >> j) & (rdiag >> k);
    for (int i = 0; i < N; i++) {
        if (board[row][i] == '*')
            continue;
        if (pos & (1L << i))
            nb_answers += solve(N, row+1, col ^ (1L << i), diag ^ (1L << (j+i)), rdiag ^ (1L << (k+i)));
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

        int nb_answers = 0;
        long long int col, diag, rdiag;
        col = diag = rdiag = -1L;

        // solve
        #pragma omp parallel for firstprivate(col, diag, rdiag) reduction(+: nb_answers) collapse(2) schedule(dynamic)
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (board[0][i] == '*' || board[1][j] == '*')
                    continue;

                int ret = 0;
                int a = i - 0 + N - 1, b = 0 + i;
                col ^= (1L << i), diag ^= (1L << a), rdiag ^= (1L << b);

                int c = j - 1 + N - 1, d = 1 + j;
                if ((col & (1L << j)) &&
                    (diag & (1L << c)) &&
                    (rdiag & (1L << d)))
                    ret = solve(N, 2, col ^ (1L << j), diag ^ (1L << c), rdiag ^ (1L << d));

                col ^= (1L << i), diag ^= (1L << a), rdiag ^= (1L << b);

                nb_answers += ret;
            }
        }

        printf("Case %d: %d\n", case_cnt, nb_answers);
    }

    return 0;
}
