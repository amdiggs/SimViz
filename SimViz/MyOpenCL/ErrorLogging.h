
#ifndef ErrorLogging_h
#define ErrorLogging_h
#include <stdio.h>

inline void CL_Check_Error(int error){
    if(error != 0){
        int test = 4;
    }
    switch(error){
        case 0: return;//printf("OpenCl Error CL_SUCCESS\n"); exit(3);
        case -1: printf("OpenCl Error CL_DEVICE_NOT_FOUND\n"); exit(3);
        case -2: printf("OpenCl Error CL_DEVICE_NOT_AVAILABLE\n"); exit(3);
        case -3: printf("OpenCl Error CL_COMPILER_NOT_AVAILABLE\n"); exit(3);
        case -4: printf("OpenCl Error CL_MEM_OBJECT_ALLOCATION_FAILURE\n"); exit(3);
        case -5: printf("OpenCl Error CL_OUT_OF_RESOURCES\n"); exit(3);
        case -6: printf("OpenCl Error CL_OUT_OF_HOST_MEMORY\n"); exit(3);
        case -7: printf("OpenCl Error CL_PROFILING_INFO_NOT_AVAILABLE\n"); exit(3);
        case -8: printf("OpenCl Error CL_MEM_COPY_OVERLAP\n"); exit(3);
        case -9: printf("OpenCl Error CL_IMAGE_FORMAT_MISMATCH\n"); exit(3);
        case -10: printf("OpenCl Error CL_IMAGE_FORMAT_NOT_SUPPORTED\n"); exit(3);
        case -11: printf("OpenCl Error CL_BUILD_PROGRAM_FAILURE\n"); exit(3);
        case -12: printf("OpenCl Error CL_MAP_FAILURE\n"); exit(3);
        case -13: printf("OpenCl Error CL_MISALIGNED_SUB_BUFFER_OFFSET\n"); exit(3);
        case -14: printf("OpenCl Error CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST\n"); exit(3);
        case -15: printf("OpenCl Error CL_COMPILE_PROGRAM_FAILURE\n"); exit(3);
        case -16: printf("OpenCl Error CL_LINKER_NOT_AVAILABLE\n"); exit(3);
        case -17: printf("OpenCl Error CL_LINK_PROGRAM_FAILURE\n"); exit(3);
        case -18: printf("OpenCl Error CL_DEVICE_PARTITION_FAILED\n"); exit(3);
        case -19: printf("OpenCl Error CL_KERNEL_ARG_INFO_NOT_AVAILABLE\n"); exit(3);
        case -30: printf("OpenCl Error CL_INVALID_VALUE\n"); exit(3);
        case -31: printf("OpenCl Error CL_INVALID_DEVICE_TYPE\n"); exit(3);
        case -32: printf("OpenCl Error CL_INVALID_PLATFORM\n"); exit(3);
        case -33: printf("OpenCl Error CL_INVALID_DEVICE\n"); exit(3);
        case -34: printf("OpenCl Error CL_INVALID_CONTEXT\n"); exit(3);
        case -35: printf("OpenCl Error CL_INVALID_QUEUE_PROPERTIES\n"); exit(3);
        case -36: printf("OpenCl Error CL_INVALID_COMMAND_QUEUE\n"); exit(3);
        case -37: printf("OpenCl Error CL_INVALID_HOST_PTR\n"); exit(3);
        case -38: printf("OpenCl Error CL_INVALID_MEM_OBJECT\n"); exit(3);
        case -39: printf("OpenCl Error CL_INVALID_IMAGE_FORMAT_DESCRIPTOR\n"); exit(3);
        case -40: printf("OpenCl Error CL_INVALID_IMAGE_SIZE\n"); exit(3);
        case -41: printf("OpenCl Error CL_INVALID_SAMPLER\n"); exit(3);
        case -42: printf("OpenCl Error CL_INVALID_BINARY\n"); exit(3);
        case -43: printf("OpenCl Error CL_INVALID_BUILD_OPTIONS\n"); exit(3);
        case -44: printf("OpenCl Error CL_INVALID_PROGRAM\n"); exit(3);
        case -45: printf("OpenCl Error CL_INVALID_PROGRAM_EXECUTABLE\n"); exit(3);
        case -46: printf("OpenCl Error CL_INVALID_KERNEL_NAME\n"); exit(3);
        case -47: printf("OpenCl Error CL_INVALID_KERNEL_DEFINITION\n"); exit(3);
        case -48: printf("OpenCl Error CL_INVALID_KERNEL\n"); exit(3);
        case -49: printf("OpenCl Error CL_INVALID_ARG_INDEX\n"); exit(3);
        case -50: printf("OpenCl Error CL_INVALID_ARG_VALUE\n"); exit(3);
        case -51: printf("OpenCl Error CL_INVALID_ARG_SIZE\n"); exit(3);
        case -52: printf("OpenCl Error CL_INVALID_KERNEL_ARGS\n"); exit(3);
        case -53: printf("OpenCl Error CL_INVALID_WORK_DIMENSION\n"); exit(3);
        case -54: printf("OpenCl Error CL_INVALID_WORK_GROUP_SIZE\n"); exit(3);
        case -55: printf("OpenCl Error CL_INVALID_WORK_ITEM_SIZE\n"); exit(3);
        case -56: printf("OpenCl Error CL_INVALID_GLOBAL_OFFSET\n"); exit(3);
        case -57: printf("OpenCl Error CL_INVALID_EVENT_WAIT_LIST\n"); exit(3);
        case -58: printf("OpenCl Error CL_INVALID_EVENT\n"); exit(3);
        case -59: printf("OpenCl Error CL_INVALID_OPERATION\n"); exit(3);
        case -60: printf("OpenCl Error CL_INVALID_GL_OBJECT\n"); exit(3);
        case -61: printf("OpenCl Error CL_INVALID_BUFFER_SIZE\n"); exit(3);
        case -62: printf("OpenCl Error CL_INVALID_MIP_LEVEL\n"); exit(3);
        case -63: printf("OpenCl Error CL_INVALID_GLOBAL_WORK_SIZE\n"); exit(3);
        case -64: printf("OpenCl Error CL_INVALID_PROPERTY\n"); exit(3);
        case -65: printf("OpenCl Error CL_INVALID_IMAGE_DESCRIPTOR\n"); exit(3);
        case -66: printf("OpenCl Error CL_INVALID_COMPILER_OPTIONS\n"); exit(3);
        case -67: printf("OpenCl Error CL_INVALID_LINKER_OPTIONS\n"); exit(3);
        case -68: printf("OpenCl Error CL_INVALID_DEVICE_PARTITION_COUNT\n"); exit(3);
    }
}

template <typename T> inline
T Safe_Get(T func, int* err) {
    CL_Check_Error(*err);
    return func;
}

#endif
