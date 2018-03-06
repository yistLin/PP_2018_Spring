#include "DRS.h"

int32_t search_range(Rect rect, int32_t x[], int32_t y[],
        int32_t w[], int32_t n) {
    int32_t ret = 0;
    for (int i = 0; i < n; i++) {
        if (rect.lx <= x[i] && x[i] <= rect.rx &&
            rect.ly <= y[i] && y[i] <= rect.ry) {
            ret += w[i];
        }
    }
    return ret;
}
