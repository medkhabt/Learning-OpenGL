# version 330 core 
layout (location = 0) in vec4 vertex; // vec2 pos and vec2 for tex
out vec2 TexCoords; 

uniform mat4 projection; 
uniform mat4 model; 
uniform mat4 zoom; 

void main(){
    gl_Position =   projection * zoom * model * vec4(vertex.xy, 0.0, 1.0); 
    TexCoords = vertex.zw;
}
