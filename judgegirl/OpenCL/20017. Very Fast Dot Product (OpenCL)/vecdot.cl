__kernel void vec_dot(
    const unsigned key1,
    const unsigned key2,
    __global unsigned *sum,
    const int local_work_size,
    const int N) {

    unsigned rotate_left, a, b, tmp = 0;
    int base = get_global_id(0) * local_work_size;
    for (int i = base; i < N && (i-base) < local_work_size; i++) {
        rotate_left = (i << (key1 & 31)) | (i >> (32 - (key1 & 31)));
        a = (rotate_left + key1) ^ key1;
        rotate_left = (i << (key2 & 31)) | (i >> (32 - (key2 & 31)));
        b = (rotate_left + key2) ^ key2;
        tmp += a * b;
    }
    sum[get_global_id(0)] = tmp;
}
