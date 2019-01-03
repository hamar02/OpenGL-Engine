#version 430

layout(location=0) in vec3 aPos;

uniform mat4 lightSpace_matrix;
uniform mat4 world_matrix;

void main(){
    gl_Position = lightSpace_matrix*world_matrix*vec4(aPos, 1.0);
}