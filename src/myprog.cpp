#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include <math.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#define STB_IMAGE_IMPLEMENTATION
#include <shader.h>
#include <tree.h>
#include <node.h>
#include <font.h>

unsigned int VAO, VBO; 

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void drawNode(int position, int level, int maxVertexesPerLevel, unsigned int shaderID, unsigned int shapeID);
unsigned int buildRectangle(); 
volatile std::sig_atomic_t stop;
void handle_signal(int signal) {
    if (signal == SIGINT) {
        std::cout << "Interrupt signal received. Cleaning up..." << std::endl;
        stop = 1;
    }
}
int main() {
    std::signal(SIGINT, handle_signal);
    glfwInit(); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
    GLFWwindow* window = glfwCreateWindow(1200, 720, "Tree", NULL, NULL);
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

    Shader fontShader("./src/shaders/textShader.vs", "./src/shaders/textShader.fs");
    Font* font = new Font(&fontShader); 


    Node* s = new Node(NULL, "S", 0, 200, font); 
    Node* a = new Node(s, "A", 0, 0, font); 
    Node* a1 = new Node(a, "A1", 0, 0, font); 
    Node* a2 = new Node(a, "A2", 0, 0, font); 
    Node* b = new Node(s, "B", 0, 0, font); 
    Node* b1 = new Node(b, "B1", 0, 0, font); 
    Node* b2 = new Node(b, "B2", 0, 0, font); 
    Node* c = new Node(s, "C", 0, 0, font); 
    Node* c1 = new Node(c, "C1", 0, 0, font); 
    Node* c2 = new Node(c, "C2", 0, 0, font); 
    Node* c3 = new Node(c, "C3", 0, 0, font); 
    Node* c11 = new Node(c1, "C11", 0, 0, font); 
    Node* c12 = new Node(c1, "C12", 0, 0, font); 
    Node* c13 = new Node(c1, "C13", 0, 0, font); 
    Node* c14 = new Node(c1, "C14", 0, 0, font); 
    Node* c141 = new Node(c14, "C141", 0, 0, font); 
    Node* c1411 = new Node(c141, "C1411", 0, 0, font); 
    Node* c1412 = new Node(c141, "C1412", 0, 0, font); 
    Node* c1413 = new Node(c141, "C1413", 0, 0, font); 
    Node* c1414 = new Node(c141, "C1414", 0, 0, font); 
    Node* c1415 = new Node(c141, "C1415", 0, 0, font); 
    Node* c142 = new Node(c14, "C142", 0, 0, font); 
    Node* d = new Node(s, "D", 0, 0, font); 
    Node* e = new Node(s, "E", 0, 0, font); 
    Node* f = new Node(s, "F", 0, 0, font); 
    Node* g = new Node(s, "G", 0, 0, font); 

    std::cout << "TEST file:: Creating tree" << std::endl;
    Shader treeShader("./src/shaders/treeShader.vs", "./src/shaders/treeShader.fs");
    Tree* t =  new Tree(&treeShader, s, 40, 60, 60);
    t->positionTree();
    std::cout << "TEST file:: positionTree execution"<< std::endl;



    Shader textureShader("./src/shaders/textureShader.vs", "./src/shaders/textureShader.fs");


    // TEXTURE 

    int width, height, nrChannels; 
    unsigned char *data = stbi_load(".resources/container.jpg", &width, &height, &nrChannels, 0);


    unsigned int texture; 
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl; 
    }
    stbi_image_free(data);

    unsigned int indices[] = {
        0, 1, 2, 
        0, 2, 3  
    };

    unsigned int VAOT, VBOT, EBO; 

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // RENDER
    while(!glfwWindowShouldClose(window)){

        processInput(window); 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        VAOT = buildRectangle();
        textureShader.use();
        //glBindTexture(GL_TEXTURE_2D, texture); 
        glBindVertexArray(VAOT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        t->drawTree();
        //font->renderText("C1415", 60.0f - 10.0f, -100.0f + 10.0f, 0.2f, glm::vec3(0.5f, 0.8f, 0.2f));
        //font->renderText("C11", -210.0f - 10.0f, 20.0f + 10.0f, 0.2f, glm::vec3(0.5f, 0.8f, 0.2f));
        //font->renderText("A2", -150.0f - 10.0f , 80.0f + 10.0f , 0.2f, glm::vec3(0.5f, 0.8f, 0.2f));
        //font->renderText("A", -180.0f- 10.0f, 140.0f + 10.0f, 0.2f, glm::vec3(0.5f, 0.8f, 0.2f));
        //font->renderText("(C) Medkha.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3f, 0.7f, 0.9f));

        glfwSwapBuffers(window);  
        glfwPollEvents();
        if(stop){
            return 0;  
        }
    }
    t->deleteAll();
    // TODO how to delete the buffers dynamically ? 
    //glDeleteBuffers(1, &VBO);
    delete a;
    delete a1;
    delete a2;
    delete b; 
    delete b1; 
    delete b2; 
    delete c; 
    delete c1; 
    delete c2; 
    delete c3; 
    delete c11; 
    delete c12; 
    delete c13; 
    delete c14; 
    delete c141; 
    delete c142; 
    delete c1411;
    delete c1412;
    delete c1413;
    delete c1414;
    delete c1415;
    delete d; 
    delete e; 
    delete f; 
    delete g; 
    delete t;
    delete font;

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

unsigned int buildRectangle() {
    // Addind the spacing in the calculation ( half of the node width )
    float vertices[] = {
        1.0f, 1.0f, 0.0f, 0.8f, 0.6f, 0.1f, 1.0f, 1.0f, 
        1.0f, -1.0f, 0.0f, 0.8f, 0.6f, 0.1f, 1.0f, 0.0f, 
        -1.0f, -1.0f, 0.0f, 0.8f, 0.6f, 0.1f, 0.0f, 0.0f, 
        -1.0f, 1.0f, 0.0f, 0.8f, 0.6f, 0.1f, 0.0f, 1.0f 
    };

    unsigned int indices[] = {
        0, 1, 2, 
        0, 2, 3  
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float)*3)); 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));

    glEnableVertexAttribArray(0); 
    glEnableVertexAttribArray(1); 
    glEnableVertexAttribArray(2); 

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Don't unbind the ebo while vao is active !
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return VAO;
}
