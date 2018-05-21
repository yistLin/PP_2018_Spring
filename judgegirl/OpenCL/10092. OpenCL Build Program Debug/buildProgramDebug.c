#include <stdio.h>
#include <assert.h>
#include <CL/cl.h>

#define MAXGPU 10
#define MAXK 1024

int main(int argc, char *argv[]) {
    char kernelFileName[MAXK];
    int ret = scanf("%s", kernelFileName);

    cl_int status;
    cl_platform_id platform_id;
    cl_uint platform_id_got;
    status = clGetPlatformIDs(1, &platform_id, &platform_id_got);
    assert(status == CL_SUCCESS && platform_id_got == 1);
    // printf("%d platform found\n", platform_id_got);

    cl_device_id GPU[MAXGPU];
    cl_uint GPU_id_got;
    status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, MAXGPU, GPU, &GPU_id_got);
    assert(status == CL_SUCCESS);
    // printf("There are %d GPU devices\n", GPU_id_got);

    /* getcontext */
    cl_context context = clCreateContext(NULL, GPU_id_got, GPU, NULL, NULL, &status);
    assert(status == CL_SUCCESS);

    /* commandqueue */
    cl_command_queue commandQueue = clCreateCommandQueue(context, GPU[0], 0, &status);
    assert(status == CL_SUCCESS);

    /* kernelsource */
    FILE *kernelfp = fopen(kernelFileName, "r");
    assert(kernelfp != NULL);
    char kernelBuffer[MAXK];
    const char *constKernelSource = kernelBuffer;
    size_t kernelLength = fread(kernelBuffer, 1, MAXK, kernelfp);
    // printf("The size of kernel source is %zu\n", kernelLength);
    cl_program program = clCreateProgramWithSource(context, 1, &constKernelSource, &kernelLength, &status);
    assert(status == CL_SUCCESS);

    /* buildprogram */
    status = clBuildProgram(program, GPU_id_got, GPU, NULL, NULL, NULL);
    // assert(status == CL_SUCCESS);
    // printf("Build program completes\n");

    /* buildProgramInfo */
    char buildLog[4096];
    size_t buildLogLength;
    status = clGetProgramBuildInfo(program, GPU[0], CL_PROGRAM_BUILD_LOG, (size_t)4096, (void *)buildLog, &buildLogLength);
    assert(status == CL_SUCCESS);

    buildLog[buildLogLength] = '\0';
    printf("%s", buildLog);

    clReleaseContext(context);
    clReleaseCommandQueue(commandQueue);
    clReleaseProgram(program);

    return 0;
}
