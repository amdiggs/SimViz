#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec4 v_color;
layout (location = 2) in vec3 v_norm;
layout (location = 3) in float v_amb;


out vec3 f_norm;
out vec4 f_color;
out vec4 f_pos;
uniform mat4 u_MVP;
uniform mat3 u_Normal;


void main()
{

    gl_Position =  u_MVP * vec4(v_pos,1.0);
    f_norm = u_Normal * v_norm;
    f_color = v_amb*v_color;
    f_pos =  u_MVP * vec4(v_pos,1.0);
    
    

    
}
#END

/////////////////////////////////////////////////////////////////////////////////////

#SHADER FRAGMENT
#version 330 core
layout (location = 0) out vec4 color;


in vec3 f_norm;
in vec4 f_color;
in vec4 f_pos;
//Light Uniforms Struct
struct Light_Source{
    vec4 clr;
    vec3 dir;
    float sat;
    
};

uniform Light_Source l_src;

float Compute_Fog(vec4 pos){
    //float dist = abs(-65.0 - (pos.z/pos.w));
    // pos min = 40.0 pos max = 75.0 ?
    float dist = (pos.z - 40.0) / 35.0;
    return  1.0 - smoothstep(0.0,0.3,dist);
}

vec4 Fog_Color = vec4(1.0,1.0,1.0,0.5);
void main()
{
    float l_dot = 0.5*(dot(-l_src.dir,f_norm) + 1.0);
    float ma = f_color.a;
    color = l_dot*f_color;
    color.a = 1.0;
    
    
    
}

#END
