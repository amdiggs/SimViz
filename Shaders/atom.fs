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


vec4 test_color;
float size;
int at_type;
const float sizes[3] = float[3](0.6, 0.25, 1.4);
vec3 cam(mat4 mat){
    return vec3(-mat[3][0],-mat[3][1],-mat[3][2]);
}

void main()
{

    at_type = int(v_type);
    size = sizes[at_type - 1];
    vec3 new_pos = v_offset + size*0.25*v_pos;
    gl_Position =  u_MVP * vec4(new_pos,1.0);
    vec4 n_norm = u_Normal * vec4(v_norm, 0.0);
    if(at_type == 1){
        test_color = vec4(0.1,0.1,0.8,1.0);
    }
    else{
        test_color = vec4(0.9,0.1,0.4,1.0);
    }
    f_color = test_color;
    f_norm = normalize(n_norm.xyz);
    f_cam_dir = normalize(cam(u_MV));
    f_pos = v_pos;
    vec4 junk = u_MVP * u_MV* u_Normal * vec4(1.0);
    
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


float diffuse_dot;
float spec_dot;


float DOT_PROD(vec3 a, vec3 b){
    return max(0.0,a.x*b.x + a.y*b.y + a.z*b.z);
}





float Get_angle(vec3 A, vec3 B){
    float c_th= dot(A,B);
    return acos(c_th);
}

vec3 Set_Angles(vec3 N_hat){
    float tx;
    vec3 z_hat = vec3(0.0, 0.0, 1.0);
    vec3 y_hat = vec3(0.0, -1.0, 0.0);
    vec3 xy = vec3(N_hat.x, N_hat.y, 0.0);
    
    
    float phi = Get_angle(xy, y_hat);
    if(N_hat.x <= -1.0){tx = 6.283185 - phi;}
    else{tx = phi;}
    float tz = Get_angle(N_hat, z_hat);
    float ty = 0.0;
    return vec3(tx,ty,tz);
    
}

mat3 ROTATION(vec3 N){
    
    vec3 ang = Set_Angles(N);
    float a = ang.x;
    float b = ang.y;
    float c = ang.z;
    
    
    //vec3 c0 = vec3(cos(a)*cos(b),sin(a)*cos(b) , -sin(b));
    //vec3 c1 = vec3(cos(a)*sin(b)*sin(c) - sin(a)*cos(c), sin(a)*sin(b)*sin(c) + cos(a)*cos(c), cos(b)*sin(c));
    //vec3 c2 = vec3(cos(a)*sin(b)*cos(c)+sin(a)*sin(c), sin(a)*sin(b)*cos(c)-cos(a)*sin(c), cos(b)*cos(c));
    
    vec3 c0 = vec3(cos(a),sin(a) , 0.0);
    vec3 c1 = vec3(-sin(a)*cos(c), cos(a)*cos(c), sin(c));
    vec3 c2 = vec3(sin(a)*sin(c), -cos(a)*sin(c), cos(b)*cos(c));
    return mat3(c0,c1,c2);
}



void main()
{
 
    light_dir = normalize(l_src);
    diffuse_dot = 0.5*dot(light_dir,f_norm) + 0.5;
    vec3 reflection = -reflect(light_dir,f_norm);
    
    
    spec_dot = pow(DOT_PROD(f_cam_dir, reflection),10.0);
    vec3 temp_color = diffuse_dot*(vec3(l_color.xyz) + 1.0*spec_dot*vec3(1.0));
    color = vec4(abs(f_norm.x),abs(f_norm.y),abs(f_norm.z),1.0);
    color = f_color;
}

#END


