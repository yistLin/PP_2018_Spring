#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    char board[20][20];
    int N, case_cnt = 0, nb_answers = 0;

    while (scanf("%d", &N) == 1) {
        case_cnt++;

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                scanf(" %c", &board[i][j]);

        printf("Case %d: %d\n", case_cnt, nb_answers);
    }
    return 0;
}
