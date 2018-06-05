#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>
#include <omp.h>
#include <CL/cl.h>

#define MAXGPU 3
#define MAXTHREAD 512
#define MAXK 4096
#define MAXN 1073741824

int main(int argc, char *argv[]) {
    cl_int status;
    cl_platform_id platform_id;
    cl_uint platform_id_got;
    status = clGetPlatformIDs(1, &platform_id, &platform_id_got);
    assert(status == CL_SUCCESS && platform_id_got == 1);

    cl_device_id GPU[MAXGPU];
    cl_uint GPU_id_got;
    status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, MAXGPU, GPU, &GPU_id_got);
    assert(status == CL_SUCCESS);

    /* getcontext */
    cl_context context = clCreateContext(NULL, MAXGPU, GPU, NULL, NULL, &status);
    assert(status == CL_SUCCESS);

    /* commandqueue */
    cl_command_queue commandQueues[MAXGPU];
    for (int i = 0; i < MAXGPU; i++) {
        commandQueues[i] = clCreateCommandQueue(context, GPU[i], 0, &status);
        assert(status == CL_SUCCESS);
    }

    /* kernelsource */
    FILE *kernelfp = fopen("vecdot.cl", "r");
    assert(kernelfp != NULL);
    char kernelBuffer[MAXK];
    const char *constKernelSource = kernelBuffer;
    size_t kernelLength = fread(kernelBuffer, 1, MAXK, kernelfp);
    cl_program program = clCreateProgramWithSource(context, 1, &constKernelSource, &kernelLength, &status);
    assert(status == CL_SUCCESS);

    /* buildprogram */
    status = clBuildProgram(program, MAXGPU, GPU, NULL, NULL, NULL);
    assert(status == CL_SUCCESS);

    /* createkernel */
    cl_kernel kernels[MAXGPU];
    for (int i = 0; i < MAXGPU; i++) {
        kernels[i] = clCreateKernel(program, "vec_dot", &status);
        assert(status == CL_SUCCESS);
    }

    /* vector */
    int local_work_size = 512;
    size_t buf_size = (MAXN / MAXTHREAD) / local_work_size;
    cl_uint* buffers[MAXGPU];
    for (int i = 0; i < MAXGPU; i++) {
        buffers[i] = (cl_uint*)malloc(buf_size * sizeof(cl_uint));
        assert(buffers[i] != NULL);
    }

    /* createbuffer */
    cl_mem dev_buffers[MAXGPU];
    for (int i = 0; i < MAXGPU; i++) {
        dev_buffers[i] = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, buf_size * sizeof(cl_uint), buffers[i], &status);
        assert(status == CL_SUCCESS);
    }

    int nb_lines = 0, Ns[20000], N;
    uint32_t key1s[20000], key2s[20000], key1, key2, ans[20000];

    while (scanf("%d %" PRIu32 " %" PRIu32, &N, &key1, &key2) == 3) {
        Ns[nb_lines] = N;
        key1s[nb_lines] = key1;
        key2s[nb_lines] = key2;
        nb_lines++;
    }

    size_t nb_workgroups[MAXGPU];

    omp_set_num_threads(MAXGPU);

    #pragma omp parallel for schedule(dynamic, 1) private(status)
    for (int j = 0; j < nb_lines; j++) {

        int i = omp_get_thread_num();

        int subN = Ns[j];
        uint32_t subkey1 = key1s[j];
        uint32_t subkey2 = key2s[j];

        status = clSetKernelArg(kernels[i], 0, sizeof(uint32_t), (void *)&subkey1);
        assert(status == CL_SUCCESS);
        status = clSetKernelArg(kernels[i], 1, sizeof(uint32_t), (void *)&subkey2);
        assert(status == CL_SUCCESS);
        status = clSetKernelArg(kernels[i], 2, sizeof(cl_mem), (void *)&dev_buffers[i]);
        assert(status == CL_SUCCESS);
        status = clSetKernelArg(kernels[i], 3, sizeof(int), (void *)&local_work_size);
        assert(status == CL_SUCCESS);
        status = clSetKernelArg(kernels[i], 4, sizeof(int), (void *)&subN);
        assert(status == CL_SUCCESS);

        size_t expanded_N = (subN - 1) / local_work_size + 1;
        expanded_N = (expanded_N % MAXTHREAD == 0) ? expanded_N : ((expanded_N / MAXTHREAD) + 1) * MAXTHREAD;
        size_t globalThreads[] = {expanded_N};
        size_t localThreads[] = {MAXTHREAD};
        status = clEnqueueNDRangeKernel(commandQueues[i], kernels[i], 1, NULL, globalThreads, localThreads, 0, NULL, NULL);
        assert(status == CL_SUCCESS);

        size_t nb_workgroup = expanded_N / MAXTHREAD;

        clEnqueueReadBuffer(commandQueues[i], dev_buffers[i], CL_TRUE, 0, nb_workgroup * sizeof(cl_uint), buffers[i], 0, NULL, NULL);

        uint32_t sum = 0;
        for (int k = 0; k < nb_workgroup; k++)
            sum += buffers[i][k];

        ans[j] = sum;
    }

    for (int i = 0; i < nb_lines; i++)
        printf("%" PRIu32 "\n", ans[i]);

    /* free */
    for (int i = 0; i < MAXGPU; i++) {
        free(buffers[i]);
        clReleaseCommandQueue(commandQueues[i]);
        clReleaseMemObject(dev_buffers[i]);
        clReleaseKernel(kernels[i]);
    }
    clReleaseContext(context);
    clReleaseProgram(program);

    return 0;
}
