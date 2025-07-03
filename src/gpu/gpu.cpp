#include <CL/cl.h>
#include <map>
#include <string>

#include "gpu.h"
#include "models/util/listWithSize.h"

using std::string;

using gpu::GpuProps;

utils::ListWithSize<cl_device_id> *devices;
utils::ListWithSize<cl_command_queue> *command_queues;
cl_context *context;
map<const char*, cl_kernel> *available_kernels;

#define SET_ERROR_IF_NULL cl_int local_err; if (err == NULL) err = &local_err;
#define RETURN_ON_ERROR if (CL_SUCCESS != err) return err;

/**
 * @brief Get the GPU devices to run the computation on.
 * 
 * @return the OpenCL error code. Initializes the devices and command queues in globals if err = CL_SUCCESS.
 */
cl_int gpu::initializeGpuData(size_t device_no) {
    cl_int err;
    cl_uint numPlatforms;
    cl_uint numDevices;

    cl_platform_id *platforms = new cl_platform_id();
    cl_device_id *devices_local = new cl_device_id[MAX_DEVICES];

    // Get platforms.
    err = clGetPlatformIDs(1, platforms, &numPlatforms);
    if (CL_SUCCESS != err) {
        delete platforms;
        delete[] devices_local;
        return err;
    }

    // Get GPU devices from the platform.
    err = clGetDeviceIDs(*platforms, CL_DEVICE_TYPE_GPU, MAX_DEVICES, devices_local, &numDevices);
    if (CL_SUCCESS != err) {
        delete platforms;
        delete[] devices_local;
        return err;
    }

    devices = new utils::ListWithSize<cl_device_id>(numDevices, devices_local);

    // Device selector
    if (device_no != -1 && device_no < devices->size()) {
        (*devices)[0] = (*devices)[device_no];
        // Note: Size remains unchanged as ListWithSize does not support resizing
    }

    delete platforms;

    context = new cl_context();
    *context = clCreateContext(NULL, devices->size(), devices->data(), NULL, NULL, &err);

    if (CL_SUCCESS != err) {
        for (size_t i = 0; i < devices->size(); i++) {
            clReleaseDevice((*devices)[i]);
        }

        delete[] devices_local;
        return err;
    }

    cl_command_queue *queues = new cl_command_queue[devices->size()];
    for (size_t i = 0; i < devices->size(); i++) {
        queues[i] = clCreateCommandQueueWithProperties(*context, (*devices)[i], NULL, &err);

        if (CL_SUCCESS != err) {
            clReleaseContext(*context);

            for (size_t i = 0; i < devices->size(); i++) {
                clReleaseDevice((*devices)[i]);
            }

            // ListWithSize manages its own memory, no need to delete data directly
            delete[] queues;

            return err;
        }

    }
    
    command_queues = new utils::ListWithSize<cl_command_queue>(devices->size(), queues);

    available_kernels = new std::map<const char*, cl_kernel>();

    return CL_SUCCESS;
}

/**
 * @brief Cleans up all GPU data when done, freeing device and kernel global variables.
 */
void gpu::destroyGpuData() {
    for (auto i = available_kernels -> begin(); i != available_kernels -> end(); i++) {
        clReleaseKernel(i -> second);
    }

    for (size_t i = 0; i < command_queues->size(); i++) {
        clReleaseCommandQueue((*command_queues)[i]);
    }

    clReleaseContext(*context);

    for (size_t i = 0; i < devices->size(); i++) {
        clReleaseDevice((*devices)[i]);
    }

    // ListWithSize manages its own memory, no need to delete data directly

    delete devices;
    delete command_queues;
    delete context;
    delete available_kernels;
}

// This function generated using AI
GpuProps gpu::getDeviceProps(cl_device_id device) {
    GpuProps output = GpuProps();
    cl_int err;

    // get the device name
    char name[128];
    err = clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(name), &name, NULL);
    output.name = name;

    // get the device vendor
    char vendor[128];
    err = clGetDeviceInfo(device, CL_DEVICE_VENDOR, sizeof(vendor), &vendor, NULL);

    output.vendor = vendor;

    // get the maximum compute units
    err = clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(output.max_compute_units), &output.max_compute_units, NULL);

    // get the maximum work group size
    err = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(output.max_work_group_size), &output.max_work_group_size, NULL);

    // get the maximum work item sizes
    err = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(output.max_work_item_sizes), &output.max_work_item_sizes, NULL);

    // get the total size of shared memory per block
    err = clGetDeviceInfo(device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &output.shared_per_SM, NULL);
    output.err = err;

    return output;
}

cl_kernel gpu::compileKernel(const char *kernelCode, const char *kernelFunctionName, cl_int *err) {
    SET_ERROR_IF_NULL;

    // Find and compile the program to be run on the GPU.
    cl_program program = clCreateProgramWithSource(*context, 1, &kernelCode, NULL, err);
    if (CL_SUCCESS != *err) return cl_kernel();

    clBuildProgram(program, devices->size(), devices->data(), NULL, NULL, NULL);

    cl_kernel kernel = clCreateKernel(program, kernelFunctionName, err);

    clReleaseProgram(program);

    return kernel;
}

cl_kernel gpu::compileKernelIfNotExists(const char *kernel, const char *kernelName, cl_int *err) {
    SET_ERROR_IF_NULL;
    if (available_kernels -> find(kernelName) == available_kernels -> end()) {
        (*available_kernels)[kernelName] = compileKernel(kernel, kernelName, err);
    }

    return (*available_kernels)[kernelName];
}

bool gpu::kernelExists(const char *kernelName) {
    return available_kernels -> find(kernelName) != available_kernels -> end();
}
cl_mem gpu::deviceOutputAllocate(size_t nbytes, cl_int *err) {
    SET_ERROR_IF_NULL;
    return clCreateBuffer(*context, CL_MEM_WRITE_ONLY, nbytes, NULL, err);
}

cl_int gpu::launchKernel(cl_kernel kernel, utils::ListWithSize<size_t> gridDim, utils::ListWithSize<size_t> blockDim) {
    cl_int err;

    for (size_t i = 0; i < command_queues->size(); i++) {
        err = clEnqueueNDRangeKernel((*command_queues)[i], kernel, gridDim.size(), NULL, gridDim.data(), blockDim.data(), 0, NULL, NULL);
        RETURN_ON_ERROR;
    }

    for (size_t i = 0; i < command_queues->size(); i++) {
        err = clFinish((*command_queues)[i]); 
        RETURN_ON_ERROR;
    }

    return CL_SUCCESS;
}
