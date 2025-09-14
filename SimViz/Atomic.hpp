//
//  Atomic.hpp
//  OpenGL
//
//  Created by Andrew Diggs on 8/31/22.
//

#ifndef Atomic_hpp
#define Atomic_hpp

#define MAX_ATOMS 10000
#define MAX_NEIGHBORS 16

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include "AMDmath.hpp"
#include <OpenCL/opencl.h>
#include "FileIO.hpp"

enum Atom_Attrib{COORDS, NEIGHBORS, TYPE, AT_NEIGHBORS};

class Atom_Mesh;
class Hist_2D_Grid_Mesh;
class Atom;
struct CL_Atom;



class Atom{
private:
    unsigned int m_id;
    unsigned int m_type;
    AMD::Vec3 m_coords;
    int m_num_neighbors;
    Atom* m_neighbors[16];
    
public:
    Atom();
    Atom(int _id, int _type, float x, float y, float z);
    Atom(Atom_Line al);
    ~Atom();
    
    unsigned int Get_ID() const;
    unsigned int Get_Type() const;
    unsigned int Get_Num_Neighbors() const;
    AMD::Vec3& Get_Coords();
    Atom** Get_Neighbors();
    
    void Set_Coords(float x, float y, float z);
    void Set_Vals(Atom_Line& line);
    void Set_Type(int t);
    void Clear_Neighbors();
    void Push_Neighbor(Atom& neb);
    void Pop_Neighbor(Atom& neb);
    
    void Rescale();
    void Shift();
    void Print();
    void Print_Neighbors();
    
    Atom& operator=(const Atom& other);
    friend CL_Atom;

};

struct CL_Atom{
    cl_int m_id;
    cl_int m_type;
    cl_float3 m_coords;
    
    CL_Atom();
    CL_Atom(const Atom& at);
};


class Bond{
private:

    AMD::Vec3 m_start;
    AMD::Vec3 m_end;
    AMD::Vec3 m_vec;
    AMD::Vec3 m_ang;
    float m_len;
    
    
public:
    Bond();
    Bond(Atom& A,Atom& B);
    ~Bond();
    void Set_Theta();
    void Set_Phi();
    void Set_Len();
    
    AMD::Vec3 get_off_set();
    AMD::Vec3& get_angles();
    AMD::Vec2& get_types();
    AMD::Vec2 m_types;
    float get_len();
    
};

class Simulation{
private:
    
    // general info about simulation
    int m_timestep;
    int m_num_atoms;
    AMD::Vec3 m_lattice[3];
    
    int m_num_blocks;
    int m_curr_block;
    
    
    bool m_init;
    bool m_need_update = false;
    int m_num_bonds;
    
    //Dump* m_data;
    Atom* atoms = NULL;
    AMD::Vec3* m_bonds=NULL;
    //private functions
    void Set_Block(int start);
    void Update_Sim(char dir);
    void Set_Sim_Box(const char** bb, int line_num);
    
    //Singelton constructor
    Simulation();
    Simulation(const Simulation&) = delete;
    static Simulation inst;
    
    
    //needed for rendering
    //int neighbor_IDs[4*MAX_ATOMS][2];
public:
    void Init(const char* file, const char* ft);
    static Simulation* Get();
    ~Simulation();
    AMD::Vec3 shift;
    
    //Getters
    int Timestep();
    int Num_Atoms();
    int Num_Bonds();
    int Num_Blocks();
    AMD::Vec3 Sim_Box();
    Atom* Atoms();
    AMD::Vec3* Bonds();
    bool Is_Init();
    bool Need_Update(){return m_need_update;}
    void Updated(){m_need_update = false;}
    void Compute_Neighbors();
    void Check_Nebs();
    void Step_Forward(){Update_Sim('f');}
    void Step_Bacward(){Update_Sim('r');}
    void Step_Zero(){Update_Sim('n');}
    void print(Atom_Attrib attrib);
};

//float Boundary_Wrapped_Dist(AMD::Vec3 A, AMD::Vec3 B);

#endif /* Atomic_hpp */
