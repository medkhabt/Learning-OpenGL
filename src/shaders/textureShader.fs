#version 330 core 
out vec4 FragColor; 

in vec3 ourColor; 
in vec3 colorPos; 

uniform sampler2D ourTexture; 
uniform vec2 resolution;
uniform float timeValue; 

void main() {
    //FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
    vec2 uv= ((colorPos.xy + 1.0f)/2.0f);
    uv.x -= 0.5f; 
    uv.y -= 1.0f;
    uv.x *= resolution.x/resolution.y;
    float d = length(uv); 
    d -= sin(timeValue);
    //d = abs(d);
    //d = smoothstep(0.0, 0.1, d);
    FragColor = vec4(d, d, d, 1.0f);
}
