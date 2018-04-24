#include <stdio.h>

#define MAXN 20

unsigned board[MAXN];

int solve(int N, int row, unsigned col, unsigned diag, unsigned rdiag) {
    if (row == N)
        return 1;

    int nb_answers = 0;
    unsigned pos = board[row] | col | diag | rdiag, setbit = 1;
    for (int i = 0; i < N; i++, setbit <<= 1) {
        if (~pos & setbit)
            nb_answers += solve(N, row+1,
                                col | setbit,
                                (diag | setbit) >> 1,
                                (rdiag | setbit) << 1);
    }

    return nb_answers;
}

int main(int argc, char* argv[]) {
    int N, case_cnt = 0;
    char str[MAXN+5];

    while (scanf("%d", &N) == 1) {
        case_cnt++;

        // input
        for (int i = 0; i < N; i++) {
            board[i] = 0;
            scanf("%s", str);
            for (int j = 0; j < N; j++)
                if (str[j] == '*')
                    board[i] |= (1 << j);
        }

        int nb_answers = 0;

        #pragma omp parallel for reduction(+: nb_answers)
        for (int i = 0; i < N; i++) {
            unsigned setbit = 1 << i;
            if (~board[0] & setbit) {
                unsigned col = 0, diag = 0, rdiag = 0;
                int ret = solve(N, 1,
                                col | setbit,
                                (diag | setbit) >> 1,
                                (rdiag | setbit) << 1);
                nb_answers += ret;
            }
        }

        printf("Case %d: %d\n", case_cnt, nb_answers);
    }

    return 0;
}