#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec4 v_color;
layout (location = 2) in vec3 v_norm;
layout (location = 3) in vec2 v_tex;
layout (location = 4) in float v_layer;
layout (location = 5) in vec3 v_offset;
layout (location = 6) in vec3 v_rot;
layout (location = 7) in float v_scale;


out vec3 f_norm;
out vec4 f_color;
out vec4 f_pos;
uniform mat4 u_MVP;
uniform mat3 u_Normal;


vec3 Rotate(vec3 ang, vec3 pos){
    float a = ang.x;
    float b = ang.y;
    float c = ang.z;

    vec3 r0 = vec3(cos(c),-1.0*sin(c)*cos(a), sin(c)*sin(a));
    vec3 r1 = vec3(sin(c),cos(c)*cos(a), -cos(c)*sin(a));
    vec3 r2 = vec3(-sin(b), sin(a), cos(a));
    
    float x = dot(r0,pos);
    float y = dot(r1,pos);
    float z = dot(r2,pos);
    return vec3(x,y,z);
}


void main()
{

    vec3 new_pos = v_offset + 0.33*Rotate(v_rot,v_pos);
    gl_Position =  u_MVP * vec4(new_pos,1.0);
    f_norm = u_Normal * Rotate(v_rot,v_norm);
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
    color = l_dot*f_color;
    color = vec4(0.0,0.0,0.0,1.0);
    color.a = 1.0;
}

#END


/*
temp_clr.a = 1.0;
float ff = Compute_Fog(f_pos);
temp_clr = mix(Fog_Color, temp_clr, ff);
color = temp_clr;
*/
