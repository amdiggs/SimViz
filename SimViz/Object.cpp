//
//  Object.cpp
//  SimViz
//
//  Created by Andrew Diggs on 1/3/23.
//

#include "Object.hpp"
#include "Atomic.hpp"

Shader init_Shader(std::string sh_file){
    return Shader(sh_file);
}

Atom_Mesh::Atom_Mesh()
:m_sh(m_shader_file), m_sh2(m_shader_file2), num_draws(500)
{
    Sphere sp(1.0);
    VertexBuffer VBO(sp.verts, sp.num_verts()*sizeof(AMD::Vertex));
    m_VAO.Add_Vertex_Buffer(VBO);
    m_IBO.Gen_Buffer(sp.indices, sp.num_idx());
    m_num_idx = m_IBO.get_num();
}

Atom_Mesh::~Atom_Mesh() {}

void Atom_Mesh::Add_Instance_Layout(Simulation& sim){
    
    this -> m_num_atoms = sim.m_num_atoms;
    for (int i = 0; i< m_num_atoms; i++){
        this->at_types[i] = sim.m_atoms[i].get_type();
        this->at_coords[i] = sim.m_atoms[i].get_coords();
        
    }
    InstVB[0] = m_VAO.AddBuffer(at_coords,sizeof(AMD::Vec3), num_draws);
    InstVB[1] = m_VAO.AddBuffer(at_types,sizeof(float), num_draws);
    
}

void Atom_Mesh::Bind(int num) const {
    if(num == 1){
        this->m_sh.bind();
    }
    else if (num==2){
        this->m_sh2.bind();
    }
    this->m_VAO.bind();
    this->m_IBO.bind();
}

void Atom_Mesh::unBind(int num) const {
    if(num == 1){
        this->m_sh.unbind();
    }
    else if (num==2){
        this->m_sh2.unbind();
    }
    this->m_VAO.unbind();
    this->m_IBO.unbind();
}

void Atom_Mesh::Set_OPs(Operator& op, Light_Src& src, int num){
    this -> m_sh.Set_Uniforms(op, src);
    this -> m_sh2.Set_Uniforms(op, src);
}

void Atom_Mesh::Draw() const {
    //glBindBuffer(GL_ARRAY_BUFFER,InstVB[0]);
    //glBufferData(GL_ARRAY_BUFFER, num_draws*sizeof(AMD::Vec3), (void*)at_coords, GL_DYNAMIC_DRAW);
    //
    //glBindBuffer(GL_ARRAY_BUFFER,InstVB[1]);
    //glBufferData(GL_ARRAY_BUFFER, num_draws*sizeof(float), (void*)at_types, GL_DYNAMIC_DRAW);
    m_VAO.bind();
    m_IBO.bind();
    //Bind(1);
    m_sh.bind();
    glDrawElementsInstanced(GL_TRIANGLES,m_num_idx, GL_UNSIGNED_INT,0, num_draws);
    //Bind(2);
    //m_sh2.bind();
    //glDrawElementsInstanced(GL_TRIANGLES,m_num_idx, GL_UNSIGNED_INT,0, num_draws);
    
    m_VAO.unbind();
    m_IBO.unbind();
}






