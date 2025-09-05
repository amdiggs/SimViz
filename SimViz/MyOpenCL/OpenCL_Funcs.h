//
//  OpenCL_Funcs.h
//  Ray-Traceing-Updated
//
//  Created by Andrew Diggs on 10/10/24.
//

#ifndef OpenCL_Funcs_h
#define OpenCL_Funcs_h

#include <stdio.h>
#include <OpenCL/opencl.h>


inline std::string Get_Kernel_String(std::string cl_file){
    std::ifstream in_file;
    std::string shader_string;
    in_file.open(cl_file);
    if (!in_file.is_open()){
        std::cout << "ERROR Loading Kernel file!!" << std::endl;
        exit(2);
    }
    std::string line;
    while (getline(in_file, line)) {
        if (!line.empty()){
            shader_string.append(line + "\n");
        }
    }
    return shader_string;
}





inline cl_platform_id Get_Platform(){
    //1: Get the number of cl_platforms.
    cl_uint num = 0;
    CL_Check_Error(clGetPlatformIDs(10, NULL, &num));
    //2: build an array of platforms
    cl_platform_id* platforms = (cl_platform_id*)malloc(num*sizeof(cl_platform_id));
    CL_Check_Error(clGetPlatformIDs(num, platforms, NULL));
    
    if(num ==1){
        cl_platform_id ret = platforms[0];
        free(platforms);
        return ret;
    }
    
    //3: Print the name of each platform
    for(int i = 0; i<num; i++){
        char plat_info[300];
        CL_Check_Error(clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(plat_info), plat_info, NULL));
        printf("PLATFORM INFO: Name = %s\n", plat_info);
        
    }
    int idx;
    printf("Enter the number of the platform that identifies your GPU.\n: ");
    scanf("%d", &idx);
    
    cl_platform_id ret = platforms[idx];
    free(platforms);
    return ret;
}


inline cl_device_id Get_Device(){
    
    cl_platform_id platform_id = Get_Platform();
    cl_device_id device_id;
    CL_Check_Error(clGetDeviceIDs(platform_id,CL_DEVICE_TYPE_ALL, 1, &device_id, NULL));
    return device_id;
    
}



inline cl_image_format Get_Image_Format(int bpp){
    
    cl_image_format format;
    
    if(bpp == 4){
        format.image_channel_order = CL_RGBA;
        format.image_channel_data_type = CL_UNSIGNED_INT8;
    }
    else if(bpp == 3){
        format.image_channel_order = CL_RGB;
        format.image_channel_data_type = CL_UNORM_INT8;
    }
    
    else if(bpp == 2){
        format.image_channel_order = CL_RG;
        format.image_channel_data_type = CL_UNSIGNED_INT8;
    }
    
    
    else{
        printf("Non valid Image format.\n");
        exit(2);
    }
    
    return format;
}




inline void Print_Image_Format(cl_image_format fmt){
    
    switch (fmt.image_channel_order) {
        case CL_R: printf("Order = R "); break;
        case CL_A: printf("Order = A "); break;
        case CL_DEPTH: printf("Order = depth "); break;
        case CL_LUMINANCE: printf("Order = Luminance "); break;
        case CL_INTENSITY: printf("Order = Intensity "); break;
        case CL_RG: printf("Order = RG "); break;
        case CL_RA: printf("Order = RA "); break;
        case CL_Rx: printf("Order = Rx "); break;
        case CL_RGx: printf("Order = RGx "); break;
        case CL_RGB: printf("Order = RGB "); break;
        case CL_RGBx: printf("Order = RGBx "); break;
        case CL_RGBA: printf("Order = RGBA "); break;
        case CL_ARGB: printf("Order = ARGB "); break;
        case CL_BGRA: printf("Order = BGRA "); break;
        default:printf("Order = - "); break;
    }
    
    
    switch (fmt.image_channel_data_type) {
        case CL_SNORM_INT8: printf("Data Type = Snorm int8\n"); break;
        case CL_SNORM_INT16: printf("Data Type = Snorm int16\n"); break;
        case CL_UNORM_INT8: printf("Data Type = Unorm int8\n"); break;
        case CL_UNORM_INT16: printf("Data Type = Unorm int16\n"); break;
        case CL_UNSIGNED_INT8: printf("Data Type = unsigned int8\n"); break;
        case CL_UNSIGNED_INT16: printf("Data Type = unsigned int16\n"); break;
        case CL_UNSIGNED_INT32: printf("Data Type = unsigned int32\n"); break;
        case CL_UNORM_SHORT_555: printf("Data Type = unorm short 555\n"); break;
        case CL_UNORM_SHORT_565: printf("Data Type = unorm short 565\n"); break;
        case CL_UNORM_INT_101010: printf("Data Type = unorm int 101010\n"); break;
        case CL_UNORM_INT24: printf("Data Type = unorm int 24\n"); break;
        case CL_SIGNED_INT8: printf("Data Type = signed int8\n"); break;
        case CL_SIGNED_INT16: printf("Data Type = signed int16\n"); break;
        case CL_SIGNED_INT32: printf("Data Type = signed int32\n"); break;
        case CL_HALF_FLOAT: printf("Data Type = half\n"); break;
        case CL_FLOAT: printf("Data Type = float\n"); break;
        default:printf("Data Type = -\n"); break;
    }
    
    
    
}


inline unsigned int query[10] ={CL_DEVICE_MAX_COMPUTE_UNITS,
    CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,CL_DEVICE_MAX_WORK_GROUP_SIZE
    ,CL_DEVICE_MAX_WORK_ITEM_SIZES ,CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
    CL_DEVICE_LOCAL_MEM_SIZE, CL_DEVICE_GLOBAL_MEM_SIZE ,CL_DEVICE_IMAGE_MAX_ARRAY_SIZE
};


inline void Print_Device_Info(cl_device_id device_id){
    unsigned int info;
    size_t size;
    size_t sizes[3];
    char version[500];
    cl_bool i_supp;
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(info), &info, NULL);
    printf("GPU INFO: compute units = %u\n", info);
    
    
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size), &size, NULL);
    printf("GPU INFO: max work group size = %lu\n", size);
    
    
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(info), &info, NULL);
    printf("GPU INFO: work item demsions = %u\n", info);
    
    
    clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(sizes), sizes, NULL);
    printf("GPU INFO: work item sizes = %lux %lu x %lu\n", sizes[0], sizes[1], sizes[2]);
    
    
    clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(size), &size, NULL);
    printf("GPU INFO: Global mem size = %lu MB\n", size/1000000);
    
    
    clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(size), &size, NULL);
    printf("GPU INFO: Global mem cache size = %lu\n", size);
    
    
    clGetDeviceInfo(device_id, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(size), &size, NULL);
    printf("GPU INFO: local mem size = %lu B\n", size);
    
    
    clGetDeviceInfo(device_id, CL_DEVICE_IMAGE_SUPPORT, sizeof(i_supp), &i_supp, NULL);
    printf("GPU INFO: Image Support = %s\n", i_supp ? "True" : "False");
    
    clGetDeviceInfo(device_id, CL_DEVICE_IMAGE_MAX_ARRAY_SIZE, sizeof(size), &size, NULL);
    printf("GPU INFO: Max image array = %lu\n", size);
    
    clGetDeviceInfo(device_id, CL_DRIVER_VERSION, sizeof(version), &version, NULL);
    printf("GPU INFO: %s\n", version);
    
    clGetDeviceInfo(device_id, CL_DEVICE_EXTENSIONS, sizeof(version), &version, NULL);
    printf("GPU INFO: %s\n", version);
    
    clGetDeviceInfo(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, sizeof(info), &info, NULL);
    printf("GPU INFO: Preferred Vector Width for char: %d\n", info);
    
    clGetDeviceInfo(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, sizeof(info), &info, NULL);
    printf("GPU INFO: Preferred Vector Width for int: %d\n", info);
    
    clGetDeviceInfo(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF, sizeof(info), &info, NULL);
    printf("GPU INFO: Preferred Vector Width for half: %d\n", info);
    
    clGetDeviceInfo(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, sizeof(info), &info, NULL);
    printf("GPU INFO: Preferred Vector Width for float: %d\n", info);
    
    clGetDeviceInfo(device_id, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, sizeof(info), &info, NULL);
    printf("GPU INFO: Preferred Vector Width for double: %d\n", info);
    
}

inline void Print_Kernel_Info(cl_kernel kernel, cl_device_id device){
    unsigned long priv;
    size_t size;
    size_t sizes[3];
   
    clGetKernelWorkGroupInfo(kernel, device, CL_KERNEL_GLOBAL_WORK_SIZE, sizeof(sizes), sizes, NULL);
    printf("Kernel Info: Global work group sizes = %lu X %lu X %lu\n", sizes[0], sizes[1], sizes[2]);
    
    
    clGetKernelWorkGroupInfo(kernel, device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size), &size, NULL);
    printf("Kernel Info: Kernel work group size = %lu\n", size);
    
    
    clGetKernelWorkGroupInfo(kernel, device, CL_KERNEL_COMPILE_WORK_GROUP_SIZE, sizeof(sizes), sizes, NULL);
    printf("Kernel Info: Compile work group sizes = %lu X %lu X %lu\n", sizes[0], sizes[1], sizes[2]);
    
    
    clGetKernelWorkGroupInfo(kernel, device, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(size), &size, NULL);
    printf("Kernel Info: Kernel perfered multiple = %lu\n", size);
    
    clGetKernelWorkGroupInfo(kernel, device, CL_KERNEL_LOCAL_MEM_SIZE, sizeof(priv), &priv, NULL);
    printf("Kernel Info: Local mem size = %lu\n", priv);
    
    clGetKernelWorkGroupInfo(kernel, device, CL_KERNEL_PRIVATE_MEM_SIZE, sizeof(priv), &priv, NULL);
    printf("Kernel Info: Private mem size = %lu\n", priv);
    
    
}




inline void GPU_INFO(){
    unsigned int err;
    cl_device_id device_id;
    err = clGetDeviceIDs(NULL,CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    Print_Device_Info(device_id);
}



#endif /* OpenCL_Funcs_h */
