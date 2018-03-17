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
            col ^= (1L << i);
            diag ^= (1L << j);
            rdiag ^= (1L << k);
            nb_answers += solve(row+1, col, diag, rdiag);
            col ^= (1L << i);
            diag ^= (1L << j);
            rdiag ^= (1L << k);
        }
    }

    return nb_answers;
}

int solve_n_queens() {
    int nb_answers = 0;
    long long int col, diag, rdiag;
    col = diag = rdiag = -1L;

    #pragma omp parallel for firstprivate(col, diag, rdiag)
    for (int i = 0; i < N; i++) {
        if (board[0][i] == '*')
            continue;
        int j = i - 0 + N - 1, k = 0 + i;
        col ^= (1L << i);
        diag ^= (1L << j);
        rdiag ^= (1L << k);
        int ret = solve(1, col, diag, rdiag);
        col ^= (1L << i);
        diag ^= (1L << j);
        rdiag ^= (1L << k);

        #pragma omp atomic
        nb_answers += ret;
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
