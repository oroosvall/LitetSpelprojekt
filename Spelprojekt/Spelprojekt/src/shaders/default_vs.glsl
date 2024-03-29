#version 430
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 UV;

layout(location = 0) out vec2 UVCord;

uniform mat4 modelMatrix;
uniform mat4 VP;

void main () 
{
    UVCord = UV;
    gl_Position =  vec4(vertex_position, 1.0f);
}