static inline unsigned rotate_left(const unsigned val, const unsigned key) {
    return (val << key) | (val >> (32 - key));
}

static inline unsigned encrypt(const unsigned val, const unsigned key) {
    return (rotate_left(val, key & 31) + key) ^ key;
}

__kernel void vec_dot(
    const unsigned key1,
    const unsigned key2,
    __global unsigned *sum,
    const int local_work_size,
    const int N
) {
    __local unsigned tmpsum[512];

    unsigned tmp = 0;
    int base = get_global_id(0) * local_work_size;
    for (int i = base; i < N && (i-base) < local_work_size; i++) {
        tmp += encrypt(i, key1) * encrypt(i, key2);
    }
    tmpsum[get_local_id(0)] = tmp;
    barrier(CLK_LOCAL_MEM_FENCE);

    #pragma unroll 9
    for (int i = (512 / 2); i > 0; i /= 2) {
        if (get_local_id(0) < i)
            tmpsum[get_local_id(0)] += tmpsum[get_local_id(0)+i];
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if (get_local_id(0) == 0)
        sum[get_group_id(0)] = tmpsum[0];
}
