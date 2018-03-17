#include <stdio.h>
#include <omp.h>

#define MAXN 20

int N = 0;
char board[MAXN][MAXN];

int solve(int row, int col[MAXN], int diag[2 * MAXN - 1], int rdiag[2 * MAXN - 1]) {
    if (row == N)
        return 1;

    int nb_answers = 0;
    for (int i = 0; i < N; i++) {
        if (board[row][i] == '*')
            continue;
        int j = i - row + N - 1, k = row + i;
        if (col[i] && diag[j] && rdiag[k]) {
            col[i] = diag[j] = rdiag[k] = 0;
            nb_answers += solve(row+1, col, diag, rdiag);
            col[i] = diag[j] = rdiag[k] = 1;
        }
    }

    return nb_answers;
}

int solve_n_queens() {
    int nb_answers = 0;
    int col[MAXN], diag[2 * MAXN - 1], rdiag[2 * MAXN - 1];
    for (int i = 0; i < MAXN; i++)
        col[i] = 1;
    for (int i = 0; i < (2 * MAXN - 1); i++)
        diag[i] = rdiag[i] = 1;

    #pragma omp parallel for firstprivate(col, diag, rdiag)
    for (int i = 0; i < N; i++) {
        if (board[0][i] == '*')
            continue;
        int j = i - 0 + N - 1, k = 0 + i;
        col[i] = diag[j] = rdiag[k] = 0;
        int ret = solve(1, col, diag, rdiag);

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
