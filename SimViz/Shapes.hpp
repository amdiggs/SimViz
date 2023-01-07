//
//  Shapes.hpp
//  OpenGL
//
//  Created by Andrew Diggs on 7/14/22.
//

#ifndef Shapes_hpp
#define Shapes_hpp

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "AMDmath.hpp"


#define MAX_VERTS 10000
#define MAX_IDX 50000






class SHAPE{
protected:
    int m_num_idx;
    int m_num_verts;
    const int m_num_theta = 60;
    const float m_dth = _2pi/m_num_theta;
    void Gen_cyl_index(int off_set);
    void Gen_fan_index(int count, int tip);
public:
    SHAPE();
    virtual ~SHAPE() = 0;
    AMD::Vertex verts[MAX_VERTS];
    unsigned int indices[MAX_IDX];
    int num_idx() const ;
    int num_verts() const;
    virtual void Gen_points() = 0;
    virtual void Gen_indices() = 0;
    
    
};



class Circle: public SHAPE{
private:
    float rad;
    void Gen_points() override;
    void Gen_indices() override;
public:
    Circle(float ex_rad);
    ~Circle();
    

    
};



class Cube{
private:
    int m_num_idx = 36;
    int m_num_verts = 8;
    AMD::Vec3 m_lengths;

    void Gen_Points();
    
    
    
public:
    Cube();
    Cube(AMD::Vec3 BB);
    ~Cube();
    AMD::Vertex verts[8];
    unsigned int indices[36] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3
    };
    
    int num_idx();
    int num_verts();
    
    
    
    
};

class Quad{
private:
    void gen_verts();
    void gen_verts(AMD::Vec3 A, AMD::Vec3 B, AMD::Vec3 C, AMD::Vec3 D);
    int m_num_verts = 4;
    int m_num_idx = 6;
    float m_size;
public:
    Quad();
    Quad(float s);
    Quad(AMD::Vec3 A, AMD::Vec3 B, AMD::Vec3 C, AMD::Vec3 D, const char* cw);
    
    AMD::Vertex verts[4];
    unsigned int indices[6] = {0,1,2,0,2,3};
    int num_idx();
    int num_verts();
};




class Sphere : public SHAPE{
private:
    float rad;
    void Gen_points() override;
    void Gen_indices() override;
    
public:
    Sphere(float e_rad);
    ~Sphere();
    
    AMD::Vec4 m_clr;
    
    
    
};




class Cylinder: public SHAPE{
private:
    float m_dz;
    float m_rad;
    float m_len;
    void Gen_points() override;
    void Gen_indices() override;
    
    
public:
    Cylinder(float e_len);
    Cylinder();
    ~Cylinder();
    
};


class Cylinder2: public SHAPE{

private:
    float m_rad;
    float m_len;
    void Gen_points() override;
    void Gen_indices() override;
    
    
public:
    Cylinder2(float e_len);
    Cylinder2();
    ~Cylinder2();
};


class Cone: public SHAPE{
private:
    void Gen_points() override;
    void Gen_indices() override;
    
    
public:
    Cone();
    ~Cone();
    
    
    
};



class Arrow: public SHAPE {
    
private:
    float m_length;
    int m_num_z;
    float text_id;
    AMD::Vec4 m_color;
    AMD::Mat4 r_mat;
    AMD::Mat4 label_rot_mat;
    AMD::Vec3 delta;
    void Gen_points() override;
    void Gen_indices() override;
    void Gen_Quad();
    void Coordinate_Transform();
    void Rotation();
    
    
public:
    Arrow(char color);
    Arrow(char color, char dir);
    Arrow(char color, char dir, float len);
    ~Arrow();
    
};


class Grid{
    
private:
    static const unsigned int max_verts = 50000;
    static const unsigned int max_indices = 250000;
    float m_w;
    float m_h;
    float m_spacing;
    int m_num_points;
    int m_num_line_idx;
    int m_num_tri_idx;
    int m_num_verts;
    void gen_points();
    void gen_line_indices();
    void gen_tri_indices();
    void add_norm();
    
public:
    Grid(float e_spaceing);
    Grid(std::string file_name);
    ~Grid();
    AMD::Vertex verts[max_verts];
    unsigned int L_indices[max_indices];
    unsigned int T_indices[max_indices];
    int num_idx(char type);
    int num_verts();
    
};


class Axis{
    
private:
    static const unsigned int max_verts = 20000;
    static const unsigned int max_indices = 150000;
    const int m_num_theta = 20;
    int m_num_idx;
    int m_num_verts;
    void gen_points(const Arrow& ar);
    void gen_points(const Sphere& sp);
    void gen_quad();
    void Gen_Sphere();
    void Gen_Arrow(char axis);
    AMD::Vec4 m_color;
    AMD::Mat4 r_mat;
    void Coordinate_Transform();
    void Rotation();
    
    
public:
    Axis();
    ~Axis();
    AMD::Vertex verts[max_verts];
    unsigned int indices[max_indices];
    int num_idx();
    int num_verts();
};

void ReadXYZ(std::string in_file, AMD::Vertex* verts, int& num_verts);
AMD::Vec3 Normed_average(AMD::Vec3 Va, AMD::Vec3 Vb);


#endif /* Shapes_hpp */
