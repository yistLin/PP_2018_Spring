#include <stdio.h>
#include <omp.h>

#define MAXN 20

int N = 0;
char board[MAXN][MAXN];

omp_lock_t writelock;

int solve(int row, int state[MAXN]) {
    if (row == N)
        return 1;

    int nb_answers = 0;
    int candi[MAXN];
    for (int i = 0; i < N; i++)
        candi[i] = 1;
    for (int r = 0; r < row; r++) {
        candi[state[r]] = 0;
        int d1 = row - (r - state[r]);
        if (d1 >= 0 && d1 < N)
            candi[d1] = 0;
        int d2 = (r + state[r]) - row;
        if (d2 >= 0 && d2 < N)
            candi[d2] = 0;
    }
    for (int i = 0; i < N; i++) {
        if (board[row][i] == '*')
            continue;
        if (candi[i] == 0)
            continue;
        state[row] = i;
        nb_answers += solve(row+1, state);
    }

    return nb_answers;
}

int solve_n_queens() {
    int nb_answers = 0;
    int state[MAXN];

    #pragma omp parallel for private(state)
    for (int i = 0; i < N; i++) {
        if (board[0][i] == '*')
            continue;
        state[0] = i;
        int ret = solve(1, state);

        omp_set_lock(&writelock);
        nb_answers += ret;
        omp_unset_lock(&writelock);
    }
    return nb_answers;
}

int main(int argc, char* argv[]) {
    int case_cnt = 0;
    omp_init_lock(&writelock);

    while (scanf("%d", &N) == 1) {
        case_cnt++;

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                scanf(" %c", &board[i][j]);

        int nb_answers = solve_n_queens();
        printf("Case %d: %d\n", case_cnt, nb_answers);
    }

    omp_destroy_lock(&writelock);
    return 0;
}
