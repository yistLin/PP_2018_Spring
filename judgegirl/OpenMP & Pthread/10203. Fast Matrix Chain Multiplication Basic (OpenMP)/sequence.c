#include <stdio.h>

#define MAXN 2048
#define INF (1LL<<60)

int N;
long long dp[MAXN*MAXN], SZ[MAXN+1];

int main() {
    while (scanf("%d", &N) == 1) {
        for (int i = 0; i <= N; i++)
            scanf("%lld", &SZ[i]);
        for (int i=0; i<N; i++)
            dp[i*N + i] = 0;

        for (int i = 1; i < N; i++) {
            for (int j = 0; j < N-i; j++) {
                int l = j, r = j+i;
                long long local = INF;
                for (int k = l; k < r; k++) {
                    long long t = dp[l*N+k] + dp[(k+1)*N+r] + SZ[l] * SZ[k+1] * SZ[r+1];
                    if (t < local)
                        local = t;
                }
                dp[l*N+r] = local;
            }
        }
        printf("%lld\n", dp[0*N+N-1]);
    }
    return 0;
}
