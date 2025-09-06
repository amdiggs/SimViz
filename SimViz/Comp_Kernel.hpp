//
//  Comp_Kernel.hpp
//  Computes_Acc
//
//  Created by Andrew Diggs on 4/7/23.
//

#ifndef Comp_Kernel_hpp
#define Comp_Kernel_hpp

#include <stdio.h>
#include <string>
#include <cmath>
#include "AMDmath.hpp"
#include <OpenCL/opencl.h>
#define NUM_KERNELS (10)
#define MAX_INPUT_BUFFERS (5)

enum Arg_Type {Input, Output, Constant};

class CL_Kernel;

class CL_Program{
private:
    const std::string m_folder = "/Users/diggs/Desktop/VolumeData/Resources/Shaders/";
    cl_device_id m_devices;
    cl_context m_context;
    cl_command_queue m_queue;
    cl_program m_program;
   
    
    cl_program Create_Program(const char* kernel_src);
    
    friend class CL_Kernel;
    
    
public:
    CL_Program(const char* file);
    ~CL_Program();
    
    CL_Kernel Create_Kernel(const char* ker_name);
  
};



class CL_Kernel{
private:
    
    CL_Program* m_prog = NULL;
    cl_kernel m_kernel;
    cl_mem m_inputs[MAX_INPUT_BUFFERS];
    cl_mem m_output;
    size_t global_dimensions[3];
    int num_dimensions;
    unsigned int num_imput_buffers = 0;
    unsigned int curr_arg = 0;
        
    void alloc_input(size_t dat_size);
    void alloc_output(size_t out_size);
    void set_arg_internal(size_t type_size, void* arg_ptr);
    void set_buffer_data(size_t size, void* arg_ptr);
    CL_Kernel();
    friend class CL_Program;
    
public:
    
    ~CL_Kernel();
    
    void Set_Global_Dimensions(size_t a, size_t b, size_t c);
    void Set_Global_Dimensions(size_t* glob);

    
    
    void Set_Arg(Arg_Type, size_t size, void*);
    void Set_Arg(Arg_Type, size_t size, float);
    void Set_Arg(Arg_Type, size_t size, int);
    
    void Compute();
    void Read_Output(size_t size,void* output);
    
    
    
};



#endif /* Comp_Kernel_hpp */

