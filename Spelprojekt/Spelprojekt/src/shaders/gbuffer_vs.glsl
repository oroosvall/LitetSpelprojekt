#version 430
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 UV;

layout(location = 0) out vec2 UVCord;

void main () 
{
    UVCord = UV;
    gl_Position =  vec4(vertex_position, 1);
}