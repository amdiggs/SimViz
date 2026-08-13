//
//  Texture.hpp
//  OpenGL
//
//  Created by Andrew Diggs on 10/2/22.
//

#ifndef Texture_hpp
#define Texture_hpp
#define IW 128



#include <stdio.h>
#include "Render.hpp"
enum Texture_Type{Object_Color_Texture,Object_Normal_Map, Environment_Color, Shadow_Map_Texture, Render_Texture};

class Atom;
class Shader;
class Texture{
private:
    
    GLenum m_type = GL_TEXTURE_2D;
    void Gen_Tex_2D(void* dat);
    int m_w, m_h, m_bpp;
    unsigned int m_ID;
    int m_layer;
    
    
public:
    Texture(int layer);
    
    ~Texture();
    
    void Bind() const;
    void UnBind() const;
    void Load(std::string image);
    void Gen_Cube_Map(std::string image);
    
    int Get_Layer() const;
    int Get_ID() const;
    
};


class Texture3D{
private:
    
    GLenum m_type = GL_TEXTURE_3D;
    void Gen_Tex_3D(void* dat);
    int m_w, m_h,m_z, m_bpp;
    unsigned int m_ID;
    int m_layer;
    
    
public:
    Texture3D(int layer);
    
    ~Texture3D();
    
    void Bind() const;
    void UnBind() const;
    void Load(std::string image);
    
    
    void Simple_Test();
    void Cavity();
    void Cavity(AMD::Vec3* vecs, int num_vecs, AMD::Vec3 box_bounds);
    int Get_Layer() const;
    int Get_ID() const;
    
};


#endif /* Texture_hpp */
