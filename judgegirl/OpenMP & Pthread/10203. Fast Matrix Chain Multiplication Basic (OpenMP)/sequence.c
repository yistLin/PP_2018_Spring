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

        #pragma omp parallel
        for (int i = 1; i < N; i++) {
            #pragma omp for
            for (int j = 0; j < N-i; j++) {
                int l = j, r = j+i;
                long long local = INF;
                long long *h_ptr = dp + l*N+l;
                long long *v_ptr = dp + r*N+(l+1);
                long long mul_sz = SZ[l] * SZ[r+1];
                for (int k = l; k < r; k++, h_ptr++, v_ptr++) {
                    long long t = *h_ptr + *v_ptr + mul_sz * SZ[k+1];
                    if (t < local)
                        local = t;
                }
                dp[l*N+r] = local;
                dp[r*N+l] = local;
            }
        }
        printf("%lld\n", dp[0*N+N-1]);
    }
    return 0;
}
