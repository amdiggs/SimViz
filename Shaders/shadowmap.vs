#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec4 v_color;
layout (location = 2) in vec3 v_norm;
layout (location = 3) in vec2 v_texCoords;
layout (location = 4) in float v_index;

uniform mat4 u_MLP;
 
void main()
{

    
    gl_Position = u_MLP*vec4(v_pos,1.0);

}
#END


#SHADER FRAGMENT
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0,0.0,0.0,1.0);
}

#END


