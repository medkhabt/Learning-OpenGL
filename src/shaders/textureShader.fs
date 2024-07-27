#version 330 core 
out vec4 FragColor; 

in vec3 ourColor; 
in vec2 TexCoord; 

float ambientStrength = 0.3;
vec3 ambient = ambientStrength * vec3(0.0,0.2,0.8);
uniform sampler2D ourTexture; 

void main() {
    //FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
    vec3 result = ambient * ourColor;
    FragColor = vec4(result, 1.0);
}
