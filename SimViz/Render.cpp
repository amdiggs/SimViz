//
//  my_UI.cpp
//  OpenGL
//
//  Created by Andrew Diggs on 9/16/22.
//

#include "Render.hpp"
#include "Operations.hpp"
#include "shader.hpp"
#include "vertexbuffer.hpp"
#include "vertexarray.hpp"
#include "Object.hpp"

bool save = false;


ImGuiIO& init_io(){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    return io;
}





//THIS IS MY UI CLASS!!!!!!!!!!!

UI_Window::UI_Window(float pos_x, float pos_y, GLFWwindow* window)
: m_x(pos_x), m_y(pos_y), m_window(window), m_io(init_io())
{
    m_cc[0] = 1.0;
    m_cc[1] = 1.0;
    m_cc[2] = 1.0;
    m_cc[3] = 0.0;
    
    (void)m_io;
    //m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //m_io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    m_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    m_io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (m_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(m_version);
    
    
    ImFont* font = m_io.Fonts -> AddFontFromFileTTF("/System/Library/Fonts/Helvetica.ttc", 16.0);
    IM_ASSERT(font != NULL);
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    view_x = 0.0;
    view_y = 0.0;
    
    

    
  
    
}

UI_Window::~UI_Window() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}



void UI_Window::NewFrame() const{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), m_doc_flags);
}

void UI_Window::render() const{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    if (this -> m_io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}




void UI_Window::Model_UI(Operator& op, Light_Src& light_src ){
    static AMD::Vec3 trans;
    const ImGuiKey m_keys[4] ={ImGuiKey_UpArrow, ImGuiKey_DownArrow, ImGuiKey_RightArrow, ImGuiKey_LeftArrow};
    
    //ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));


    ImGui::Begin("UI prarmeters");
    //####These are the functions that act on the Operator!!##################################
 
    //ImGui::Text("display w = %d, h = %d", 100, 100);// Edit 1 float using a slider from 0.0f to 1.0f
    

    //Projection operations
    ImGui::InputFloat("Near",&op.get_proj_vec()[2] , 0.1f, 1.0f, "%.1f");
    ImGui::InputFloat("Far", &op.get_proj_vec()[3], 0.1f, 1.0f, "%.1f");
    ImGui::InputFloat("X Lim", &op.get_proj_vec()[0], 0.1f, 1.0f, "%.1f");
    ImGui::InputFloat("Y Lim", &op.get_proj_vec()[1], 0.1f, 1.0f, "%.1f");
    op.w_scale = view_y / view_x;
   
    
    
    ImGui::InputFloat("View X", &op.V_translation[0], 0.05f, 1.0f, "%.2f");
    ImGui::InputFloat("View Y", &op.V_translation[1], 0.05f, 1.0f, "%.2f");
    ImGui::InputFloat("View Z", &op.V_translation[2], 0.05f, 1.0f, "%.2f");
    
    
    ImGui::InputFloat("Model X", &op.M_Get_Trans()[0], 0.05f, 1.0f, "%.2f");
    ImGui::InputFloat("Model Y", &op.M_Get_Trans()[1], 0.05f, 1.0f, "%.2f");
    ImGui::InputFloat("Model Z", &op.M_Get_Trans()[2], 0.05f, 1.0f, "%.2f");
    
    
    for ( int i = 0; i < 4; i++){
    if (ImGui::IsKeyDown(m_keys[i]) && !m_io.KeyShift && !m_io.KeyAlt){
        
        switch (i) {
            case 0:
                op.M_Get_Trans()[1] += 0.025;
                break;
                
            case 1:
                op.M_Get_Trans()[1] -= 0.025;
                break;
            case 2:
                op.M_Get_Trans()[0] += 0.025;
                break;
                
            case 3:
                op.M_Get_Trans()[0] -= 0.025;
                break;
                
                
                
            default:
                break;
        }
        
    }
    }
    
    if (m_io.KeyShift){
        
        if (ImGui::IsKeyDown(ImGuiKey_UpArrow)){
            op.M_Get_Trans()[2] += 0.05;
            
        }
        else if (ImGui::IsKeyDown(ImGuiKey_DownArrow)){
            op.M_Get_Trans()[2] -= 0.05;
            
        }
        
    }
    
    if (m_io.KeyAlt){
        
        if (ImGui::IsKeyDown(ImGuiKey_UpArrow)){
            op.M_rotation_vec[2] = 0.01;
            op.M_set_rotation();
            
        }
        else if (ImGui::IsKeyDown(ImGuiKey_DownArrow)){
            op.M_rotation_vec[2] = -0.005;
            op.M_set_rotation();
            
        }
        else if (ImGui::IsKeyDown(ImGuiKey_RightArrow)){
            op.M_rotation_vec[1] = -0.005;
            op.M_set_rotation();
            
        }
        else if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)){
            op.M_rotation_vec[1] = 0.005;
            op.M_set_rotation();
            
        }
        else if (ImGui::IsKeyDown(ImGuiKey_P)){
            op.M_rotation_vec[0] = -0.001;
            op.M_set_rotation();
            
        }
        else if (ImGui::IsKeyDown(ImGuiKey_L)){
            op.M_rotation_vec[0] = 0.001;
            op.M_set_rotation();
            
        }
    }
    
    if(!m_io.WantCaptureMouse){
        if(ImGui::IsMouseDragging(0)){
            op.M_rotation_vec[1] = -0.01*m_io.MouseDelta.x; op.M_rotation_vec[2]  = -0.01*m_io.MouseDelta.y;
            op.M_set_rotation();
            
        }
    }
   
    
    
    


    
    
        if (ImGui::Button("SAVE")){
            save = true;
        }
    

    static float theta = 0.0;
    static float phi = 0.0;
    AMD::Vec3 light = light_src.get_light_src();
    
    ImGui::Text("The Light Source is at");
    ImGui::Text("x = %.2f, y = %.2f, z = %.2f", light[0],light[1],light[2]);
    
    ImGui::SliderFloat("Theta", &theta,0.0, 6.28, "%.2f");
    ImGui::SameLine();
    ImGui::SliderFloat("Phi", &phi,0.0, 6.28, "%.2f");
    
    light_src.set_light_dir(theta, phi);
    
    
    
    ImGui::Text("Light Color:"); ImGui::SameLine(); ImGui::Text("Clear Color:");
    float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
    ImGui::SetNextItemWidth(w);
    ImGui::ColorPicker4("##MyColor##2", light_src.get_clr_ptr(), ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(w);
    ImGui::ColorPicker3("##MyColor##3", (float*)(&m_cc), ImGuiColorEditFlags_NoInputs);
    

    
    ImGui::InputFloat("Color Saturation", light_src.get_src_ptr(), 0.01f, 1.0f, "%.2f");
    

    
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}



void UI_Window::Light_UI(Light_Src& light_src){
    static float theta = 0.0;
    static float phi = 0.0;
    AMD::Vec3 light = light_src.get_light_src();
    
    ImGui::Begin("Lighting");
    ImGui::Text("The Light Source is at");
    ImGui::Text("x = %.2f, y = %.2f, z = %.2f", light[0],light[1],light[2]);
    
    ImGui::SliderFloat("Theta", &theta,0.0, 6.28, "%.2f");
    ImGui::SameLine();
    ImGui::SliderFloat("Phi", &phi,0.0, 6.28, "%.2f");
    
    light_src.set_light_dir(theta, phi);
    
    
    
    ImGui::Text("Light Color:"); ImGui::SameLine(); ImGui::Text("Clear Color:");
    float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
    ImGui::SetNextItemWidth(w);
    ImGui::ColorPicker4("##MyColor##2", light_src.get_clr_ptr(), ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(w);
    ImGui::ColorPicker3("##MyColor##3", (float*)(&m_cc), ImGuiColorEditFlags_NoInputs);
    

    
    ImGui::InputFloat("Color Saturation", light_src.get_src_ptr(), 0.01f, 1.0f, "%.2f");
    
    ImGui::End();
}



/*
void UI_Window::log_window(Ensamble_Of_Atoms& ats){
 
    AMD::Vec3 at1;
    AMD::Vec3 at2;
    AMD::Vec3 diff;
    int a, b;
    int na, nb;
    if (ImGui::BeginTable("table1", 3, ImGuiTableFlags_Hideable))
    {
        //ImGui::BeginTable("table1", 3, ImGuiTableFlags_Hideable);
        // Submit columns name with TableSetupColumn() and call TableHeadersRow() to create a row with a header in each column.
        // (Later we will show how TableSetupColumn() has other uses, optional flags, sizing weight etc.)
        ImGui::TableSetupColumn("Atom1");
        ImGui::TableSetupColumn("Atom2");
        ImGui::TableSetupColumn("diff");
        ImGui::TableHeadersRow();
        for (int row = 0; row < ats.num_bonds; row++)
        {
            a = ats.neb_IDs[row][0]; b = ats.neb_IDs[row][1];
            ImGui::TableNextRow();
            at1 = ats.m_ats[a].get_coords(); at2 = ats.m_ats[b].get_coords();
            na = ats.m_ats[a].m_num; nb = ats.m_ats[b].m_num;
            diff = at1 - at2;
            
            if(diff.len() > 2.5){

            ImGui::TableNextColumn();
            ImGui::Text(" [%d] %.2f,%.2f, %.2f",a,at1[0],at1[1],at1[2]);
            
            ImGui::TableNextColumn();
            ImGui::Text(" [%d] %.2f,%.2f, %.2f",b,at2[0],at2[1],at2[2]);
            
            ImGui::TableNextColumn();
            ImGui::Text("%.2f,%.2f, %.2f : %.2f",diff[0], diff[1], diff[2], diff.len());
            }
            else{continue;}
            
        }
        ImGui::EndTable();

    }
    
}


void UI_Window::log_window(Bond* bd, int num){
 
    if (ImGui::BeginTable("table1", 2, ImGuiTableFlags_Hideable))
    {
        //ImGui::BeginTable("table1", 3, ImGuiTableFlags_Hideable);
        // Submit columns name with TableSetupColumn() and call TableHeadersRow() to create a row with a header in each column.
        // (Later we will show how TableSetupColumn() has other uses, optional flags, sizing weight etc.)
        ImGui::TableSetupColumn("Angles");
        ImGui::TableSetupColumn("Off Set");
        ImGui::TableHeadersRow();
        for (int row = 0; row < num; row++)
        {
            AMD::Vec3 temp1 = bd[row].get_angles();
            AMD::Vec3 temp2 = bd[row].get_off_set();
            ImGui::TableNextRow();
            

            ImGui::TableNextColumn();
            ImGui::Text("%.2f,%.2f, %.2f",temp1.x, temp1.y,temp1.z);
            
            ImGui::TableNextColumn();
            ImGui::Text("%.2f,%.2f, %.2f",temp2.x, temp2.y,temp2.z);
            
        }
        ImGui::EndTable();
    }
}

void UI_Window::log_window( AMD::Vertex* verts, int num){
    
    if (ImGui::BeginTable("table1", 4, ImGuiTableFlags_Hideable))
    {
        //ImGui::BeginTable("table1", 3, ImGuiTableFlags_Hideable);
        // Submit columns name with TableSetupColumn() and call TableHeadersRow() to create a row with a header in each column.
        // (Later we will show how TableSetupColumn() has other uses, optional flags, sizing weight etc.)
        ImGui::TableSetupColumn("Coords");
        ImGui::TableSetupColumn("Color");
        ImGui::TableSetupColumn("Normal");
        ImGui::TableSetupColumn("Text Coords");
        ImGui::TableHeadersRow();
        for (int row = 0; row < num; row++)
        {
            ImGui::TableNextRow();
            

            ImGui::TableNextColumn();
            ImGui::Text("%.2f,%.2f, %.2f",verts[row].pos.x,verts[row].pos.y,verts[row].pos.z);
            
            ImGui::TableNextColumn();
            ImGui::Text("%.2f,%.2f, %.2f, %.2f",verts[row].clr.r,verts[row].clr.g,verts[row].clr.b, verts[row].clr.a);
            
            ImGui::TableNextColumn();
            ImGui::Text("%.2f,%.2f, %.2f",verts[row].norm.x,verts[row].norm.y,verts[row].norm.z);
            
            ImGui::TableNextColumn();
            ImGui::Text("%.2f %.2f", verts[row].texture.x,verts[row].texture.y );
        }
        ImGui::EndTable();
    }
}


*/
void UI_Window::log_window( AMD::Vertex* verts, unsigned int* idx ,int num){
    int a, b, c;
    if (ImGui::BeginTable("table1", 3, ImGuiTableFlags_Hideable))
    {
        //ImGui::BeginTable("table1", 3, ImGuiTableFlags_Hideable);
        // Submit columns name with TableSetupColumn() and call TableHeadersRow() to create a row with a header in each column.
        // (Later we will show how TableSetupColumn() has other uses, optional flags, sizing weight etc.)
        ImGui::TableSetupColumn("V1");
        ImGui::TableSetupColumn("V2");
        ImGui::TableSetupColumn("V3");
        ImGui::TableHeadersRow();
        for (int row = 0; row < num; row+=3)
        {
            ImGui::TableNextRow();
            a = idx[row]; b = idx[row + 1]; c = idx[row + 2];

            ImGui::TableNextColumn();
            ImGui::Text("%d: %.2f,%.2f, %.2f",a, verts[a].pos.x,verts[a].pos.y,verts[a].pos.z);
            
            ImGui::TableNextColumn();
            ImGui::Text("%d: %.2f,%.2f, %.2f",b, verts[b].pos.x,verts[b].pos.y,verts[b].pos.z);
            
            ImGui::TableNextColumn();
            ImGui::Text("%d: %.2f,%.2f, %.2f",c, verts[c].pos.x,verts[c].pos.y,verts[c].pos.z);

        }
        ImGui::EndTable();
    }
}


void UI_Window::log_window( AMD::Mat4 mat){
    ImGui::Begin("window");
    if (ImGui::BeginTable("table1", 4, ImGuiTableFlags_Hideable))
    {
        ImGui::TableSetupColumn("1");
        ImGui::TableSetupColumn("2");
        ImGui::TableSetupColumn("3");
        ImGui::TableSetupColumn("4");
        ImGui::TableHeadersRow();
        for (int row = 0; row < 4; row++)
        {
            ImGui::TableNextRow();

            ImGui::TableNextRow();
            for (int column = 0; column < 4; column++)
            {
                ImGui::TableSetColumnIndex(column);
                ImGui::Text("%f", mat[row][column]);
            }

        }
        ImGui::EndTable();
    }
    ImGui::End();
}


void UI_Window::log_window(int nebs[][2], int num){
    if (ImGui::BeginTable("table1", 1, ImGuiTableFlags_Hideable))
    {
        ImGui::TableSetupColumn("1");
        for (int row = 0; row <num; row++)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("i = %d, j= %d", nebs[row][0] +1, nebs[row][1] +1);

        }

        
        ImGui::EndTable();
    }
}





AMD::Vec4& UI_Window::get_color() {
    return m_cc;
}


void UI_Window::mouse_drag(AMD::Vec3& vec){
    return;
}



void UI_Window::Write_Buffer(std::string file_name){
    int count = 0;
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    const int num_pix = 3 * width * height;
    unsigned char* pixels = new unsigned char[num_pix];
    
    std::fstream outfile;
    outfile.open(file_name, std::ios::out);
    
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    
    if(outfile.is_open()){
        outfile << "P3\n" << width << " " << height << "\n255" << std::endl;
        for (int i = 0; i < width*height; i++){
            outfile << int(pixels[count]) << " "
            << int(pixels[count + 1]) << " "
            << int(pixels[count + 2]) << " " << std::endl;
            count+=3;
        }
    }
    
    outfile.close();
    delete[] pixels;
    return;
    
}





Renderer::Renderer()
: m_w(1600), m_h(1600), m_cc(0.1, 0.0, 0.4, 0.0)
{   glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHintString(GLFW_COCOA_FRAME_NAME, "testframe");
    this -> m_window = glfwCreateWindow(m_w, m_h,"SimViz", NULL, NULL);
    set_context();
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    glewExperimental = GL_TRUE;
    glewInit();
    
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);
    //glCullFace(GL_BACK);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    
}


Renderer::~Renderer(){
    //if(save){Write_Curr_Buffer(save_file);}
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
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    glewExperimental = GL_TRUE;
    glewInit();
}

void Renderer::set_color(float clr[4]) {
    m_cc[0] = clr[0]; m_cc[1] = clr[1];
    m_cc[2] = clr[2]; m_cc[3] = clr[3];
}

void Renderer::set_color(AMD::Vec4 clr){
    m_cc[0] = clr[0]; m_cc[1] = clr[1];
    m_cc[2] = clr[2]; m_cc[3] = clr[3];
}

void Renderer::clear(UI_Window& ui){
    glfwGetFramebufferSize(m_window, &m_w, &m_h);
    glViewport(0, 0, m_w, m_h);
    set_color(ui.get_color());
    ui.view_x = (float)m_w;
    ui.view_y = (float)m_h;
    glClearColor(m_cc[0], m_cc[1], m_cc[2], 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Renderer::Draw(IndexBuffer& ib, VertexArray& va, Shader& sh, GLenum draw_type){
    sh.bind();
    va.bind();
    ib.bind();
    glDrawElements(draw_type,ib.get_num(), GL_UNSIGNED_INT, 0);
    
    
}



GLFWwindow* Renderer::get_window(){
    return this -> m_window;
}

int Renderer::is_open(){
    return glfwWindowShouldClose(m_window);
}

void Renderer::poll(){
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}


void Renderer::Write_Buffer(std::string file_name){
    int count = 0;
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    const int num_pix = 3 * width * height;
    unsigned char* pixels = new unsigned char[num_pix];
    
    std::fstream outfile;
    outfile.open(file_name, std::ios::out);
    
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    
    if(outfile.is_open()){
        outfile << "P3\n" << width << " " << height << "\n255" << std::endl;
        for (int i = 0; i < width*height; i++){
            outfile << int(pixels[count]) << " "
            << int(pixels[count + 1]) << " "
            << int(pixels[count + 2]) << " " << std::endl;
            count+=3;
        }
    }
    outfile.close();
    exit(0);
    return;
    
}


float Renderer::Get_Ratio(){
    return float(m_h) / float(m_w);
}

/*
int Renderer::Write_Curr_Buffer(std::string file_name){
    stbi_flip_vertically_on_write(1);
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
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
*/



