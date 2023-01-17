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


const char* test_file = "/Users/diggs/Desktop/TOPCon/OUT-FILES/out-nuc-1-14-23-2/nuc-1.36.dump";
extern int num_lines;

void JUNK(Simulation& sim){
    
    Renderer rend;
    
    Atom_Mesh at;
    at.Add_Instance_Layout(sim);
    
    
    Operator op;
    Light_Src l_src;
    
    UI_Window ui(0.0,0.0,rend.get_window());
    while (!rend.is_open()) {
        rend.clear(ui);
        at.Set_OPs(op, l_src, 1);
        at.Set_OPs(op, l_src, 2);
        at.Draw();
        
        ui.NewFrame();
        ui.Model_UI(op, l_src);
        ui.log_window(op.M_mat);
        ui.log_window(op.Proj_mat);
        ui.render();
        
        rend.poll();
        
    }
    
}

int main(void)
{
    Simulation test = read_dump(test_file);
    JUNK(test);

    return 0;
}
