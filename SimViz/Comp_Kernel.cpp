//
//  Comp_Kernel.cpp
//  Computes_Acc
//
//  Created by Andrew Diggs on 4/7/23.
//

#include "Comp_Kernel.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <regex>




const char* s_dig = "[[:digit:]]+";
const char* s_float = "[[:digit:]]+\\.[[:digit:]]+";
const char* s_space = "[[:blank:]]";
const char* s_exp_float = "[[:digit:]]+\\.[[:digit:]]+e\\+[[:digit:]]+";
const char* s_comment = "^#.*";
const char* arg_list = "";





static bool comment(std::string str, std::string reg_ex){
    std::regex reg(reg_ex);
    std::smatch m;
    std::regex_search(str, m, reg);
    return !m.empty();
};

template <typename T>
void CL_Error_Check(T err, const char* step){
    if (!err)
    {
        printf("Error: Failed to %s!\n", step);
        exit(1);
    }
}


std::string Get_Kernel_String(std::string cl_file){
    std::ifstream in_file;
    std::string shader_string;
    in_file.open(cl_file);
    if (!in_file.is_open()){
        std::cout << "ERROR Loading Shader file!!" << std::endl;
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


static unsigned int Hash(const char* word){
    unsigned int hash_int = 0;
    for (int i = 0; i<std::strlen(word); i++){
        hash_int+=word[i]*word[i];
    }
    return hash_int % NUM_KERNELS;
}




CL_Program::CL_Program(const char* file)
{   std::string cl_file = m_folder + file;
    std::string kernel_src = Get_Kernel_String(cl_file);
    m_program = Create_Program(kernel_src.c_str());
}

CL_Program::~CL_Program() {
     clReleaseProgram(m_program);
     clReleaseCommandQueue(m_queue);
     clReleaseContext(m_context);
    
    
}

cl_program CL_Program::Create_Program(const char* kernel_src) {
    int err;
    unsigned int num_dev = 0;
    err = clGetDeviceIDs(NULL,CL_DEVICE_TYPE_GPU, 1, &m_devices, &num_dev);
    if (err)
    {
        printf("Error: Failed to create a device group!\n");
        exit(1);
    }
  
    // Create a compute context
    //
    m_context = clCreateContext(0, 1, &m_devices, NULL, NULL, &err);
    if (!m_context)
    {
        printf("Error: Failed to create a compute context!\n");
        exit(1);
    }
 
    // Create a command commands
    //
    m_queue = clCreateCommandQueue(m_context, m_devices, 0, &err);
    if (!m_queue)
    {
        printf("Error: Failed to create a command commands!\n");
        exit(1);
    }
    // Create the compute program from the source buffer
    //
    cl_program program = clCreateProgramWithSource(m_context, 1, (const char **) &kernel_src, NULL, &err);
    if (!program)
    {
        printf("Error: Failed to create compute program!\n");
        exit(1);
    }
 
    // Build the program executable
    //
    err = clBuildProgram(program, 0, NULL, "-I.", NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];
 
        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, m_devices, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
    }
    return program;
}


CL_Kernel CL_Program::Create_Kernel(const char* func_name){
    int error = 0;
    unsigned int table_ID = Hash(func_name);
    CL_Kernel ret;
    ret.m_prog = this;
    cl_kernel ker = clCreateKernel(m_program, func_name, &error);
    
    if (!ker || error != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }
    ret.m_kernel = ker;
    return ret;
}




CL_Kernel::CL_Kernel(){}
CL_Kernel::~CL_Kernel()
{
    for (int i = 0; i < num_imput_buffers; i++){
        clReleaseMemObject(m_inputs[i]);
    }
    clReleaseMemObject(m_output);
    clReleaseKernel(m_kernel);
}




void CL_Kernel::alloc_input(size_t dat_size){
    m_inputs[num_imput_buffers] = clCreateBuffer(m_prog->m_context, CL_MEM_READ_ONLY, dat_size, NULL, NULL);
    if (!m_inputs[num_imput_buffers])
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }
    num_imput_buffers++;
}


void CL_Kernel::alloc_output(size_t dat_size){
    m_output = clCreateBuffer(m_prog->m_context, CL_MEM_WRITE_ONLY, dat_size, NULL, NULL);
    if (!m_output)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }
}


void CL_Kernel::set_buffer_data(size_t size, void* dat){
    int idx = num_imput_buffers - 1;
    int error = clEnqueueWriteBuffer(m_prog->m_queue, m_inputs[idx], CL_TRUE, 0, size, dat, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array!\n");
        exit(1);
    }
 
}



void CL_Kernel::set_arg_internal(size_t type_size, void* arg_ptr){
    int error;
    error  = clSetKernelArg(m_kernel, curr_arg, type_size, arg_ptr);
    if (error != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", curr_arg);
        exit(1);
    }
}









void CL_Kernel::Set_Arg(Arg_Type typ, size_t size, void* arg){
    int idx = num_imput_buffers;
    switch (typ) {
        case Input:
            alloc_input(size);
            set_buffer_data(size, arg);
            set_arg_internal(sizeof(cl_mem), &m_inputs[idx]);
            curr_arg++;
            break;
            
        case Output:
            alloc_output(size);
            set_arg_internal(sizeof(cl_mem), &m_output);
            curr_arg++;
            break;
            
        case Constant:
            set_arg_internal(size, arg);
            curr_arg++;
            break;
            
        default:
            printf("ERROR: Did not set Arg type.\n");
            exit(1);
            break;
    }
    
}





void CL_Kernel::Read_Output(size_t size,void* output){
    int error = clEnqueueReadBuffer(m_prog->m_queue, m_output, CL_TRUE, 0, size, output, 0, NULL, NULL);
    if (error != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", error);
        exit(1);
    }
    
}



void CL_Kernel::Set_Global_Dimensions(size_t* glob){
    int tmp = 0;
    for(int i = 0; i<3; i++){
        global_dimensions[i] = glob[i];
        if(glob[i] > 0){ tmp++;}
        
    }
    num_dimensions = tmp;
}

void CL_Kernel::Compute(){
    
    int error = clEnqueueNDRangeKernel(m_prog->m_queue, m_kernel, num_dimensions, NULL, global_dimensions, NULL, 0, NULL, NULL);
    if (error)
    {
        printf("Error: Failed to execute kernel!\n");
        exit(1);
    }
    
    clFinish(m_prog->m_queue);
    
}




