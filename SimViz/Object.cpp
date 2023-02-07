//
//  Object.cpp
//  SimViz
//
//  Created by Andrew Diggs on 1/3/23.
//

#include "Object.hpp"
#include "Atomic.hpp"

Shader init_Shader(std::string sh_file){
    return Shader(sh_file, false);
}

Atom_Mesh::Atom_Mesh()
:m_sh(m_shader_file, false),m_sh2(m_shader_file2, false), curr_timestep(0), instance_added(false)
{
    Sphere sp(1.0);
    VertexBuffer VBO(sp.verts, sp.num_verts()*sizeof(AMD::Vertex));
    m_VAO.Add_Vertex_Buffer(VBO);
    m_IBO.Gen_Buffer(sp.indices, sp.num_idx());
    m_num_idx = m_IBO.get_num();
}

Atom_Mesh::~Atom_Mesh()
{
    free(at_types);
    free(at_coords);
}

void Atom_Mesh::Add_Instance_Layout(Simulation& sim){
    
    this -> m_num_atoms = sim.num_atoms;
    AMD::Vec3 BB(sim.sim_box[0][1],sim.sim_box[1][1],sim.sim_box[2][1]);
    BB*=0.5;
    at_types = (float*)malloc(sim.num_atoms*sizeof(float));
    at_coords = (AMD::Vec3*)malloc(sim.num_atoms*sizeof(AMD::Vec3));
    for (int i = 0; i< m_num_atoms; i++){
        this->at_types[i] = sim.atoms[i].get_type();
        AMD::Vec3 tmp = sim.atoms[i].get_coords() - BB;
        this->at_coords[i] = tmp/5.43;
        
    }
    InstVBa = m_VAO.AddBuffer(at_coords,sizeof(AMD::Vec3), m_num_atoms);
    InstVBb = m_VAO.AddBuffer(at_types,sizeof(float), m_num_atoms);
    instance_added = true;
}

void Atom_Mesh::Bind(int num) const {
    if(num == 1){
        this->m_sh.bind();
    }
    else if (num==2){
        this->m_sh2.bind();
        return;
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
        return;
    }
    this->m_VAO.unbind();
    this->m_IBO.unbind();
}

void Atom_Mesh::Set_OPs(Operator& op, Light_Src& src, Simulation& sim){
    this -> m_sh.Set_Uniforms(op, src);
    this -> m_sh2.Set_Uniforms(op, src);
    
    if(!instance_added && sim.Is_Init()){Add_Instance_Layout(sim);}
    else if (curr_timestep != sim.Get_Timestep() && sim.Is_Init()){
        AMD::Vec3 BB(sim.sim_box[0][1],sim.sim_box[1][1],sim.sim_box[2][1]);
        BB*=0.5;
        for (int i = 0; i< m_num_atoms; i++){
            this->at_types[i] = sim.atoms[i].get_type();
            AMD::Vec3 tmp = sim.atoms[i].get_coords() - BB;
            this->at_coords[i] = tmp/5.43;
            
        }
    }
    
}

void Atom_Mesh::Draw() const {
    
    glBindBuffer(GL_ARRAY_BUFFER,InstVBa);
    glBufferData(GL_ARRAY_BUFFER, m_num_atoms*sizeof(AMD::Vec3), (void*)at_coords, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,InstVBb);
    glBufferData(GL_ARRAY_BUFFER, m_num_atoms*sizeof(float), (void*)at_types, GL_DYNAMIC_DRAW);
    
    m_VAO.bind();
    m_IBO.bind();
    m_sh.bind();
    glDrawElementsInstanced(GL_TRIANGLES,m_num_idx, GL_UNSIGNED_INT,0, m_num_atoms);
    m_sh2.bind();
    glDrawElementsInstanced(GL_LINES,m_num_idx, GL_UNSIGNED_INT,0, m_num_atoms);
    
    m_VAO.unbind();
    m_IBO.unbind();
}






