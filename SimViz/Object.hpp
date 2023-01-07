//
//  Object.hpp
//  SimViz
//
//  Created by Andrew Diggs on 1/3/23.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include <string>
#include "vertexbuffer.hpp"
#include "vertexarray.hpp"
#include "shader.hpp"
#include "Shapes.hpp"

class Mesh{
private:
    VertexArray m_VAO;
    IndexBuffer m_IBO;
    std::string m_shader_file;
    
public:
    Mesh();
    virtual ~Mesh() = 0;
    void Bind();
    void unBind();
    virtual void Draw();
};


#endif /* Object_hpp */
