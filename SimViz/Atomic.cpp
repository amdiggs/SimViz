//
//  Atomic.cpp
//  OpenGL
//
//  Created by Andrew Diggs on 8/31/22.
//

#include "Atomic.hpp"
#include "FileIO.hpp"
#include "AtomInfo.h"
#include "Meshes.hpp"
Simulation* Sim = Simulation::Get();
extern Dump_Arr* data;

float Boundary_Wrapped_Dist(AMD::Vec3 A, AMD::Vec3 B){
    AMD::Vec3 box = Sim->Sim_Box();
    float dist_sq = 0.0;
    for (int i = 0; i< 3; i++){
        float delta = abs(A[i]-B[i]);
        if(delta > 0.5*box[i]){
            float x1 = (A[i] < B[i]) ? A[i] : B[i];
            float x2 = (A[i] < B[i]) ? B[i] : A[i];
            dist_sq += ((x1 + box[i]) - x2) * ((x1 + box[i]) - x2);
        }
        else{dist_sq += delta * delta;}
    }
    
    return sqrt(dist_sq);
}

float Dist(AMD::Vec3 A, AMD::Vec3 B){
    float dx = A.x - B.x;
    float dy = A.y - B.y;
    float dz = A.z - B.z;
    float dist_sq = dx*dx + dy*dy + dz*dz;
    
    return sqrt(dist_sq);
}

float Comp_Bond_Length(Atom& A, Atom& B){
    atom_info ia = Get_Atom_Info(A.Get_Type());
    atom_info ib = Get_Atom_Info(B.Get_Type());
    float ra = ia.rad;
    float rb = ib.rad;
    return 1.25*(ra + rb);
}

Atom::Atom()
: m_id(10000000), m_type(0), m_coords(0.0,0.0,0.0), m_num_neighbors(0)
{}

Atom::Atom(Atom_Line al)
: m_num_neighbors(0)
{
    m_id = al.id;
    m_type = al.type;
    m_coords = al.coords;

}

Atom::Atom(int _id, int _type, float x, float y, float z)
:m_id(_id), m_type(_type),m_coords(x, y, z), m_num_neighbors(0)
{}

Atom& Atom::operator=(const Atom& other){
    if(this == &other){return *this;}
    
    this->m_id = other.m_id;
    this->m_type = other.m_type;
    this->m_coords = other.m_coords;
    this->m_num_neighbors = other.m_num_neighbors;
    
    this->m_neighbors[0] = other.m_neighbors[0];
    this->m_neighbors[1] = other.m_neighbors[1];
    this->m_neighbors[2] = other.m_neighbors[2];
    this->m_neighbors[3] = other.m_neighbors[3];
    this->m_neighbors[4] = other.m_neighbors[4];
    
    return *this;
}


Atom::~Atom(){}


AMD::Vec3& Atom::Get_Coords(){
    return this -> m_coords;
}
unsigned int Atom::Get_Type() const{
    return m_type;
}

unsigned int Atom::Get_ID() const{
    return m_id;
}


unsigned int Atom::Get_Num_Neighbors() const{
    return m_num_neighbors;
}

Atom** Atom::Get_Neighbors(){
    return m_neighbors;
}


void Atom::Set_Coords(float x, float y, float z){
    this->m_coords.x = x;
    this->m_coords.y = y;
    this->m_coords.z = z;
}


void Atom::Set_Vals(Atom_Line& line){
    m_id = line.id;
    m_type = line.type;
    m_coords = line.coords;
    Shift();
    Clear_Neighbors();
}

void Atom::Set_Type(int t){
    m_type = t;
}

void Atom::Clear_Neighbors(){
    for (int i = 0; i< m_num_neighbors; i++){
        m_neighbors[i] = 0;
    }
    m_num_neighbors = 0;
}

void Atom::Push_Neighbor(Atom& neb){
    if (m_num_neighbors == MAX_NEIGHBORS){
        std::cout << "Exceeded Max Number of Neighbors!!!\n";
        Print_Neighbors();
        exit(-5);
    }
    this->m_neighbors[m_num_neighbors] = &neb;
    m_num_neighbors ++;
}


void Atom::Pop_Neighbor(Atom& neb){
    
}

void Atom::Shift(){
    AMD::Vec3 Box = Sim->Sim_Box();
    AMD::Vec3 shift = Sim->shift;
    float sx = Box.x * shift.x;
    
    if(m_coords.x >= sx){
        m_coords.x -= sx;
    }
    else{
        m_coords.x += (1.0 - shift.x)*Box.x;
    }
    
    float sy = Box.y * shift.y;
    
    if(m_coords.y >= sy){
        m_coords.y -= sy;
    }
    else{
        m_coords.y += (1.0 - shift.y)*Box.y;
    }
    
    
    float sz = Box.z * shift.z;
    
    if(m_coords.z >= sz){
        m_coords.z -= sz;
    }
    else{
        m_coords.z += (1.0 - shift.z)*Box.z;
    }
}


void Atom::Print(){
    std::cout << m_id << " " << m_type
    << " " << m_coords.x << " " << m_coords.y << " " << m_coords.z << std::endl;
}


void Atom::Print_Neighbors(){
    Print();
    for(int i = 0; i<m_num_neighbors; i++){
        m_neighbors[i] -> Print();
        std::cout << Boundary_Wrapped_Dist(this->Get_Coords(), m_neighbors[i]->Get_Coords()) << std::endl;
    }
    
}

CL_Atom::CL_Atom()
{
    m_type = 0;
    m_id = 0;
    m_coords.s[0] = 0.0;
    m_coords.s[1] = 0.0;
    m_coords.s[2] = 0.0;
}

CL_Atom::CL_Atom(const Atom& at)
{
    m_type = at.Get_Type();
    m_id = at.Get_ID();
    m_coords.s[0] = at.m_coords.x;
    m_coords.s[1] = at.m_coords.y;
    m_coords.s[2] = at.m_coords.z;
}




//###################Bond class#####################

Bond::Bond(){}

Bond::Bond(Atom& A,Atom& B)
{
    m_start = A.Get_Coords();
    m_end = B.Get_Coords();
    m_vec = m_end - m_start;
    m_types.x = A.Get_Type();
    m_types.y = B.Get_Type();
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

//
//
//
//   This is the simulation class it contains all info about the simulation.

Simulation::Simulation()
:m_num_blocks(0), m_curr_block(0),m_num_atoms(0), m_init(false), shift(0.0,0.0,0.0) {}




Simulation::~Simulation(){free(atoms);}


Simulation Simulation::inst;

Simulation* Simulation::Get(){ return &inst;}






void Simulation::Init(const char* file, const char* ft){
    data->Init(file, ft);
    m_num_atoms = data->dumps[0].dump_num_atoms;
    atoms = (Atom*)malloc(m_num_atoms*sizeof(Atom));
    for(int i = 0; i<m_num_atoms; i++){
        atoms[i] = Atom();
    }
    Set_Block(0);
    m_bonds = (AMD::Vec3*)malloc(6*m_num_atoms*sizeof(AMD::Vec3));
    Compute_Neighbors();
    Check_Nebs();
    m_init = true;
    
}
void Simulation::Compute_Neighbors() { 
    int count = 0;
    float dist;
    for (int i = 0; i< m_num_atoms; i++){
        AMD::Vec3 A = atoms[i].Get_Coords();
        if(A.x > 10000.0){continue;}
        for (int j = i+1; j< m_num_atoms; j++){
            AMD::Vec3 B = atoms[j].Get_Coords();
            if(B.x > 10000.0){continue;}
            dist = Dist(A, B);
            float cut = Comp_Bond_Length(atoms[i], atoms[j]);
            if(dist <= cut){
                m_bonds[count] = A;
                count++;
                m_bonds[count] = B;
                count++;
                atoms[i].Push_Neighbor(atoms[j]);
                atoms[j].Push_Neighbor(atoms[i]);
            }
        
        }
    }
    this -> m_num_bonds = count;
}



void Simulation::Check_Nebs()
{
    for(int i = 0; i<m_num_atoms; i++){
        int num = atoms[i].Get_Num_Neighbors();
        if(num < 1){atoms[i].Print();}
    }
}

void Simulation::Set_Block(int block){
    if(block > m_num_blocks-1){
        block = 0;
    }
    else if (block < 0){
        block = m_num_blocks - 1;
    }
    
    m_timestep = data->dumps[block].timestep;
    m_num_atoms = data->dumps[block].dump_num_atoms;
    
    for (int i = 0; i< 3; i++){
        m_lattice[i] = data->dumps[block].m_lattice[i];
    }
    
    
    
    for (int i = 0; i< m_num_atoms; i++){
        atoms[i].Set_Vals(data->dumps[block].Atom_Lines[i]);
    }
    
    m_curr_block = block;
}


void Simulation::Update_Sim(char dir){
    switch (dir) {
        case 'f':
            Set_Block(m_curr_block + 1);
            break;
            
        case 'r':
            Set_Block(m_curr_block - 1);
            
        case 'n':
            Set_Block(m_curr_block);
        
        default:
            break;
    }
    Compute_Neighbors();
    m_need_update = true;
}



bool Simulation::Is_Init(){
    return m_init;
}

int Simulation::Timestep(){
    return m_timestep;
}


int Simulation::Num_Atoms(){
    return this->m_num_atoms;
}

int Simulation::Num_Bonds(){
    return this->m_num_bonds;
}

int Simulation::Num_Blocks(){
    return this->m_num_blocks;
}


AMD::Vec3 Simulation::Sim_Box(){
    float x = m_lattice[0][0];
    float y = m_lattice[1][1];
    float z = m_lattice[2][2];
    return AMD::Vec3(x, y, z);
}


Atom* Simulation::Atoms(){
    return atoms;
}

AMD::Vec3* Simulation::Bonds(){return m_bonds;}

void Simulation::print(Atom_Attrib attrib){
    switch (attrib) {
        case COORDS:
            for (int i = 0; i< m_num_atoms; i++){
                atoms[i].Get_Coords().print();
                std::cout << "=======================================" << std::endl;
            }
            break;
        case TYPE:
            for (int i = 0; i< m_num_atoms; i++){
                std::cout << atoms[i].Get_Type() << std::endl;
                std::cout << "=======================================" << std::endl;
            }
            break;
            
        case AT_NEIGHBORS:
            for (int i = 0; i< m_num_atoms; i++){
                atoms[i].Print_Neighbors();
                std::cout << "=======================================" << std::endl;
            }
            break;
            
        case NEIGHBORS:
            for (int i = 0; i< m_num_bonds; i++){
                //std::cout << neighbor_IDs[i][0] <<" -- " << neighbor_IDs[i][1] << std::endl;
                std::cout << "=======================================" << std::endl;
            }
            break;
        default:
            break;
    }
    
    
}

