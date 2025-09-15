#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 v_pos;


uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(v_pos,1.0);
    
}
#END

#SHADER GEOMETRY
#version 330 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

out vec4 f_pos;

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
    f_pos = gl_in[0].gl_Position + offset;
    EmitVertex();
    
    gl_Position = gl_in[0].gl_Position - offset;
    f_pos = gl_in[0].gl_Position - offset;
    EmitVertex();
    
    gl_Position = gl_in[1].gl_Position + offset;
    f_pos = gl_in[1].gl_Position + offset;
    EmitVertex();
    
    gl_Position = gl_in[1].gl_Position - offset;
    f_pos = gl_in[1].gl_Position - offset;
    EmitVertex();
    
    EndPrimitive();
}
#END




#SHADER FRAGMENT
#version 330 core
layout (location = 0) out vec4 color;
in vec4 f_pos;
float Compute_Fog(vec4 pos){
    //float dist = abs(-65.0 - (pos.z/pos.w));
    // pos min = 40.0 pos max = 75.0 ?
    float dz = (pos.z - 40.0) / 35.0;
    float dx = abs(pos.x - 3.0)/55.0;
    float dist = dx + dz;
    return smoothstep(0.0,1.0,dist);
}

vec4 Fog_Color = vec4(1.0,1.0,1.0,0.5);


void main()
{
    vec4 fog = Compute_Fog(f_pos)*Fog_Color;
    color = vec4(0.2,0.2,0.2,0.65) + fog;
}

#END

