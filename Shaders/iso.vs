#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec4 v_clr;
layout (location = 2) in float val;


uniform mat4 u_MVP;
out VS_OUT{
    float gs_val;
} vs_out;

void main()
{
    gl_Position = u_MVP * vec4(v_pos ,1.0);
    vs_out.gs_val = val;
}
#END
//#####################################################################################
#SHADER GEOMETRY
#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

out float fs_val;
in VS_OUT{
    float gs_val;
}gs_in[];


void main()
{

    gl_Position = gl_in[0].gl_Position;
    fs_val = gs_in[0].gs_val;
    EmitVertex();
    
    gl_Position = gl_in[1].gl_Position;
    fs_val = gs_in[0].gs_val;
    EmitVertex();
    
    gl_Position = gl_in[2].gl_Position;
    fs_val = gs_in[0].gs_val;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position;
    fs_val = gs_in[0].gs_val;
    EmitVertex();
    
    EndPrimitive();
}
#END

#SHADER FRAGMENT
#version 330 core
layout (location = 0) out vec4 color;

in float fs_val;


void main()
{
    color = vec4(fs_val,0.1,1.0 - fs_val,1.0);
}

