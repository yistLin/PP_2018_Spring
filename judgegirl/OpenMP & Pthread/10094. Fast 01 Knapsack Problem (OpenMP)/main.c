#include <stdio.h>

#define MAXN 10005
#define MAXM 1000005

int weights[MAXN];
int values[MAXN];

int main() {
    int N, M;

    scanf("%d%d", &N, &M);
    for (int i = 0; i < N; i++)
        scanf("%d%d", &weights[i], &values[i]);

    return 0;
}
