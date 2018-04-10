#include <stdio.h>
#include <omp.h>

#define MAXN 20

int N = 0;
char board[MAXN][MAXN];

int solve(int row, long long int col, long long int diag, long long int rdiag) {
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
            nb_answers += solve(row+1, col ^ (1L << i), diag ^ (1L << j), rdiag ^ (1L << k));
        }
    }

    return nb_answers;
}

int solve_n_queens() {
    int nb_answers = 0;

    #pragma omp parallel for collapse(3) reduction(+:nb_answers) schedule(dynamic)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                int ret = 0;
                long long int col, diag, rdiag;
                col = diag = rdiag = -1L;

                if (board[0][i] == '*' || board[1][j] == '*' || board[2][k] == '*')
                    continue;

                int a = i - 0 + N - 1, b = 0 + i;
                col ^= (1L << i);
                diag ^= (1L << a);
                rdiag ^= (1L << b);

                int c = j - 1 + N - 1, d = 1 + j;
                if ((col & (1L << j)) &&
                    (diag & (1L << c)) &&
                    (rdiag & (1L << d))) {
                    col ^= (1L << j);
                    diag ^= (1L << c);
                    rdiag ^= (1L << d);

                    int e = k - 2 + N - 1, f = 2 + k;
                    if ((col & (1L << k)) &&
                        (diag & (1L << e)) &&
                        (rdiag & (1L << f))) {
                        ret = solve(3, col ^ (1L << k), diag ^ (1L << e), rdiag ^ (1L << f));
                    }
                }

                nb_answers += ret;
            }
        }
    }

    return nb_answers;
}

int main(int argc, char* argv[]) {
    int case_cnt = 0;

    while (scanf("%d", &N) == 1) {
        case_cnt++;

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                scanf(" %c", &board[i][j]);

        int nb_answers = solve_n_queens();
        printf("Case %d: %d\n", case_cnt, nb_answers);
    }
    return 0;
}
