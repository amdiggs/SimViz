#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec4 v_color;
layout (location = 2) in vec3 v_norm;
//layout (location = 3) in vec2 v_tex;
//layout (location = 4) in float indx;

out DATA
{
    vec3 g_pos;
    vec3 g_norm;
    vec3 g_cam_dir;
    vec4 g_color;


}data_out;


uniform mat4 u_MV;
uniform mat4 u_MVP;
uniform mat4 u_Normal;





vec3 cam(mat4 mat){
    return vec3(-mat[3][0],-mat[3][1],-mat[3][2]);
}

void main()
{

    gl_Position = u_MVP * vec4(v_pos ,1.0);
    vec4 n_norm = u_Normal * vec4(v_norm, 0.0);
    data_out.g_color = v_color;
    data_out.g_norm = normalize(n_norm.xyz);
    data_out.g_cam_dir = normalize(cam(u_MV));
    data_out.g_pos = v_pos;
    vec4 junk = u_MVP * u_MV* u_Normal * vec4(1.0);
    
}
#END
//////////////////////////////////////////////////////////////////////////////////////////////
#SHADER GEOMETRY
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

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
    color = vec4(temp_color,1.0);
}

#END


