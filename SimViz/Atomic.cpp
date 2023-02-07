//
//  Atomic.cpp
//  OpenGL
//
//  Created by Andrew Diggs on 8/31/22.
//

#include "Atomic.hpp"
#include "FileIO.hpp"
#include "Shapes.hpp"
#include "Object.hpp"
#include <thread>
#include <chrono>

int num_atoms;
AMD::Vec3 sim_box;

Atom::Atom()
: m_coords(AMD::Vec3()), m_type(0), m_num_neighbors(0)
{}

Atom::Atom(std::string line)
:m_num_neighbors(0)
{
    // LAMMPS dump looks like At.num At.type xs ys zs
    std::stringstream ss;
    float x, y, z;
    ss << line;
    ss >> m_id >> m_type >> x >> y >> z;
    m_coords = AMD::Vec3(x,y,z);
    
}

Atom::Atom(int _id, int _type, float x, float y, float z)
:m_id(_id), m_type(_type),m_coords(x, y, z)
{}

Atom::~Atom() { 
    //free(m_neighbors);
}


AMD::Vec3& Atom::get_coords(){
    return this -> m_coords;
}
unsigned int Atom::get_type() const{
    return m_type;
}

unsigned int Atom::get_id() const{
    return m_id;
}

void Atom::Rescale(){
    m_coords.x *= sim_box.x;
    m_coords.y *= sim_box.y;
    m_coords.z *= sim_box.z;
}

void Atom::Print(){
    std::cout << m_id << " " << m_type
    << " " << m_coords.x << " " << m_coords.y << " " << m_coords.z << std::endl;
}

//Bond class#####################

Bond::Bond(){}

Bond::Bond(Atom& A,Atom& B)
{
    m_start = A.get_coords();
    m_end = B.get_coords();
    m_vec = m_end - m_start;
    m_types.x = A.get_type();
    m_types.y = B.get_type();
    Set_Len();
    Set_Theta();
    Set_Phi();
    
    
}


Bond::~Bond() {}




void Bond::Set_Theta(){
    AMD::Vec3 z_hat = AMD::Vec3(0.0, 0.0, 1.0);
    m_ang.z = AMD::Get_angle(m_vec, z_hat);
    
}

void Bond::Set_Phi(){
    AMD::Vec3 y_hat = AMD::Vec3(0.0, -1.0, 0.0);
    AMD::Vec3 xy = AMD::Vec3(this->m_vec.x, this->m_vec.y, 0.0);
    float phi = AMD::Get_angle(xy, y_hat);
    if(this->m_vec.x < 0){
        m_ang.x = 6.283185307 - phi;
    }
    else{m_ang.x = phi;}
    m_ang.y = 0.0;
    return;
}

AMD::Vec3 Bond::get_off_set() {
    AMD::Vec3 temp = m_vec*0.5;
    return m_start + temp;
}

AMD::Vec3 &Bond::get_angles() {
    return this->m_ang;
}

AMD::Vec2 &Bond::get_types() {
    return this->m_types;
}

float Bond::get_len() { 
    return m_len;
}

void Bond::Set_Len() { 
    if((int)m_types.x == 2 && (int)m_types.y == 2){
        m_len = m_vec.len() - 1.5;
    }
    else{m_len = m_vec.len() - 0.8;}
    
    
}




Simulation::Simulation()
:curr_line(0), num_lines(0), init(false) {}

Simulation::Simulation(const char* file)
:curr_line(0), num_lines(0), init(false)
{
    m_data = Read_File(file, num_lines, block_len);
    Update_Sim('f');
    init = true;
    
}




Simulation::~Simulation()
{
    for (int i = 0; i < num_lines; i++){
        free(m_data[i]);
    }
    free(m_data);
}


void Simulation::Init_Sim(std::string file){
    
    m_data = Read_File(file.c_str(), num_lines, block_len);
    Update_Sim('f');
    init = true;
    
}
void Simulation::Set_Neighbors() { 
    float cut;
    int num_nebs;
    int count = 0;
    AMD::Vec3 diff;
    for (int i = 0; i< num_atoms; i++){
        num_nebs =0;
        for (int j = i+1; j< num_atoms; j++){
            diff = this->atoms[i].get_coords() - this->atoms[j].get_coords();
            switch (atoms[i].get_type() + atoms[j].get_type()) {
                case 2:
                    cut = cutoffs[0];
                    break;
                    
                case 3:
                    cut = cutoffs[1];
                    break;
                    
                case 4:
                    cut = cutoffs[2];
                    break;
                    
                default:
                    cut = 0.0;
                    break;
            }
            
            if(diff.len() <= cut){
                neighbor_IDs[count][0] = i; neighbor_IDs[count][1] = j;
                count++;
                num_nebs++;
            }
        
        }
    }
    this -> num_bonds = count;
}



void Simulation::Set_Block(int start){
    if(start + block_len > num_lines){
        return;
        
    }
    std::stringstream ss;
    m_timestep = atoi(m_data[start + 1]);
    num_atoms = atoi(m_data[start + 3]);
    int count = 0;
    AMD::Vec3 BB;
    float lo, hi;
    for(int i = start + 5; i< start + 8; i++){
        ss << m_data[i];
        ss >> lo >> hi;
        sim_box[count][0] = lo;
        sim_box[count][1] = hi;
        BB[count] = hi - lo;
        count++;
        ss.str(std::string());
        ss.clear();
        
    }
    count = 0;
    int _id, _type;
    float xs,ys,zs;
    for (int i = start + 9; i< start + block_len; i++){
        ss << m_data[i];
        ss >> _id >> _type >> xs >> ys >> zs;
        atoms[_id - 1] = Atom(_id, _type, xs*BB.x, ys*BB.y, zs*BB.z);
        count++;
        ss.str(std::string());
        ss.clear();
    }
    this -> curr_line = start + block_len;
}


void Simulation::Update_Sim(char dir){
    switch (dir) {
        case 'f':
            Set_Block(curr_line);
            break;
            
        case 'r':
            Set_Block(curr_line - 2*block_len);
            
        default:
            break;
    }
    Set_Neighbors();
}



bool Simulation::Is_Init(){
    return init;
}

int Simulation::Get_Timestep(){
    return m_timestep;
}

float **Simulation::Compute_Histogram() {
    int na = this-> num_atoms;
    int numx = ceil(this -> sim_box[0][1]);
    int numy = ceil(this -> sim_box[1][1]);
    
    AMD::Vec3 tmp;
    float** hist = (float**)malloc(numx*numy*sizeof(float*));
    for(int i = 0; i< numx*numy; i++){
        hist[i] = (float*)malloc(sizeof(float));
    }
    for(int i = 0; i< na; i++){
        tmp = this->atoms[i].get_coords();
        int idx = (int)tmp.x;
        int idy = (int)tmp.y;
    }
    
    
    
    return (float**)hist;
}




void Simulation::print(){
    for (int i = 0; i< block_len; i++){
        std::cout << m_data[i] << std::endl;
        std::cout << "=======================================" << std::endl;
    }
}


















