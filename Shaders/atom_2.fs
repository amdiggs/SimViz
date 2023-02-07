#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec4 v_color;
layout (location = 2) in vec3 v_norm;
layout (location = 3) in vec3 v_offset;
layout (location = 4) in float v_type;
    
out vec3 f_pos;
out vec3 f_norm;
out vec3 f_cam_dir;
out vec4 f_color;





uniform mat4 u_MV;
uniform mat4 u_MVP;
uniform mat4 u_Normal;
//uniform vec3 u_BB;


float size;
int at_type;
const float sizes[3] = float[3](0.6, 0.25, 1.4);

void main()
{

    at_type = int(v_type);
    size = sizes[at_type - 1];
    vec3 new_pos = v_offset + size*0.25*v_pos;
    gl_Position =  u_MVP * vec4(new_pos ,1.0);
    vec4 junk = u_MVP * u_MV* u_Normal * vec4(1.0);
    
}
#END
//////////////////////////////////////////////////////////////////////////////////////////////
#SHADER GEOMETRY
#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 3) out;

out vec3 f_pos;
out vec3 f_norm;
out vec3 f_cam_dir;
out vec4 f_color;


in DATA
{

    vec3 g_pos;
    vec3 g_norm;
    vec3 g_cam_dir;
    vec4 g_color;

}data_in[];



void main()
{
    //vec3 tmp = normalize(
    for(int i=0; i<3; i++)
    {
      gl_Position = gl_in[i].gl_Position;
        f_pos = data_in[i].g_pos;
        f_norm = data_in[i].g_norm;
        f_cam_dir = data_in[i].g_cam_dir;
        f_color = data_in[i].g_color;
      EmitVertex();
    }
    EndPrimitive();
}
#END
/////////////////////////////////////////////////////////////////////////////////////

#SHADER FRAGMENT
#version 330 core
layout (location = 0) out vec4 color;

in vec3 f_pos;
in vec3 f_norm;
in vec3 f_cam_dir;
in vec4 f_color;


uniform vec3 l_src;
uniform vec4 l_color;
vec3 light_dir;





void main()
{
 
    vec4 junk = l_color + vec4(l_src,1.0);
    color = vec4(0.2,0.2,0.2,1.0);
}

#END


