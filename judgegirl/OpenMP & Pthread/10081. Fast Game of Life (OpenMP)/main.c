#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>

#define MAXN 2000+5
#define MAXM 1000

int main(int argc, char* argv[]) {
    int N, M, turns = 0, next_turns, nachbarn;
    char c;
    bool board[2][MAXN][MAXN];

    // clear board
    memset(board, 0, sizeof(bool) * 2 * MAXN * MAXN);

    // input N, M
    scanf("%d %d", &N, &M);

    // input board
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++) {
            scanf(" %c", &c);
            if (c == '1')
                board[0][i][j] = 1;
        }

    // simulations
    int nb_thread = 20, blk = (N + nb_thread - 1) / nb_thread, lblk, rblk;
    for (int times = 0; times < M; times++) {
        next_turns = (turns + 1) % 2;
        #pragma omp parallel for
        for (int b = 0; b < nb_thread; b++) {
            lblk = blk * b + 1;
            rblk = ((blk * (b+1) + 1) < N) ? (blk * (b+1) + 1) : N;
            for (int i = lblk; i <= rblk; i++) {
                for (int j = 1; j <= N; j++) {
                    nachbarn = board[turns][i-1][j-1] + board[turns][i-1][j] +
                               board[turns][i-1][j+1] + board[turns][i][j-1] +
                               board[turns][i][j+1] + board[turns][i+1][j-1] +
                               board[turns][i+1][j] + board[turns][i+1][j+1];
                    if (board[turns][i][j]) {
                        if (nachbarn < 2)
                            board[next_turns][i][j] = 0;
                        else if (nachbarn < 4)
                            board[next_turns][i][j] = 1;
                        else
                            board[next_turns][i][j] = 0;
                    }
                    else {
                        if (nachbarn == 3)
                            board[next_turns][i][j] = 1;
                        else
                            board[next_turns][i][j] = 0;
                    }
                }
            }
        }
        turns = next_turns;
    }

    // output result
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            printf(board[turns][i][j] ? "1": "0");
        }
        printf("\n");
    }

    return 0;
}
