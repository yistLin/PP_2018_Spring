#include <stdio.h>

int main() {
    char X[60000], Y[60000];

    while (scanf("%s %s", X, Y) == 2) {
        printf("X = %s\n", X);
        printf("Y = %s\n", Y);
    }

    return 0;
}
