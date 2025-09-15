#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec4 v_color;
layout (location = 2) in vec3 v_offset;
layout (location = 3) in float v_rad;


out vec3 f_norm;
out vec4 f_color;
out vec4 f_pos;
uniform mat4 u_MVP;
uniform mat3 u_Normal;


void main()
{

    vec3 new_pos = v_offset + v_rad*v_pos;
    gl_Position =  u_MVP * vec4(new_pos,1.0);
    f_norm = u_Normal * v_pos;
    f_color = v_color;
    f_pos =  u_MVP * vec4(new_pos,1.0);
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
    float dz = (pos.z - 40.0) / 35.0;
    float dx = abs(pos.x - 3.0)/55.0;
    float dist = dx + dz;
    return smoothstep(0.0,1.0,dist);
}

vec4 Fog_Color = vec4(0.8,0.60,0.60,0.5);
void main()
{
    float l_dot = 0.5*(dot(-l_src.dir,f_norm) + 1.0);
    float ma = f_color.a;
    float ff = Compute_Fog(f_pos);
    color = (1.0 - ff)*l_dot*f_color + ff*Fog_Color;
    color.a = 1.0;
}

#END


/*
temp_clr.a = 1.0;
float ff = Compute_Fog(f_pos);
temp_clr = mix(Fog_Color, temp_clr, ff);
color = temp_clr;
*/
