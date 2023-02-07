//
//  Atomic.hpp
//  OpenGL
//
//  Created by Andrew Diggs on 8/31/22.
//

#ifndef Atomic_hpp
#define Atomic_hpp

#define MAX_ATOMS 6000

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include "AMDmath.hpp"
class Atom_Mesh;


class Atom{
private:
    unsigned int m_id;
    unsigned int m_type;
    AMD::Vec3 m_coords;
    int m_num_neighbors;
    
public:
    Atom();
    Atom(std::string line);
    Atom(int _id, int _type, float x, float y, float z);
    ~Atom();
    
    unsigned int get_id() const;
    unsigned int get_type() const;
    AMD::Vec3& get_coords();
    
    void Rescale();
    void Find_Neighbors(Atom* ats);
    void Print();

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


struct Sim_Block
{
    int timestep;
    int num_atoms;
    float sim_box[3][2];
    Atom atoms[MAX_ATOMS];
};

class Simulation{
private:
    int m_timestep;
    float cutoffs[3] = {2.6, 1.8, 1.0};
    int block_len;
    int num_lines;
    int num_blocks;
    int curr_line;
    char** m_data;
    bool init;
    
    void Read_Dump(const char* file);
    void Set_Block(int start);
    void Set_Neighbors();
    void Get_Num_Lines(const char* file);
    
public:
    int num_atoms;
    float sim_box[3][2];
    Atom atoms[MAX_ATOMS];
    
    int neighbor_IDs[4*MAX_ATOMS][2];
    int num_bonds;
    
    Simulation();
    Simulation(const char* file);
    ~Simulation();
    
    int Get_Timestep();
    bool Is_Init();
    void Update_Sim(char dir);
    void Init_Sim(std::string file);
    float** Compute_Histogram();
    
    void print();
};


#endif /* Atomic_hpp */
