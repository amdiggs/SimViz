//
//  Operations.cpp
//  OpenGL
//
//  Created by Andrew Diggs on 8/3/22.
//

#include "Operations.hpp"




Operator::Operator()
{
    
    Proj_vec = AMD::Vec4(3.0, 3.0, 1.0, 10.0);
    M_scale = 1.25;
    M_Scale_vec = AMD::Vec3(M_scale,M_scale,M_scale);
    M_Trans_vec.z = 3.5;
    
}



Operator::~Operator(){}


void Operator::Set_W_Scale(int w, int h){
    w_scale = (float)h / (float)w;
}


void Operator::Reset(AMD::Mat4& mat){
    
    for (int i = 0; i< 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j){
                mat[i][j] = 1.0;
            }
            else{mat[i][j] = 0.0;}
        }
    }
    
}




void Operator::Projection(AMD::Vec4 vec){
    float near = vec.b;
    float far = vec.a;
    float dz = abs(far - near);
    float yalpha = vec.g / (2.0 * dz);
    float xalpha = vec.r/(2.0 * dz);
    
    
    Proj_mat[0][0] = near;
    Proj_mat[1][1] = near;
    Proj_mat[2][2] = (far + near) / (far - near);
    Proj_mat[2][3] = (-2.0*far*near) / (far - near);
    Proj_mat[3][2] = 1.0;
    Proj_mat[3][3] = 0.0;
    
    
    
}



//Model Matrix


void Operator::M_set_rotation() {
    Normal_mat.Transpose();
    M_Rot_mat.Rotate(M_rotation_vec);
    Normal_mat.Rotate(M_rotation_vec);
    M_rotation_vec.Reset();
    Normal_mat.Transpose();

}


AMD::Vec3& Operator::M_Get_Trans(){
    return M_Trans_vec;
}


float& Operator::M_Get_Scale(){
    return M_scale;
}

void Operator::M_Set_Scale(){
    M_Scale_vec = AMD::Vec3(M_scale,M_scale,M_scale);
}

void Operator::set_Model(){
    Reset(M_mat);
    M_Set_Scale();
    M_mat.Scale(M_Scale_vec);
    M_mat = M_Rot_mat * M_mat;
    M_mat.Translate(M_Trans_vec);
}



//#############################################################
void Operator::V_set_rotation(float ax, float ay, float az){
    V_mat.Rotate(V_rotation);
}

void Operator::V_set_rotation(){
    V_mat.Translate(V_translation);
    V_mat.Rotate(V_rotation);
    
}

void Operator::V_set_translation(AMD::Vec3 tr){
    V_mat.Translate(tr);
}

void Operator::V_set_translation(){
    V_mat.Translate(V_translation);
}


void Operator::V_set_scale(float sc){
    V_mat.Scale(AMD::Vec3(w_scale*sc,sc,sc));
}

void Operator::set_View(){
    Reset(V_mat);
    V_set_translation();
    V_set_scale(V_scale);
    
    
}

float* Operator::get_V_scale(){
    return &V_scale;
}


void Operator::set_MV(){
    
    MV_mat = V_mat * M_mat;
    MV_mat.Transpose();
}

void Operator::set_projection() {
    Projection(Proj_vec);
}


AMD::Vec4& Operator::get_proj_vec(){
    return this->Proj_vec;
}


void Operator::set_MVP(){
    
    MVP_mat = Proj_mat *V_mat * M_mat;
    MVP_mat.Transpose();
    
}

AMD::Mat4 Operator::Get_MVP() const{
    return MVP_mat;
}



AMD::Mat4 Operator::Get_MV() const{
    return MV_mat;
}







/*
 v1 + v' = v2; v' points v1->v2
 v' = v2 - v1; v' points v1->v2
 => v2 - v1 points v1->v2
 */




Light_Src::Light_Src()
:light_src_dir(AMD::Vec3(0.0,1.0,0.0)),light_color(AMD::Vec4(0.6, 0.8, 0.5, 0.1))
{}

Light_Src::~Light_Src(){}


void Light_Src::set_light_dir(AMD::Vec3 src) { 
    light_src_dir = src;
}

void Light_Src::set_light_dir(float theta, float phi) { 
    light_src_dir.x = cos(phi)*sin(theta);
    light_src_dir.z = sin(phi)*sin(theta);
    light_src_dir.y = cos(theta);
}

void Light_Src::set_light_color(AMD::Vec4 l_clr) { 
    light_color = l_clr;
}

void Light_Src::set_sat(float val) {
    sat= val;
}

AMD::Vec3 &Light_Src::get_light_src() { 
    return this->light_src_dir;
}

AMD::Vec4 &Light_Src::get_light_clr() { 

    return this->light_color;
}

float &Light_Src::get_light_sat() { 
    return this->sat;
}



float* Light_Src::get_src_ptr(){
    return light_src_dir.get();
}

float* Light_Src::get_clr_ptr(){
    return light_color.get();
}

float* Light_Src::get_sat_ptr(){
    return &sat;
}
