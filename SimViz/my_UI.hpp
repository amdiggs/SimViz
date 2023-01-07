//
//  my_UI.hpp
//  OpenGL
//
//  Created by Andrew Diggs on 9/16/22.
//

#ifndef my_UI_hpp
#define my_UI_hpp

#include <stdio.h>
#include "OGL.h"
#include "../OutSideResources/imgui.h"
#include "../OutSideResources/imgui_impl_glfw.h"
#include "../OutSideResources/imgui_impl_opengl3.h"
#include "AMDmath.hpp"

class Operator;
class Light_Src;

ImGuiIO& init_io();

class UI_Window{
private:
    GLFWwindow* m_window;
    ImGuiIO& m_io;
    AMD::Vec4 m_cc;
    AMD::Vec4 Ob_clr;
    
    ImGuiDockNodeFlags m_doc_flags =  ImGuiDockNodeFlags_PassthruCentralNode;
    float view_x, view_y, view_z;
    float m_x, m_y;
    int display_w, display_h;
    float  ui_x, ui_y;
    const char* m_version = "#version 150";
 
    float projection[4] = {1.0, -3.0, 3.0, 3.0};
    
public:
    UI_Window(float pos_x, float pos_y,GLFWwindow* window);
    ~UI_Window();
    
    
    void Model_UI(Operator& op ,Light_Src& light_src);
    void render() const;
    void NewFrame() const;
    
    AMD::Vec4& get_color();
    
    
    void mouse_drag(AMD::Vec3& vec);
    
    //void log_window(Ensamble_Of_Atoms& ats);
    //void log_window( Bond* bd, int num);
    void log_window( AMD::Vertex* verts, int num);
    void log_window( AMD::Vertex* verts, unsigned int* idx, int num);
    void log_window( AMD::Mat4 mat);
    void log_window(int neb[][2], int num);
    void Write_Buffer(std::string file_name);
    
    
};



#endif /* my_UI_hpp */
