//
//  Atomic.cpp
//  OpenGL
//
//  Created by Andrew Diggs on 8/31/22.
//

#include "Atomic.hpp"
#include "AMDmath.hpp"
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

AMD::Vec3 Boundary_Wrapped_Diff(AMD::Vec3 A, AMD::Vec3 B){
    AMD::Vec3 box = Sim->Sim_Box();
    AMD::Vec3 ret;
    for (int i = 0; i< 3; i++){
        float delta = abs(A[i]-B[i]);
        if(delta > 0.5*box[i]){
            ret[i] = (A[i] < B[i]) ? A[i] + box[i] - B[i] : A[i] - box[i] - B[i];
        }
        else{ret[i] = A[i] - B[i];}
    }
    
    return ret;
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
    return 1.33*(ra + rb);
}

Atom::Atom()
: m_id(10000000), m_type(0), m_coords(0.0,0.0,0.0), m_num_neighbors(0), draw(true)
{}

Atom::Atom(Atom_Line al)
: m_num_neighbors(0), draw(true)
{
    m_id = al.id;
    m_type = al.type;
    m_coords = al.coords;

}

Atom::Atom(int _id, int _type, float x, float y, float z)
:m_id(_id), m_type(_type),m_coords(x, y, z), m_num_neighbors(0), draw(true)
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
bool Atom::Should_Draw(){
    return draw;
}
void Atom::Hide(){draw = false;}
void Atom::Dont_Hide(){draw = true;}
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

Bond::Bond(AMD::Vec3 A,AMD::Vec3 B)
{
    m_origin = A;
    m_dir = Boundary_Wrapped_Diff(B, A);
    m_len = m_dir.len();
}


Bond::~Bond() {}

//
//
Molecule::Molecule(){};
Molecule::~Molecule(){};

void Molecule::Push_Atom(Atom* at){
    m_ats[m_num_ats] = at;
    m_num_ats++;
}

Atom** Molecule::Get_Atoms(){return &(m_ats[0]);}

Dipole Molecule::Comp_Dipole(){
    AMD::Vec3 v1;
    AMD::Vec3 dir;
    AMD::Vec3 A = m_ats[0]->Get_Coords();
    for(int i = 1; i< m_num_ats; i++){
    AMD::Vec3 B = m_ats[i]->Get_Coords();
        v1 = AMD::Normalize(Boundary_Wrapped_Diff(A,B));
        dir+=v1;
    }
    m_dp.dir = AMD::Normalize(dir);
    m_dp.len = dir.len();
    m_dp.origin = m_ats[0]->Get_Coords();
    return m_dp;
}


void Molecule::Clear(){
    m_num_ats = 0;
}


int H2O_Check(Atom& at, Atom** others){
    int num_H = 0;
    Atom** nebs = at.Get_Neighbors();
    for(int i = 0; i<at.Get_Num_Neighbors(); i++){
        Atom* neb = nebs[i];
        atom_info ai = Get_Atom_Info(neb->Get_Type());
        if(strcmp(ai.ID, "H") == 0){
            others[num_H] = neb;
            num_H ++;
        }
    }
    //if(num_H > 2){printf("num H = %d\n",num_H);}
    return num_H;
}

Array_of_H2O::Array_of_H2O(){molecs = new Molecule[500];}
Array_of_H2O::~Array_of_H2O(){delete[] molecs;}



void Array_of_H2O::Comp_H2O(){
    int num_ats = Sim->Num_Atoms();
    int count = 0;
    Atom* ats = Sim->Atoms();
    Atom** nebs = (Atom**)malloc(5*sizeof(Atom*));
    int num_neb = 0;
    for(int i = 0; i<num_ats; i++){
        //if(ats[i].Get_Type() != 79){continue;}
        num_neb = H2O_Check(ats[i], nebs);
        if(num_neb >= 2){
            molecs[count].Clear();
            molecs[count].Push_Atom(&(ats[i]));
            ats[i].draw = false;
            for(int j = 0; j < num_neb; j++){
                molecs[count].Push_Atom(nebs[j]);
                nebs[j]->draw = false;
            }
            count ++;
        }
    }
    num_h2o = count;
    free(nebs);
    printf("num H2O = %d\n",count);
}
//   This is the simulation class it contains all info about the simulation.

Simulation::Simulation()
:m_num_blocks(0), m_curr_block(0),m_num_atoms(0), m_init(false),num_atom_types(0), shift(0.0,0.0,0.0) {}

Simulation::~Simulation(){delete [] m_atoms; free(m_bonds);}

Simulation Simulation::inst;

Simulation* Simulation::Get(){ return &inst;}

void Simulation::Init(const char* file, int ft){
    data->Init(file, ft);
    m_num_blocks = data->num_dumps;
    m_num_atoms = data->dumps[0].dump_num_atoms;
    m_atoms = new Atom[m_num_atoms];
    Set_Block(0);
    m_bonds = (Bond*)malloc(6*m_num_atoms*sizeof(Bond));
    Compute_Neighbors();
    //Check_Nebs();
    m_init = true;
    m_need_update = true;
}
void Simulation::Compute_Neighbors() { 
    int count = 0;
    float dist;
    for (int i = 0; i< m_num_atoms; i++){
        int at_typ = m_atoms[i].Get_Type();
        bool is_in = false;
        for(int t = 0; t < num_atom_types; t++){
            if(at_typ == atom_types[t]){
                is_in = true;
                break;
            }
        }
        if(!is_in){
            atom_types[num_atom_types] = at_typ;
            num_atom_types ++;
        }
        AMD::Vec3 A = m_atoms[i].Get_Coords();
        if(A.x > 10000.0){continue;}
        for (int j = i+1; j< m_num_atoms; j++){
            AMD::Vec3 B = m_atoms[j].Get_Coords();
            if(B.x > 10000.0){continue;}
            dist = Boundary_Wrapped_Dist(A, B);
            float cut = Comp_Bond_Length(m_atoms[i], m_atoms[j]);
            if(dist <= cut){
                m_bonds[count] = Bond(A,B);
                count++;
                m_atoms[i].Push_Neighbor(m_atoms[j]);
                m_atoms[j].Push_Neighbor(m_atoms[i]);
            }
        
        }
    }
    this -> m_num_bonds = count;
}



void Simulation::Check_Nebs()
{
    for(int i = 0; i<m_num_atoms; i++){
        int num = m_atoms[i].Get_Num_Neighbors();
        if(num < 1){m_atoms[i].Print();}
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
    
    m_lattice = data->dumps[block].m_lattice;
    
    for (int i = 0; i< m_num_atoms; i++){
        m_atoms[i].Set_Vals(data->dumps[block].Atom_Lines[i]);
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

int Simulation::Num_Types(){
    return this->num_atom_types;
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

int* Simulation::Types(){
    return atom_types;
}

Atom* Simulation::Atoms(){
    return m_atoms;
}

Bond* Simulation::Bonds(){return m_bonds;}

void Simulation::print(Atom_Attrib attrib){
    switch (attrib) {
        case COORDS:
            for (int i = 0; i< m_num_atoms; i++){
                m_atoms[i].Get_Coords().print();
                std::cout << "=======================================" << std::endl;
            }
            break;
        case TYPE:
            for (int i = 0; i< m_num_atoms; i++){
                std::cout << m_atoms[i].Get_Type() << std::endl;
                std::cout << "=======================================" << std::endl;
            }
            break;
            
        case AT_NEIGHBORS:
            for (int i = 0; i< m_num_atoms; i++){
                m_atoms[i].Print_Neighbors();
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

