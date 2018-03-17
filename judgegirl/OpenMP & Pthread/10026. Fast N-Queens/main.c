#include <stdio.h>
#include <omp.h>

#define MAXN 20

int N = 0;
char board[MAXN][MAXN];

int solve_n_queens() {
    int nb_answers = 0;

    #pragma omp parallel for
    for (int tid = 0; tid < N; tid++) {
        int iptrs[MAXN];
        long long int cols[MAXN], diags[MAXN], rdiags[MAXN];

        int row, i, j, k;
        long long int col, diag, rdiag;
        int ret = 0;

        int sptr = 1;
        i = tid;
        j = i + N - 1;
        k = i;
        if (board[0][i] == '*')
            continue;
        iptrs[1] = 0;
        cols[1] = -1L ^ (1L << i);
        diags[1] = -1L ^ (1L << j);
        rdiags[1] = -1L ^ (1L << k);

        while (1) {
            if (sptr < 1)
                break;

            row = sptr;
            if (row == N) {
                ret++;
                sptr--;
                continue;
            }

            i = iptrs[sptr];
            j = i - row + N - 1;
            k = row + i;
            col = cols[sptr];
            diag = diags[sptr];
            rdiag = rdiags[sptr];

            while (i < N) {
                if (board[row][i] != '*' &&
                    (col & (1L << i)) &&
                    (diag & (1L << j)) &&
                    (rdiag & (1L << k))) {
                    col ^= (1L << i);
                    diag ^= (1L << j);
                    rdiag ^= (1L << k);

                    iptrs[sptr] = i+1;

                    sptr++;
                    iptrs[sptr] = 0;
                    cols[sptr] = col;
                    diags[sptr] = diag;
                    rdiags[sptr] = rdiag;
                    break;
                }
                i++;
                j = i - row + N - 1;
                k = row + i;
            }

            if (i == N) {
                sptr--;
            }
        }

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
