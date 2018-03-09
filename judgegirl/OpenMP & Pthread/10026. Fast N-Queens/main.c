#include <stdio.h>
#include <omp.h>

int N = 0;
char board[20][20];

int solve(int row, int state[20]) {
    if (row == N) {
        printf("row = %d, the end.\n", N);
        return 1;
    }

    int nb_answers = 0;
    for (int i = 0; i < N; i++) {
        if (board[row][i] == '*')
            continue;
    }
}

int main(int argc, char* argv[]) {
    int case_cnt = 0;

    while (scanf("%d", &N) == 1) {
        case_cnt++;

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                scanf(" %c", &board[i][j]);

        int nb_answers = 0;
        printf("Case %d: %d\n", case_cnt, nb_answers);
    }
    return 0;
}
