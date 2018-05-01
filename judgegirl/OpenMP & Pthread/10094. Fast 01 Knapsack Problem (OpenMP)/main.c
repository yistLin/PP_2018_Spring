#include <stdio.h>

#define MAXN 10005
#define MAXM 1000005

#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

int weights[MAXN];
int values[MAXN];
int dp[2][MAXM];

int main() {
    int N, M;

    scanf("%d%d", &N, &M);
    for (int i = 0; i < N; i++)
        scanf("%d%d", &weights[i], &values[i]);

    int cur = 0, nxt;
    for (int i = 0; i < N; i++) {
        nxt = 1 - cur;
        for (int j = M; j >= weights[i]; j--)
            dp[nxt][j] = MAX(dp[cur][j], dp[cur][j-weights[i]] + values[i]);
        for (int j = weights[i]-1; j >= 0; j--)
            dp[nxt][j] = dp[cur][j];
        cur = nxt;
    }

    printf("%d\n", dp[nxt][M]);

    return 0;
}
