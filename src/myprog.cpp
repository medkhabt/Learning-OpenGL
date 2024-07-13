#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

enum COLORS {
    ORANGE, 
    YELLOW, 
    NONE
};

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos, 1.0);\n"
" vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"}\0";

const char *fragmentShaderSourceFromVertexShader = "#version 330 core\n" 
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
" FragColor = vertexColor;\n"
"}\0"; 

const char *fragmentShaderSourceOrange = "#version 330 core\n" 
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0"; 
const char *fragmentShaderSourceYellow = "#version 330 core\n" 
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0"; 

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
unsigned int  basicVertexShader();
unsigned int basicFragmentShader(enum COLORS color);
unsigned int shaderProgram(enum COLORS color);
unsigned int drawRectangle(); 
unsigned int drawTriangle(float offsetX);

int main() {
    glfwInit(); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        printf("Failed to initialize OpenGL context\n");
        return -1;
    }

    // Successfully loaded OpenGL
    printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    int nAttributes; 
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nAttributes << std::endl;
    // vertex input 
    unsigned int VAORectangle = drawRectangle();  
    unsigned int VAOTriangle1 = drawTriangle(0.5f); 
    unsigned int VAOTriangle2 = drawTriangle(-0.5f); 

    unsigned int shaderProgramYellow = shaderProgram(YELLOW);
    unsigned int shaderProgramOrange= shaderProgram(ORANGE);
    unsigned int shaderProgramNone= shaderProgram(NONE);



// RENDER
    while(!glfwWindowShouldClose(window)){
        
        processInput(window); 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgramOrange);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(VAORectangle);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glUseProgram(shaderProgramNone);
        glBindVertexArray(VAOTriangle1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shaderProgramYellow); 
        glBindVertexArray(VAOTriangle2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);  
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAORectangle); 
    glDeleteVertexArrays(1, &VAOTriangle1); 
    glDeleteVertexArrays(1, &VAOTriangle2); 
    // TODO how to delete the buffers dynamically ? 
    //glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgramOrange); 
    glDeleteProgram(shaderProgramYellow); 
    glDeleteProgram(shaderProgramNone); 

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}
unsigned int basicVertexShader() {
     unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
     glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
     glCompileShader(vertexShader);

     int success; 
     char infoLog[512]; 
     glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); 

     if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);  
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl; 

     }
     return vertexShader;
}
unsigned int basicFragmentShader(enum COLORS color) {
    const char *fragmentShaderSource;  
     switch(color){
         case YELLOW: 
             fragmentShaderSource = fragmentShaderSourceYellow; 
             break; 
         case ORANGE: 
             fragmentShaderSource = fragmentShaderSourceOrange; 
             break; 
         case NONE: 
             fragmentShaderSource = fragmentShaderSourceFromVertexShader; 
             break;
     }
     unsigned int fragmentShader; 
     fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 
     glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); 
     glCompileShader(fragmentShader); 

     int success; 
     char infoLog[512]; 
     glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); 

     if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);  
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl; 
     }
    return fragmentShader;

}

unsigned int drawRectangle() {
    float vertices[] = {
        0.5f, 0.0f, 0.0f, 
        0.5f, 1.0f, 0.0f, 
        -0.5f, 0.0f, 0.0f, 
        -0.5f, 1.0f, 0.0f, 
    };

    unsigned int indices[] = {
        0, 1, 2, 
        1, 2, 3  
    };

    unsigned int VBO, VAO, EBO; 
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(0); 

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Don't unbind the ebo while vao is active !
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return VAO;
}
unsigned int drawTriangle(float offsetX){
    float vertices[] = {
        0.5f + offsetX, -1.0f, 0.0f, 
        0.5f + offsetX, 0.0f, 0.0f, 
        -0.5f + offsetX, -1.0f, 0.0f, 
    };

    unsigned int VBO, VAO; 
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(0); 

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Don't unbind the ebo while vao is active !
    glBindVertexArray(0);

    return VAO;
}
unsigned int shaderProgram(enum COLORS color){
     // Vertex shader
     unsigned int vertexShader = basicVertexShader(); 
     // Fragment shader 
     unsigned int fragmentShader =  basicFragmentShader(color);
     // shader program 
     unsigned int shaderProgram = glCreateProgram(); 
     glAttachShader(shaderProgram, vertexShader); 
     glAttachShader(shaderProgram, fragmentShader); 
     glLinkProgram(shaderProgram);

     int success; 
     char infoLog[512]; 
     glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); 
     if(!success){
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);  
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl; 
     }
     glDeleteShader(vertexShader); 
     glDeleteShader(fragmentShader);
     return shaderProgram;
}
