//  SimViz
//
//  Created by Andrew Diggs on 12/29/22.
//

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#define ShFile "/Users/diggs/Desktop/SimViz/Shaders/atom.fs"

#define LOG(x) std::cout << x << std::endl

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
#include "Render.hpp"
#include "Object.hpp"

#include "FileIO.hpp"
#include "Atomic.hpp"


const char* test_file = "/Users/diggs/Desktop/SimViz/SimViz/Other/small-test.dump";
extern int num_lines;

void JUNK(Simulation& sim){
    
        
    
}

int main(void)
{
    Simulation test;
    
    Renderer rend;
    Atom_Mesh at;
    
    
    Operator op;
    Light_Src l_src;
    
    UI_Window ui(0.0,0.0,rend.get_window());
    while (!rend.is_open()) {
        rend.clear(ui);
        
        ui.NewFrame();
        ui.Model_UI(op, l_src, test);
        //ui.log_window(op.M_mat);
        //ui.log_window(op.Proj_mat);
        ui.render();
        if(test.Is_Init()){
            at.Set_OPs(op, l_src, test);
            at.Draw();
        }
        
        rend.poll();
        
    }
    return 0;
}
