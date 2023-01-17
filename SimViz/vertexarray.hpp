//
//  vertexarray.hpp
//  OpenGL
//
//  Created by Andrew Diggs on 7/27/22.
//

#ifndef vertexarray_hpp
#define vertexarray_hpp

#include <stdio.h>
class VertexBuffer;
class VertexArray{
private:
    unsigned int m_ID;
    unsigned int count = 0;
public:
    VertexArray();
    ~VertexArray();
    
    void bind() const;
    void unbind()const;
    unsigned int AddBuffer(const void* off_set, unsigned int size, int num);
    //void AddBuffer(const void* off_set, int num);
    void Add_Vertex_Buffer(VertexBuffer& vb);
    
};
#endif /* vertexarray_hpp */
