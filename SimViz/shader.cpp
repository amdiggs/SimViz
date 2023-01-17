//
//  shader.cpp
//  OpenGL
//
//  Created by Andrew Diggs on 7/28/22.
//

#include "shader.hpp"
#include "Shapes.hpp"
#include "Operations.hpp"



Shader::Shader(std::string file_name){
    std::string vertexShader = get_sh_str("VERTEX", file_name);
    std::string geometryShader = get_sh_str("GEOMETRY",file_name);
    std::string fragmentShader = get_sh_str("FRAGMENT", file_name);
    CreateShader(vertexShader, geometryShader, fragmentShader);
    

    MV_loc = UniformLoc("u_MV");
    MVP_loc = UniformLoc("u_MVP");
    N_loc = UniformLoc("u_Normal");
    
    
    light_src_loc = UniformLoc("l_src");
    light_clr_loc = UniformLoc("l_color");
    
    
    
    
    
}


Shader::~Shader(){
    glDeleteProgram(m_ID);
}



void Shader::bind() const {
    glUseProgram(m_ID);
}

void Shader::unbind() const {
    glUseProgram(0);
}





unsigned int Shader::CompileShader(unsigned int type, const std::string& source){
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << std::endl;
    }
    
    return id;
    
    
}
void Shader::CreateShader(const std::string& vertexShader,const std::string& geometryShader, const std::string& fragmentShader){
    m_ID = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(m_ID, vs);
    glAttachShader(m_ID, gs);
    glAttachShader(m_ID, fs);
    glLinkProgram(m_ID);
    glValidateProgram(m_ID);
    
    glDeleteShader(vs);
    glDeleteShader(gs);
    glDeleteShader(fs);
    
    return;
    
    
}


int Shader::UniformLoc(const char* name) const{
    int loc = glGetUniformLocation(m_ID, name);
    if(loc == -1){
        std::cout << "Uniform not found" << std::endl;
        exit(2);
    }
    else{return loc;}
}



bool comment(std::string str, std::string reg_ex){
    std::regex reg(reg_ex);
    std::smatch m;
    std::regex_search(str, m, reg);
    return !m.empty();
};



std::string Shader::get_sh_str(std::string sh_type, std::string sh_file){
    std::ifstream in_file;
    std::string shader_string;
    in_file.open(sh_file);
    if (!in_file.is_open()){
        std::cout << "ERROR File Did Not Open!!" << std::endl;
    }
    std::string line;
    while (getline(in_file, line)) {
        if (comment(line, "#SHADER " + sh_type)){
            while (getline(in_file, line)){
                if (!line.empty()){
                    if (comment(line, "#END")) {
                        break;
                    }
                    else{shader_string.append(line + "\n");}
                }
            }
        }
    }
    return shader_string;
}





void Shader::Set_Uniforms(Operator& op, Light_Src& src){
    bind();
    op.set_Model();
    op.set_View();
    op.set_projection();
    op.set_MV();
    op.set_MVP();
    
    
    Set_Value('M', op.MV_ptr);
    Set_Value('N', op.rot_ptr);
    Set_Value('P', op.MVP_ptr);
    
    
    Set_Value('s', src.get_light_src());
    Set_Value('z', src.get_light_clr());
    //Set_Value('x', src.get_light_sat());
    
}

void Shader::Set_Value(char type, const float* mat_ptr) const {
    switch (type) {
            
        case 'M':
            glUniformMatrix4fv(MV_loc,1,GL_TRUE, mat_ptr);
            break;
            
        case 'N':
            glUniformMatrix4fv(N_loc,1,GL_TRUE, mat_ptr);
            break;
            
        case 'P':
            glUniformMatrix4fv(MVP_loc,1,GL_TRUE, mat_ptr);
            break;
            
        default:
            break;
    }
}

void Shader::Set_Value(char type, const AMD::Vec2& vec) const {
    glUniform2f(light_src_loc, vec.x, vec.y);
}


void Shader::Set_Value(char type, const AMD::Vec3& vec) const {
    switch (type) {
        case 's':
            glUniform3f(light_src_loc, vec.x, vec.y, vec.z);
            break;
        case 'c':
            glUniform3f( light_clr_loc, vec.x, vec.y, vec.z);
            break;
         
        default:
            break;
    }
}


void Shader::Set_Value(char type, const AMD::Vec4& vec) const {
    glUniform4fv(light_clr_loc,1,(float*) &vec);
}


void Shader::Set_Value(char type, const float& f) const {
    glUniform1f(sat_loc,f);
}

void Shader::Set_Uniform(const char *name, const AMD::Vec3 *vec) const {
    int loc = UniformLoc(name);
    glUniform3fv(loc,1,(float*)vec);
}

void Shader::Set_Uniform(const char *name, const AMD::Vec3 vec[], int count) const {
    int loc = UniformLoc(name);
    glUniform3fv(loc,count,(float*)&vec[0]);
}

void Shader::Set_Uniform(const char* name, const int typs[], int count) const{
    int loc = UniformLoc(name);
    glUniform1iv(loc,count,(int*)&typs[0]);
}



