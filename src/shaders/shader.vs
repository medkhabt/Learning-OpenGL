#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform float offsetX; 
out vec3 ourColor;
out vec4 position;

void main() {
    gl_Position = vec4(aPos.x + offsetX, -aPos.y, aPos.z, 1.0);
    position = gl_Position;
    ourColor = aColor;
}
