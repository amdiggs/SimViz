#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 v_pos;


out float val;
uniform mat4 u_MVP;

void main()
{
    
    gl_Position = u_MVP * vec4(v_pos,1.0);
    val = v_pos.z;
    
}
#END


#SHADER FRAGMENT
#version 330 core
layout (location = 0) out vec4 color;

in float val;
uniform vec2 MM;

float Comp_Red(){
    float y = 1.0f / (MM.y - MM.x);
    float b = -1.0f*y*MM.x;
    float fr = val*y + b;
    fr *= 10.0f;
    float r2 = floor(fr);
    bool equi = (mod(fr,1.0f) > 0.04);
    if(equi){
        return r2/10.0;
    }
    else{return 2.0f;}
}


float WHITE(float r){
    float x0 = r - 0.55;
    float arg = -80.0*x0*x0;
    return 0.5*exp(arg);
}

void main()
{
    
    float mr = Comp_Red();
    if(mr > 1.1){
        color = vec4(0.2 , 0.2 , 0.2, 1.0);
    }
    else{
        float red = smoothstep(0.5,0.9,mr);
        float mb = 1.0 - mr;
        float blue = smoothstep(0.5,0.9,mb);
        float mg = smoothstep(0.2,0.8,mr);
        float w = 1.0 - (red+blue); //WHITE(mr);
        color = vec4(red + 0.5*w , w , blue + 0.5*w, 1.0);
    }
}

#END
