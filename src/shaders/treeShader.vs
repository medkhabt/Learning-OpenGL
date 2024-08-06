#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model; 
uniform mat4 projection;
uniform mat4 zoom; 
uniform mat4 treeAnimation;  

out vec3 colorPos;

void main() {
    gl_Position =  projection * zoom * model *  treeAnimation * vec4(aPos, 1.0);
    colorPos = gl_Position.xyz; 
}
