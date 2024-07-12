#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n" 
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0"; 

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 

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


     // Vertex shader
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

     // Fragment shader 

     unsigned int fragmentShader; 
     fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 
     glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); 
     glCompileShader(fragmentShader); 

     glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); 

     if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);  
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl; 
     }

     // shader program 
     unsigned int shaderProgram = glCreateProgram(); 
     glAttachShader(shaderProgram, vertexShader); 
     glAttachShader(shaderProgram, fragmentShader); 
     glLinkProgram(shaderProgram);

     glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); 
     if(!success){
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);  
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl; 
     }
     glDeleteShader(vertexShader); 
     glDeleteShader(fragmentShader);


    // vertex input 
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 
        0.5f, -0.5f, 0.0f, 
        0.0f, 0.5, 0.0f 
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
    glBindVertexArray(0);


// RENDER
    while(!glfwWindowShouldClose(window)){

        processInput(window); 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram); 
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);  
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO); 
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram); 

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
