#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 v_pos;


uniform mat4 u_MVP;

out DATA
{
    float g_val;
    
}data_out;

void main()
{
    gl_Position = u_MVP * vec4(v_pos,1.0);
    data_out.g_val = v_pos.z;
    
}
#END

#SHADER GEOMETRY
#version 330 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

out float val;
in DATA
{

    float g_val;
}data_in[];

vec3 CAM = vec3(0.0,0.0,-15.0);

vec4 OFFSET(){
    vec4 dir = gl_in[1].gl_Position - gl_in[0].gl_Position;
    vec3 tmp = 0.1f*normalize(cross(dir.xyz,CAM));
    return vec4(tmp,0.0);
}


void main()
{
    vec4 offset = OFFSET();
    gl_Position = gl_in[0].gl_Position + offset;
    val = data_in[0].g_val;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position - offset;
    val = data_in[0].g_val;
    EmitVertex();
    
    gl_Position = gl_in[1].gl_Position + offset;
    val = data_in[1].g_val;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position - offset;
    val = data_in[1].g_val;
    EmitVertex();
    EndPrimitive();
}
#END




#SHADER FRAGMENT
#version 330 core
layout (location = 0) out vec4 color;

in float val;
vec2 dims = vec2(3800.0, 1685.0);
uniform vec2 MM;

float Comp_Red(){
    float y = 1.0f / (MM.y - MM.x);
    float b = -1.0f*y*MM.x;
    float fr = val*y + b;
    fr *= 10.0f;
    float r2 = floor(fr);
    return r2 /10.0;
}

void main()
{
    float r = Comp_Red();
    float b = 1.0f - r;
    color = vec4(r,0.0,b,1.0);
}

#END
