#version 330 core 
out vec4 FragColor; 

in vec3 ourColor; 
in vec3 colorPos; 

uniform sampler2D ourTexture; 
uniform vec2 resolution;
uniform vec2 rootCrd; 
uniform float timeValue; 

void main() {
    //FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
    vec2 uv= ((colorPos.xy + 1.0f)/2.0f);
    uv.x -= 0.5f; 
    //uv.y -= 1.0f;
    uv -= rootCrd;
    uv.x *= resolution.x/resolution.y;
    float l = length(uv); 
    l -= 1 * pow( sin(0.6 * timeValue), 7)/ 6;

    float d = -abs(l);
    d = smoothstep(-0.03, 0.0, d); 
    l = smoothstep(0.0, 0.03,-l); 
    //FragColor = vec4(d, l, 0.0, 1.0f);
    FragColor = vec4((5.0f+l*234.0f+d*239.0f)/255, (5.0f+195.0f*l+d*223.0f)/255, (23.0f+l*116.0f+d*155.0f)/255, 1.0f);
}
