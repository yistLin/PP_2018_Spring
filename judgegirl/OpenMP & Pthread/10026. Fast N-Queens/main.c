#include <stdio.h>
#include <omp.h>

#define MAXN 20

int N = 0;
char board[MAXN][MAXN];

int solve(int row, int state[MAXN]) {
    if (row == N)
        return 1;

    int nb_answers = 0;
    for (int i = 0; i < N; i++) {
        if (board[row][i] == '*')
            continue;
        
        // detect collisions
        int r = 0;
        for (; r < row; r++) {
            if (state[r] == i)  // column
                break;
            if ((r - state[r]) == (row - i))  // diagonal
                break;
            if ((r + state[r]) == (row + i))  // inverse diagonal
                break;
        }

        if (r == row) {  // no collision with previous queens
            state[row] = i;
            nb_answers += solve(row+1, state);
        }
    }

    return nb_answers;
}

int solve_n_queens() {
    int nb_answers = 0;

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        if (board[0][i] == '*')
            continue;
        int state[MAXN];
        state[0] = i;
        nb_answers += solve(1, state);
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
