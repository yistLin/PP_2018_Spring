#include "labeling.h"
#include <cstdio>
#include <cassert>

#define MAXSIZE (40 << 20)
#define THREAD_OF_BLOCK 1024

namespace lab2 {
    __device__ int segscan_warp(int *ptr, bool *flag, const unsigned int tid) {
        const unsigned int lane = tid & 31;
        if (lane >= 1) {
            ptr[tid] = flag[tid] ? ptr[tid] : ptr[tid-1] + ptr[tid];
            flag[tid] = flag[tid-1] | flag[tid];
        }
        if (lane >= 2) {
            ptr[tid] = flag[tid] ? ptr[tid] : ptr[tid-2] + ptr[tid];
            flag[tid] = flag[tid-2] | flag[tid];
        }
        if (lane >= 4) {
            ptr[tid] = flag[tid] ? ptr[tid] : ptr[tid-4] + ptr[tid];
            flag[tid] = flag[tid-4] | flag[tid];
        }
        if (lane >= 8) {
            ptr[tid] = flag[tid] ? ptr[tid] : ptr[tid-8] + ptr[tid];
            flag[tid] = flag[tid-8] | flag[tid];
        }
        if (lane >= 16) {
            ptr[tid] = flag[tid] ? ptr[tid] : ptr[tid-16] + ptr[tid];
            flag[tid] = flag[tid-16] | flag[tid];
        }
        return ptr[tid];
    }

    __device__ int segscan_block(int *ptr, bool *flag, const unsigned int tid) {
        unsigned int warpid = tid >> 5;
        unsigned int warp_first = warpid << 5;
        unsigned int warp_last = warp_first + 31;

        bool warp_is_open = (flag[warp_first] == 0);
        __syncthreads();

        int val = segscan_warp(ptr, flag, tid);
        int warp_total = ptr[warp_last];
        int warp_flag = (flag[warp_last] != 0 || !warp_is_open);
        bool will_accumulate = (warp_is_open && flag[tid] == 0);
        __syncthreads();

        if (tid == warp_last) {
            ptr[warpid] = warp_total;
            flag[warpid] = warp_flag;
        }
        __syncthreads();

        if (warpid == 0) {
            segscan_warp(ptr, flag, tid);
        }
        __syncthreads();

        if (warpid != 0 && will_accumulate) {
            val = ptr[warpid-1] + val;
        }
        __syncthreads();

        ptr[tid] = val;
        __syncthreads();

        return val;
    }

    __global__ void segscan(const char *i_data, int *o_data, int data_size) {
        __shared__ int val[THREAD_OF_BLOCK];
        __shared__ bool flag[THREAD_OF_BLOCK];
        const unsigned int tid = threadIdx.x;
        const unsigned int base = blockIdx.x * blockDim.x * 4 + 4 * tid;

        // Handle 4 elements in global device memory
        int sub_val[4] = { 0 };
        for (int i = 0; i < 4 && (base + i) < data_size; i++) {
            sub_val[i] = (i_data[base + i] == ' ') ? 0 : 1;
        }
        bool sub_flag[4] = {1, 1, 1, 1};
        for (int i = 0; i < 4 && (base + i) < data_size; i++) {
            if (i_data[base + i] != ' ' && (base + i > 0) && i_data[base + i - 1] != ' ') {
                sub_flag[i] = 0;
            }
        }
        __syncthreads();

        // Segscan 4 elements sequentially
        for (int i = 1; i < 4; i++) {
            sub_val[i] = sub_flag[i] ? sub_val[i] : sub_val[i-1] + sub_val[i];
            sub_flag[i] = sub_flag[i-1] | sub_flag[i];
        }
        val[tid] = sub_val[3];
        flag[tid] = sub_flag[3];
        __syncthreads();

        // Intra-block scan
        int ret = segscan_block(val, flag, tid);
        __syncthreads();

        // Accumulate
        if (tid != 0) {
            for (int i = 0; i < 4; i++) {
                if (sub_flag[0] == 0 && sub_flag[i] == 0) {
                    sub_val[i] += val[tid - 1];
                }
            }
        }
        __syncthreads();

        // Write back
        for (int i = 0; i < 4 && (base + i) < data_size; i++) {
            o_data[base + i] = sub_val[i];
        }
    }

    __global__ void sweep(int *data, int data_size) {
        const unsigned int base = (blockIdx.x + 1) * (THREAD_OF_BLOCK * 4);
        const unsigned int tid = threadIdx.x;
        if (base + tid < data_size) {
            if (data[base + tid] - 1 == tid) {
                data[base + tid] += data[base - 1];
            }
        }
    }
}

void labeling(const char *cuStr, int *cuPos, int strLen){
    int nb_blocks = ((strLen-1)/THREAD_OF_BLOCK) / 4 + 1;
    lab2::segscan<<<nb_blocks, THREAD_OF_BLOCK>>>(cuStr, cuPos, strLen);
    lab2::sweep<<<nb_blocks-1, THREAD_OF_BLOCK>>>(cuPos, strLen);
}
