#version 330 core 

out vec4 FragColor;

in vec3 ourColor; 
in vec4 position;
void main() {
    FragColor = vec4(position.xyz, 1.0f);
}
