//
//  Atomic.cpp
//  OpenGL
//
//  Created by Andrew Diggs on 8/31/22.
//

#include "Atomic.hpp"
#include "FileIO.hpp"
#include "Shapes.hpp"
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




Atom* atoms(std::string file){
    std::ifstream infile (file);
    std::string line;
    std::string item_type;
    std::string item;
    std::stringstream ss, ss2;
    std::string temp;
    std::string temp2;
    num_atoms = 0;
    Atom* local_atoms = nullptr;
    if (infile.is_open()) {
        while (getline(infile,line)) {
            if (ITEM(line, "ITEM:")){
                ss << line;
                ss >> item >> item_type;
                if (ITEM(item_type, "NUMBER")){
                    getline(infile, line);
                    num_atoms = atoi(line.c_str());
                    local_atoms = (Atom*)malloc(num_atoms*sizeof(Atom));
                    ss.str(std::string());
                    ss.clear();
                    continue;
                }
                else if (ITEM(item_type, "BOX")){
                    float low, high;
                    std::stringstream bbss;
                    for (int i = 0; i<3; i++){
                        getline(infile, line);
                        bbss << line;
                        bbss >> low >> high;
                        sim_box[i] = high;
                        bbss.str(std::string());
                        bbss.clear();
                        
                    }
                    ss.str(std::string());
                    ss.clear();
                    continue;
                }
                else if (ITEM(item_type, "ATOMS")){
                    for (int i = 0; i<num_atoms; i++){
                        getline(infile,line);
                        local_atoms[i] = Atom(line);
                        
                    }
                    ss.str(std::string());
                    ss.clear();
                }
                
                ss.str(std::string());
                ss.clear();
            }
        }
    }
    
    return local_atoms;
}

















Simulation::Simulation()
:curr_block(0), num_blocks(0), num_bonds(0) {}

Simulation::~Simulation()
{
    free(m_data);
}

void Simulation::Set_Neighbors() { 
    float cut;
    int num_nebs;
    int count = 0;
    AMD::Vec3 diff;
    for (int i = 0; i< num_atoms; i++){
        num_nebs =0;
        for (int j = i+1; j< num_atoms; j++){
            diff = this->m_atoms[i].get_coords() - this->m_atoms[j].get_coords();
            if(this->m_atoms[i].get_type() == 1 && this->m_atoms[j].get_type() == 2 ){cut = cutoffs[0];}
            else if(this->m_atoms[i].get_type() == 2 && this->m_atoms[j].get_type() == 2 ){cut = cutoffs[1];}
            else if(this->m_atoms[i].get_type() == 1 && this->m_atoms[j].get_type() == 1 ){cut = cutoffs[2];}
            else{cut = 35.7;}
            if(diff.len() <= cut){
                neighbor_IDs[count][0] = i; neighbor_IDs[count][1] = j;
                count++;
                num_nebs++;
            }
        
        }
    }
    this -> num_bonds = count;
}

void Simulation::Set_Num_Blocks(int num){
    this -> num_blocks = num;
}


void Simulation::Set_Blocks(char** dat){
    int nb = this->num_blocks;
    int curr_line = 0;
    if(nb == 0){
        std::cout << "Number of data blocks not set" << std::endl;
        exit(9);
    }
    this -> m_data = (Sim_Block*)malloc(nb*sizeof(Sim_Block));
    for(int i = 0; i < nb; i++){
        this->m_data[i] = read_block(dat, curr_line);
        
    }
    this -> m_timestep = this -> m_data[0].timestep;
    this -> m_num_atoms = this -> m_data[0].num_atoms;
    for(int i = 0; i<3; i++){
        this -> m_sim_box[i][0] = this -> m_data[0].sim_box[i][0];
        this -> m_sim_box[i][1] = this -> m_data[0].sim_box[i][1];
    }
}


void Simulation::Update_Sim(int block_num){
    this -> m_timestep = this -> m_data[block_num].timestep;
    for(int i =0; i<this -> m_num_atoms; i++){
        Atom test = this -> m_data[block_num].atoms[i];
        this -> m_atoms[i] = this -> m_data[block_num].atoms[i];
    
    }
    this -> Set_Neighbors();
}

float **Simulation::Compute_Histogram() {
    int na = this->m_num_atoms;
    int numx = ceil(this -> m_sim_box[0][1]);
    int numy = ceil(this -> m_sim_box[1][1]);
    
    AMD::Vec3 tmp;
    float** hist = (float**)malloc(numx*numy*sizeof(float*));
    for(int i = 0; i< numx*numy; i++){
        hist[i] = (float*)malloc(sizeof(float));
    }
    for(int i = 0; i< na; i++){
        tmp = this->m_atoms[i].get_coords();
        int idx = (int)tmp.x;
        int idy = (int)tmp.y;
    }
    
    
    
    return (float**)hist;
}























