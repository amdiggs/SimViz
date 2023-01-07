//
//  Operations.hpp
//  OpenGL
//
//  Created by Andrew Diggs on 8/3/22.
//

#ifndef Operations_hpp
#define Operations_hpp

#include <stdio.h>
#include <cmath>
#include "shader.hpp"
#include "AMDmath.hpp"

class Shader;
//struct color;

class Operator{
public:
    float w_scale = 9.0/16.0;
    void Reset(AMD::Mat4& mat);

    
    //Model Matrix
    AMD::Mat4 M_mat;
    const float* M_ptr = &M_mat.m[0][0];
    float M_scale =1.0;
    AMD::Vec3 M_Scale_vec;
    AMD::Vec3 M_Trans_vec;
    AMD::Vec3 M_rotation_vec;
    AMD::Mat4 M_Rot_mat;
    AMD::Mat4 M_TSc_mat;
    
    //Normal Operator
    AMD::Mat4 Normal_mat;
    const float* rot_ptr = &Normal_mat.m[0][0];
    AMD::Vec3 N_rot_vec;
    

    // View Matrix
        
    AMD::Mat4 V_mat;
    const float* V_ptr = &V_mat.m[0][0];
    int V_loc;
    AMD::Vec3 V_translation;
    AMD::Vec3 V_rotation;
    float V_scale = 1.0;
    
    AMD::Mat4 MV_mat;
    const float* MV_ptr = &MV_mat.m[0][0];
    
    
    //Projection Matrix
    AMD::Mat4 Proj_mat;
    const float* Proj_ptr = &Proj_mat.m[0][0];
    int Proj_loc;
    AMD::Vec4 Proj_vec;
    
    
    AMD::Mat4 MVP_mat;
    const float* MVP_ptr = &MVP_mat.m[0][0];
    

    
//public:
    Operator();
    ~Operator();
    
    void Set_W_Scale(int w, int h);
    
    
    void M_set_rotation(AMD::Vec3 vec);
    void M_set_rotation();
    AMD::Vec3& M_Get_Trans();
    float& M_Get_Scale();
    void M_Set_Scale();
    void set_Model() ;
    
    
    
    void V_set_rotation(float ax, float ay, float az);
    void V_set_rotation();
    void V_set_translation(AMD::Vec3 tr);
    void V_set_translation();
    void V_set_scale(AMD::Vec4);
    void V_set_scale(float sc);
    float* get_V_scale();
    void set_View() ;

    void Projection(AMD::Vec4 vec);
    void set_projection();
    AMD::Vec4& get_proj_vec();
    
    
    void set_MV();
    void set_MVP();
    AMD::Mat4 Get_MV() const;
    AMD::Mat4 Get_MVP() const;
};


class Light_Src{
private:

    AMD::Vec3 light_src_dir;
    int src_loc;
    
    AMD::Vec4 light_color;
    int light_clr_loc;
    
    float sat =1.0;
    int sat_loc;
    
    
public:
    Light_Src();
    ~Light_Src();
    void set_light_dir(AMD::Vec3 src);
    void set_light_dir(float theta, float phi);
    void set_light_color(AMD::Vec4 l_clr);
    void set_sat(float);
    
    AMD::Vec3& get_light_src();
    AMD::Vec4& get_light_clr();
    float& get_light_sat();
    
    float* get_src_ptr();
    float* get_clr_ptr();
    float* get_sat_ptr();
    
    
    
};



#endif /* Operations_hpp */
