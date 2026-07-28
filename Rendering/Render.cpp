//
//  Render.cpp
//  OpenGL
//
//  Created by Andrew Diggs on 7/14/22.
//

#include "Render.hpp"
#include "imgui.h"
#include "vertexbuffer.hpp"
#include "vertexarray.hpp"
#include "shader.hpp"
#include "Atomic.hpp"
#include "Meshes.hpp"
#include "FrameBuffer.hpp"
#include "Operations.hpp"
#include "Computes.hpp"
#include "FileIO.hpp"
#include <filesystem>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


//std::string save_file = "Users/diggs/Desktop/volume.png";
bool save = false;
static AMD::Vec4 CC(1.0,1.0,1.0,1.0);
int Ww =0;
int Wh = 0;
extern Operator* op;
extern Simulation* Sim;
UI_Window* ui = UI_Window::Get();
Renderer* rend = Renderer::Get();


ImGuiIO& init_io(){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    return io;
}

Renderer::Renderer()
{   glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    this -> m_Window = glfwCreateWindow(1800, 1000,"SimViz", NULL, NULL);
    check(m_Window);
    set_context();
    ui->Init(m_Window);

}


Renderer* Renderer::Get(){return &inst;}
Renderer::~Renderer(){
    glfwTerminate();
}


void Renderer::check(GLFWwindow* window){
    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }
}


void Renderer::set_context(){
    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);
    glewExperimental = GL_TRUE;
    glewInit();
    glfwGetFramebufferSize(m_Window, &Ww, &Wh);
}

void Renderer::Draw_Pass(){
    ui->NewFrame();
    ui->Simple_window();
    glClearColor(CC[0], CC[1], CC[2], 0.0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glfwGetFramebufferSize(m_Window, &Ww, &Wh);
    int dw = Ww - 2.0*ui->display_w;
    //int dh= 2.0*ui->display_h;
    glViewport(ui->display_w,0, dw, Wh);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glDisable(GL_CULL_FACE);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::Draw(){
    for(int i = 0; i<num_meshes; i++){
        meshes[i]->Set_Shader();
        meshes[i]->Draw();
    }
}


void Renderer::Push_Mesh(Mesh& m){
    meshes[num_meshes] = &m;
    num_meshes++;
}

void Renderer::Push_Call(int call){
    calls[num_calls] = call;
    num_calls++;
}


void Renderer::Pop_Call(int call){
    int count = 0;
    for(int i = 0; i<num_calls; i++){
        int tmp = calls[i];
        if(tmp != call){
            calls[count] = tmp;
            count++;
        }
    }
    num_calls = count;
}
void Renderer::Set_Uniforms(){
    for(int i = 0; i<num_meshes; i++){
        meshes[i]->Set_Uniforms();
    }
}


void Renderer::Set_Uniforms(Light_Src& l_src){
    if(op->need_update){
        op->Set();
        for(int i = 0; i<num_meshes; i++){
            meshes[i]->Set_Uniforms(l_src);
        }
        op->need_update = false;
    }
}


int Renderer::is_open(){
    return glfwWindowShouldClose(m_Window);
}

void Renderer::poll(){
    ui->render();
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}





int Renderer::Write_Curr_Buffer(std::string file_name){
    stbi_flip_vertically_on_write(1);
    int width, height;
    glfwGetFramebufferSize(m_Window, &width, &height);
    const int num_pix = 3 * width * height;
    unsigned char* pixels = new unsigned char[num_pix];

    std::fstream outfile;
    outfile.open(file_name, std::ios::out);

    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    if(stbi_write_png(file_name.c_str(), width, height, 3, pixels, width*3*sizeof(unsigned char))){
        return 1;
    }
    else return -1;

}




GLFWwindow* Renderer::Get_Window(){
    return m_Window;
}









//##################################################################################################
//THIS IS MY UI CLASS!!!!!!!!!!!

UI_Window::UI_Window()
    : m_io(init_io())
{
    (void)m_io;
}


void UI_Window::Init(GLFWwindow* window)
{
    m_window = window;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(m_version);


    ImFont* font = m_io.Fonts -> AddFontFromFileTTF("/System/Library/Fonts/Helvetica.ttc", 16.0);
    IM_ASSERT(font != NULL);
    //glfwGetFramebufferSize(m_window, &display_w, &display_h);
    //ImVec2 ui_size = ImGui::GetWindowSize();
    display_w = 0;
    display_h = 0;
    m_dir = std::filesystem::current_path().string();

}

UI_Window::~UI_Window() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


UI_Window* UI_Window::Get(){return &inst;}

void UI_Window::Error_PopUp(const char* msg){
    ImGui::OpenPopup("Error");
    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Error:\n%s", msg);
        if (ImGui::Button("Ok", ImVec2(120, 0))) {
            err = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

bool UI_Window::ls_dir(){
    std::filesystem::path dir_path = m_dir;
    static int selected_file = -1;
    int i = 0;
    if (ImGui::Selectable("../", selected_file == i,ImGuiSelectableFlags_DontClosePopups)){
        size_t last_dir = m_dir.rfind('/');
        std::string tmp = m_dir.substr(0,last_dir);
        m_dir = tmp;
        return false;
    }
    i++;
    for( auto const& entry : std::filesystem::directory_iterator(dir_path)){
        std::string file_name = entry.path().filename().string();
        size_t dot_pos = file_name.rfind(".");
        if(dot_pos < file_name.length() - 1){
            std::string extension = file_name.substr(dot_pos + 1);
            bool is_dump = (std::strcmp(extension.c_str(), "dump") == 0);
            bool is_poscar = (std::strcmp(extension.c_str(), "poscar") == 0);
            bool is_xyz = (std::strcmp(extension.c_str(), "xyz") == 0);
            if(is_dump || is_poscar || is_xyz){
                if (ImGui::Selectable(file_name.c_str(), selected_file == i,ImGuiSelectableFlags_DontClosePopups)){
                    m_input_file = m_dir + "/" + file_name.c_str();
                    if(is_dump){m_ft = 0;}
                    else if(is_poscar){m_ft = 1;}
                    else if(is_xyz){m_ft = 2;}
                    return true;
                }
            }
            else{
                ImGui::TextDisabled("%s",file_name.c_str());
            }
        }
        else if(entry.is_directory()){
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.4f, 1.0f, 1.0f)); // R
                if (ImGui::Selectable(file_name.c_str(), selected_file == i,ImGuiSelectableFlags_DontClosePopups)){
                    std::string tmp = m_dir + "/" + file_name;
                    m_dir = tmp;
                    ImGui::PopStyleColor();
                    //printf("%s", m_dir.c_str());
                    return false;
                }
            ImGui::PopStyleColor();
        }
        i++;
    }
    return false;
}
//Draw_Call Draw_Funcs[5] = {Draw_Atoms, Draw_Iso, Draw_Rho, Draw_Wire,Draw_Vox_Full};

void UI_Window::Simple_window(){
    static float theta = 0.0;
    static float phi = 0.0;
    static float M_theta= 1.57;
    static float M_phi= 0.0;
    static bool play = false;
    static int counter = 1;
    static float C_theta = 0.0;
    static float C_phi = 0.0;
    static int timestep = 0;
    static bool file_selected = false;
    static float slice_lo = -50.;
    static float slice_hi = 50.;
    static AMD::Vec3 trans;
    static AMD::Vec3 look;
    static AMD::Vec3 Cam_Pos(-5.5,0.0,-65.0);
    static AMD::Vec4 Proj(3.0, 3.0, 1.0, 100.0);
    static Light_Src light_src;
    const ImGuiKey m_keys[4] ={ImGuiKey_UpArrow, ImGuiKey_DownArrow, ImGuiKey_RightArrow, ImGuiKey_LeftArrow};
    op->need_update = false;
    static char buf1[256];

    ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));


    ImGui::Begin("UI prarmeters");
    //####These are the functions that act on the Operator!!##################################
    for(int i = 0; i< num_items; i++){
        ImGui::Text("%s", m_names[i]);
        ImGui::SameLine();
        ImGui::Text(" %.2f", *(m_items[i]));
    }
    ImGui::Text("display w = %d, h = %d", Ww, Wh);// Edit 1 float using a slider from 0.0f to 1.0f
    if(abs(op->w_scale - ((float)Wh / (float)Ww)) > 0.001){
        op->need_update = true;
        ImGui::Text("%s", "window resize");
    }
    ImVec2 ui_size = ImGui::GetWindowSize();
    display_w = ui_size.x;
    display_h = ui_size.y;

    //enum File_Type {lammps, qe, jdftx, ase};
    const char* fts[4] = {"LAMMPS/dump", "Quantum Espresso", "JDFTX", "ASE/XYZ"};
    ImGui::Text("File: %s", m_input_file.c_str());
    static bool loaded = false;
    if(!loaded){
    ImGui::SameLine();
    if (ImGui::Button("Open"))
        ImGui::OpenPopup("open");
    }

    // Always center this window when appearing
    //enum File_Type {lammps, qe, jdftx, ase};
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("open", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        file_selected = ls_dir();
        if (ImGui::Button("Open", ImVec2(120, 0))) { 
            Sim->Init(m_input_file.c_str(), m_ft);
            loaded = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }


    if (ImGui::CollapsingHeader("Object View", ImGuiTreeNodeFlags_None)){
        if(ImGui::InputFloat("View X", &Cam_Pos.x, 0.05f, 1.0f, "%.2f")){op->need_update = true;}
        if(ImGui::InputFloat("View Y", &Cam_Pos.y, 0.05f, 1.0f, "%.2f")){op->need_update = true;}
        if(ImGui::InputFloat("View Z", &Cam_Pos.z, 0.05f, 1.0f, "%.2f")){op->need_update = true;}

        if(ImGui::InputFloat("Near", &Proj.b, 1.0f, 1.0f, "%.2f")){
            op->Get_Proj_vec() = Proj;
            op->need_update = true;

        }
        if(ImGui::InputFloat("Far", &Proj.a, 1.0f, 100.0f, "%.2f")){
            op->Get_Proj_vec() = Proj;
            op->need_update = true;

        }
    }

    for ( int i = 0; i < 4; i++){
        if (ImGui::IsKeyDown(m_keys[i]) && !m_io.KeyShift && !m_io.KeyAlt){

            switch (i) {
                case 0:
                    //Cam_Pos.y += 0.1;
                    op->need_update = true;
                    C_theta -= 0.1;
                    break;

                case 1:
                    //Cam_Pos.y -= 0.1;
                    op->need_update = true;
                    C_theta += 0.1;
                    break;
                case 2:
                    //op.m_Cam.Move_LeftRight(-0.01);
                    C_phi +=0.1;
                    op->need_update = true;
                    break;

                case 3:
                    //op.m_Cam.Move_LeftRight(0.01);
                    C_phi -=0.1;
                    op->need_update = true;
                    break;
                default:
                    break;
            }

        }
    }

    if (m_io.KeyShift){

        if (ImGui::IsKeyDown(ImGuiKey_UpArrow)){
            Cam_Pos.z += 0.5;
            op->need_update = true;

        }
        else if (ImGui::IsKeyDown(ImGuiKey_DownArrow)){
            Cam_Pos.z -= 0.5;
            op->need_update = true;

        }
        else if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)){
            Cam_Pos.x -= 0.5;
            op->need_update = true;
        }
        else if (ImGui::IsKeyDown(ImGuiKey_RightArrow)){
            Cam_Pos.x += 0.5;
            op->need_update = true;
        }
    }


    if(!m_io.WantCaptureMouse){
        if(ImGui::IsMouseDragging(0)){
            if(abs(m_io.MouseDelta.x) > abs(m_io.MouseDelta.y)){
                M_phi += 0.005*m_io.MouseDelta.x;
            }
            else{M_theta  += 0.005*m_io.MouseDelta.y;}
            op->need_update = true;

        }
    }
    float wheel_test = m_io.MouseWheel;
    if(wheel_test){
        Cam_Pos.z += 0.25*wheel_test;
        op->need_update = true;
    }
    if (ImGui::CollapsingHeader("Colors/Light Source", ImGuiTreeNodeFlags_None)){
        ImGui::Text("Light Color:"); ImGui::SameLine(); ImGui::Text("Clear Color:");
        float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
        ImGui::SetNextItemWidth(w);
        if(ImGui::ColorPicker4("##MyColor##2", light_src.Get_Color_ptr(), ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha)){
            op->need_update = true;
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(w);
        ImGui::ColorPicker3("##MyColor##3", CC.get(), ImGuiColorEditFlags_NoInputs);

        ImGui::InputFloat("Color Saturation", light_src.Get_Dir_ptr(), 0.01f, 1.0f, "%.2f");
        static int div1 = 0;
        static int div2 = 0;
        ImGui::VSliderInt("theta", ImVec2(30, 100), &div1, 0, 24);
        ImGui::SameLine(100.0f);
        ImGui::VSliderInt("phi", ImVec2(30, 100), &div2, 0, 24);
        theta = PI*(div1/10.0);
        phi = PI*(div2/10.0);

        light_src.Set_Pos(theta, phi);
        AMD::Vec3 light = light_src.Get_Pos();
        ImGui::Text("Camera");
        ImGui::SameLine();
        ImGui::Text("theta = %f, phi = %f", C_theta, C_phi);
        ImGui::Text("theta = %f, phi = %f", theta, phi);
        ImGui::Text("Light Source Position");
        ImGui::SameLine();
        ImGui::Text("x = %.2f, y = %.2f, z = %.2f", light.x, light.y,light.z);
        ImGui::Text("Target");
        ImGui::SameLine();
        ImGui::Text("x = %.2f, y = %.2f, z = %.2f", light_src.Get_Target().x,light_src.Get_Target().z,light_src.Get_Target().z);
        ImGui::Text("Direction");
        ImGui::SameLine();
        ImGui::Text("x = %.2f, y = %.2f, z = %.2f", light_src.Get_Direction_vec().x,light_src.Get_Direction_vec().y,light_src.Get_Direction_vec().z);
    }
    //op.m_Cam.Look_At(AMD::Vec3(0.0,0.0,10.0));

    static bool atoms = false;
    //static bool iso = false;
    static bool bonds = false;
    static bool rho = false;
    static bool wire = false;
    static bool vox = false;
    static bool vector_field = false;
    static bool hide = false;

    if (ImGui::CollapsingHeader("Draw Call/ Computes", ImGuiTreeNodeFlags_None)){
    if(ImGui::Checkbox("Draw Atoms", &atoms)){
        if(atoms){rend->Push_Call(0);}
        else{rend->Pop_Call(0);}
    }
    if(ImGui::Checkbox("Draw Dipole", &vector_field)){
        if(vector_field){rend->Push_Call(1);}
        else{rend->Pop_Call(1);}
    }
    if(vector_field){
        ImGui::SameLine();
        if(ImGui::Checkbox("Hide H2O",&hide )){
            if(hide){Sim->hide = true;}
            else{Sim->hide = false;}
        Simulation::Get()->Step_Zero();
        }
    }
    if(ImGui::Checkbox("Draw Bonds", &bonds)){
        if(vector_field){rend->Push_Call(2);}
        else{rend->Pop_Call(1);}
    }
    if(ImGui::Checkbox("Draw Density", &rho)){
        if(rho){rend->Push_Call(2);}
        else{rend->Pop_Call(2);}
    }
    if(ImGui::Checkbox("Draw Wire Frame", &wire)){
        if(wire){rend->Push_Call(3);}
        else{rend->Pop_Call(3);}
    }
    if(ImGui::Checkbox("Draw Voxel", &vox)){
        if(vox){rend->Push_Call(4);}
        else{rend->Pop_Call(4);}
    }
    }
    if(ImGui::InputFloat("Slice High", &slice_hi, 0.05f, 0.5f, "%.2f")){Sim->slice_hi = slice_hi;op->need_update = true;}
    if(ImGui::InputFloat("Slice Low", &slice_lo, 0.05f, 0.5f, "%.2f")){Sim->slice_lo = slice_lo; op->need_update = true;}
    static char save_name[128] = "none";
    static bool save = false;
    if(save){
        save = false;
        Write_Buffer(save_name);
    }
    if (ImGui::Button("SAVE"))
        ImGui::OpenPopup("Save");

    // Always center this window when appearing
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.0f, 0.5f));
    if (ImGui::BeginPopupModal("Save", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::InputText("input text", save_name, IM_ARRAYSIZE(save_name));
        if (ImGui::Button("Save", ImVec2(120, 0))) { 
            save = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }
    //ImGui::Text("%s", save_file.c_str());
    ImGui::Text("%s","Play");
    if(!play){
        if(ImGui::Button(">")){
            play = true;
        }
    }
    if(play){
        if(ImGui::Button("||")){
            play = false;
        }
        if(!(counter%100)){
            Simulation::Get()->Step_Forward();
            timestep = Simulation::Get()->Timestep();
            counter = 0;
        }
        counter++;
    }

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Timestep");
    ImGui::SameLine();

    ImGui::PushID(0);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3.0 / 7.0f, 0.6f, 0.6f));
    float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
    if (ImGui::ArrowButton("##left", ImGuiDir_Left)) {
        Simulation::Get()->Step_Bacward();
        timestep = Simulation::Get()->Timestep();
    }
    ImGui::PopStyleColor(1);
    ImGui::PopID();

    ImGui::SameLine(0.0f, spacing);

    ImGui::PushID(1);
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3.0 / 7.0f, 0.6f, 0.6f));
    if (ImGui::ArrowButton("##right", ImGuiDir_Right)) {
        Simulation::Get()->Step_Forward();
        timestep = Simulation::Get()->Timestep();
    }
    ImGui::PopStyleColor(1);
    ImGui::PopID();
    ImGui::SameLine();
    ImGui::Text(" %d",timestep);

    static float sx = 0.0;
    static float sy = 0.0;
    static float sz = 0.0;
    if(ImGui::VSliderFloat("Shift X", ImVec2(30, 100), &sx, 0.0, 1.0)){
        Simulation::Get()->shift.x = sx;
        Simulation::Get()->Step_Zero();
        timestep = Simulation::Get()->Timestep();

    }
    ImGui::SameLine(120.0f);
    if(ImGui::VSliderFloat("Shift Y", ImVec2(30, 100), &sy, 0.0, 1.0)){
        Simulation::Get()->shift.y = sy;
        Simulation::Get()->Step_Zero();
        timestep = Simulation::Get()->Timestep();

    }
    ImGui::SameLine(240.0f);
    if(ImGui::VSliderFloat("Shift z", ImVec2(30, 100), &sz, 0.0, 1.0)){
        Simulation::Get()->shift.z = sz;
        Simulation::Get()->Step_Zero();
        timestep = Simulation::Get()->Timestep();

    }
    if(err){Error_PopUp(buf1);}
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    if(op->need_update){
        op->Set_W_Scale(Ww, Wh);
        op->m_Cam.Move_To(Cam_Pos);
        op->m_Cam.Look_UpDown(C_theta);
        op->m_Cam.Look_LeftRight(C_phi);
        op->m_model.Rotate_Pole(M_theta);
        op->m_model.Rotate_Azimuth(M_phi);
    }
}



void UI_Window::Push_Item(const char* name, float* item){
    m_items[num_items] = item;
    m_names[num_items] = name;
    num_items++;
}



void UI_Window::NewFrame() const{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI_Window::render() const{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



void UI_Window::Write_Buffer(const char* file_name){
    stbi_flip_vertically_on_write(1);
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    width-=600;
    const int num_pix = 3 * width * height;
    unsigned char* pixels = new unsigned char[num_pix];

    std::fstream outfile;
    outfile.open(file_name, std::ios::out);

    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glReadBuffer(GL_FRONT);
    glReadPixels(600, 0, width , height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    if(stbi_write_png(file_name, width, height, 3, pixels, width*3*sizeof(unsigned char))){
        return;
    }
    else return;

}

UI_Window UI_Window::inst;
Renderer Renderer::inst;



