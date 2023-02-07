//
//  Object.hpp
//  SimViz
//
//  Created by Andrew Diggs on 1/3/23.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include <string>
#include "vertexbuffer.hpp"
#include "vertexarray.hpp"
#include "shader.hpp"
#include "Shapes.hpp"
class Operator;
class Light_Src;
class Simulation;
Shader init_Shader(std::string sh_file);

class Atom_Mesh{
private:
    //AMD::Vec3 at_coords[3000];
    AMD::Vec3* at_coords;
    float* at_types;
    int m_num_atoms;
    unsigned int m_num_idx;
    VertexArray m_VAO;
    IndexBuffer m_IBO;
    const std::string m_shader_file = "/Users/diggs/Desktop/SimViz/SimViz/Shaders/atom.fs";
    const std::string m_shader_file2 = "/Users/diggs/Desktop/SimViz/SimViz/Shaders/atom_2.fs";
    Shader m_sh;
    Shader m_sh2;
    unsigned int InstVB[2];
    unsigned int InstVBa;
    unsigned int InstVBb;
    unsigned int curr_timestep;
    bool instance_added;
public:
    Atom_Mesh();
    ~Atom_Mesh();
    void Add_Instance_Layout(Simulation& sim);
    void Bind(int num) const;
    void unBind(int num) const;
    void Draw() const;
    void Set_OPs(Operator& op, Light_Src& src, Simulation& sim);
};


#endif /* Object_hpp */
