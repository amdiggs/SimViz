//
//  shader.hpp
//  OpenGL
//
//  Created by Andrew Diggs on 7/28/22.
//

#ifndef shader_hpp
#define shader_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "OGL.h"
#include "AMDmath.hpp"
class Operator;
class Light_Src;

class Shader{
private:
    
    unsigned int m_ID;
    unsigned int CompileShader(unsigned int type, const std::string& source);
    void CreateShader(const std::string& vertexShader,const std::string& geometryShader, const std::string& fragmentShader);
    std::string get_sh_str(std::string sh_file);
    std::string get_sh_str(std::string sh_type, std::string sh_file);
    
    int displace_loc, MV_loc, MVP_loc, N_loc;
    
     int light_src_loc, light_clr_loc, sat_loc;
    

    
public:
    Shader(std::string file_name);
    ~Shader();
    
    void bind();
    void unbind();
    int UniformLoc(const char* name) const;
    void Get_Uniforms(int texc, const char* texv[]);
    
    void Set_Uniforms(Operator& op, Light_Src src);
    void Set_Value(char type, const float* mat_ptr) const;
    void Set_Value(char type, const AMD::Vec3& vec) const;
    void Set_Value(char type, const AMD::Vec4& vec) const;
    void Set_Value(char type, const AMD::Vec2& vec) const;
    void Set_Value(char type, const float& f) const;
    void Set_Value(char type, const int& a) const;
    
    
    
    
};

bool comment(std::string str, std::string reg_ex);
#endif /* shader_hpp */
