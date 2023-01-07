//  SimViz
//
//  Created by Andrew Diggs on 12/29/22.
//

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#define ShFile "/Users/diggs/Desktop/SimViz/Shaders/atom.fs"

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "Object.hpp"
#include "Shapes.hpp"
#include "AMDmath.hpp"
#include "Operations.hpp"
#include "my_UI.hpp"


void Simple_Test(){
    Shader sh(ShFile);
    Sphere sp(1.0);
    VertexArray VAO;
    VertexBuffer VBO(sp.verts, sp.num_verts()*sizeof(AMD::Vertex));
    VAO.Add_Vertex_Buffer(VBO);
    IndexBuffer IBO;
    IBO.Gen_Buffer(sp.indices, sp.num_idx());
    sh.bind();
}

int main(void)
{
    GLFWwindow* window;
    std::cout << "Testing" << std::endl;
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHintString(GLFW_COCOA_FRAME_NAME, "testframe");
    
    window = glfwCreateWindow(1600, 1200, "SimViz", NULL, NULL);
    
    
    if(!window){
        glfwTerminate();
        return -1;
    }
    
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glewExperimental = GL_TRUE;
    glewInit();
    
    
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    Sphere sp(1.0);
    VertexArray VAO;
    VertexBuffer VBO(sp.verts, sp.num_verts()*sizeof(AMD::Vertex));
    VAO.Add_Vertex_Buffer(VBO);
    IndexBuffer IBO;
    IBO.Gen_Buffer(sp.indices, sp.num_idx());
    Shader sh("/Users/diggs/Desktop/SimViz/SimViz/Shaders/atom.fs");
    sh.bind();
    
    Operator op;
    Light_Src l_src;
    
    UI_Window ui(0.0,0.0,window);
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1, 0.0, 0.35, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        sh.Set_Uniforms(op, l_src);
        sh.bind();
        VAO.bind();
        IBO.bind();
        glDrawElements(GL_TRIANGLES,IBO.get_num(), GL_UNSIGNED_INT, 0);
        
        ui.NewFrame();
        ui.Model_UI(op, l_src);
        ui.render();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    glfwTerminate();
    return 0;
}
