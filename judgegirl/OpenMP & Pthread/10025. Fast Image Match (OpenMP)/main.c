#include <stdio.h>
#include <omp.h>

static inline int square(int x) {
    return x * x;
}

int main(int argc, char *argv[]) {
    int a_h, a_w, b_h, b_w;
    int A[500][500], B[500][500], Diff[500][500];

    while (scanf("%d%d%d%d", &a_h, &a_w, &b_h, &b_w) == 4) {
        for (int i = 0; i < a_h; i++)
            for (int j = 0; j < a_w; j++)
                scanf("%d", &A[i][j]);

        for (int i = 0; i < b_h; i++)
            for (int j = 0; j < b_w; j++)
                scanf("%d", &B[i][j]);

        // image match
        int min_x, min_y, min_diff = 0x7FFFFFFF;  // max int32_t
        # pragma omp parallel for
        for (int x = 0; x <= (a_h - b_h); x++) {
            for (int y = 0; y <= (a_w - b_w); y++) {
                int diff = 0;
                for (int i = 0; i < b_h; i++) {
                    for (int j = 0; j < b_w; j++) {
                        diff += square(A[x+i][y+j] - B[i][j]);
                    }
                }
                Diff[x][y] = diff;
            }
        }

        for (int x = 0; x <= (a_h - b_h); x++) {
            for (int y = 0; y <= (a_w - b_w); y++) {
                if (Diff[x][y] < min_diff) {
                    min_diff = Diff[x][y];
                    min_x = x + 1;
                    min_y = y + 1;
                }
            }
        }

        printf("%d %d\n", min_x, min_y);
    }

    return 0;
}
