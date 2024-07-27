#version 330 core 

out vec4 FragColor;
in  vec3 colorPos ;
void main() {
    float d = length(colorPos.xy);
    vec3 final = (colorPos.yzx + 1.0f)/2.0f;
    d -= 0.5f; 
    d = abs(d);
    d = smoothstep(0.0, 0.1, d);
    FragColor = vec4(1.0, 0.0f, 0.0f, 1.0f);
}
