#include <stdio.h>
#include <string.h>

#define P 4
#define MAXN 1000000 + 5

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int main(int argc, char* argv[]) {
    int N, arr[MAXN];
    int last_idx[1000];
    memset(arr, 0, sizeof(int) * MAXN);

    // input
    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d", &arr[i]);
    }

    // distribute work
    int lptr, rptr, blk_size = (N+P-1) / P;
    for (int p = 0; p < P; p++) {
        lptr = blk_size * p;
        rptr = MIN(blk_size * (p+1), N);
        last_idx[p] = rptr - 1;

        for (int i = lptr+1; i < rptr; i++)
            arr[i] += arr[i-1];
        // turns = 0;
        // for (int l_shift = 1; l_shift < (rptr - lptr); l_shift *= 2) {
        //     next_turns = (turns + 1) % 2;
        //     for (int i = lptr; i < lptr + l_shift; i++)
        //         arr[next_turns][i] = arr[turns][i];
        //     for (int i = lptr + l_shift; i < rptr; i++) {
        //         arr[next_turns][i] = arr[turns][i] + arr[turns][i - l_shift];
        //     }
        //     turns = (turns + 1) % 2;
        // }
    }

    // tree sum on last element
    int tree_sum[P][P];
    for (int p = 0; p < P; p++) {  // copy last element
        tree_sum[0][p] = arr[last_idx[p]];
    }
    int last_rnd = 0;
    for (int i = 0, l_shift = 1; l_shift < P; i++, l_shift *= 2) {
        for (int p = 0; p < l_shift; p++)
            tree_sum[i+1][p] = tree_sum[i][p];
        for (int p = l_shift; p < P; p++)
            tree_sum[i+1][p] = tree_sum[i][p] + tree_sum[i][p - l_shift];
        last_rnd = i + 1;
    }
    for (int p = 0; p < P; p++)
        arr[last_idx[p]] = tree_sum[last_rnd][p];  // write back

    // accumulated sum
    int prev_sum = 0;
    for (int p = 1; p < P; p++) {
        lptr = blk_size * p;
        rptr = MIN(blk_size * (p+1), N);
        prev_sum = tree_sum[last_rnd][p-1];

        for (int i = lptr; i < rptr-1; i++)
            arr[i] += prev_sum;
    }

    // output
    for (int i = 0; i < N-1; i++)
        printf("%d ", arr[i]);
    printf("%d\n", arr[N-1]);

    return 0;
}
