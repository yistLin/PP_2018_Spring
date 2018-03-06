#include "utils.h"

int ret[128];
int run(int n, int key) {
    int sum = 0;
    f(n, key, ret, ret+32, ret+64, ret+96);
    for (int i = 0; i < 128; i+=32)
        sum += ret[i];
    return sum;
}
