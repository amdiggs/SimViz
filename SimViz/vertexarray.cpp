//
//  vertexarray.cpp
//  OpenGL
//
//  Created by Andrew Diggs on 7/27/22.
//

#include "vertexarray.hpp"
#include "vertexbuffer.hpp"
#include "OGL.h"
#include "AMDmath.hpp"
VertexArray::VertexArray()
:count(0){
    
    glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray(){
    
    glDeleteVertexArrays(1, &m_ID);
}


void VertexArray::AddBuffer( const void* off_set,unsigned int size, int num ){
    bind();
    unsigned int testVBO;
    int num_comp = size/sizeof(float);
    glGenBuffers(1, &testVBO);
    glBindBuffer(GL_ARRAY_BUFFER, testVBO);
    glBufferData(GL_ARRAY_BUFFER, num*size, (void*)off_set, GL_STATIC_DRAW);
    glEnableVertexAttribArray(count);
    glVertexAttribPointer(count, num_comp, GL_FLOAT, GL_FALSE, size, (void*)0);
    glVertexAttribDivisor(count,1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    count++;
    
}





void VertexArray::Add_Vertex_Buffer(VertexBuffer& vb){
    bind();
    vb.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AMD::Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(AMD::Vertex), (void*)offsetof(AMD::Vertex, clr));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(AMD::Vertex), (void*)offsetof(AMD::Vertex, norm));
    //glEnableVertexAttribArray(3);
    unbind();
    vb.unbind();
}

void VertexArray::bind(){
    
    glBindVertexArray(m_ID);
    
}

void VertexArray::unbind(){
    glBindVertexArray(0);
}
