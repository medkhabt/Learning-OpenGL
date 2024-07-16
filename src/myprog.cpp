#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <tree.h>
#include <node.h>

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void drawNode(int position, int level, int maxVertexesPerLevel, unsigned int shaderID, unsigned int shapeID);

int main() {
    glfwInit(); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
    GLFWwindow* window = glfwCreateWindow(800, 600, "Tree", NULL, NULL);
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



    Node* s = new Node(NULL, "S", 0, 0); 
    Node* a = new Node(s, "A", 0, 0); 
    Node* a1 = new Node(a, "A1", 0, 0); 
    Node* a2 = new Node(a, "A2", 0, 0); 
    Node* b = new Node(s, "B", 0, 0); 
    Node* c = new Node(s, "C", 0, 0); 
    Node* c1 = new Node(c, "C1", 0, 0); 
    Node* c2 = new Node(c, "C2", 0, 0); 
    Node* c3 = new Node(c, "C3", 0, 0); 
    Node* c11 = new Node(c1, "C11", 0, 0); 
    Node* c12 = new Node(c1, "C12", 0, 0); 
    Node* c13 = new Node(c1, "C13", 0, 0); 
    Node* c14 = new Node(c1, "C14", 0, 0); 
    Node* c15 = new Node(c1, "C15", 0, 0); 

    std::cout << "TEST file:: Creating tree" << std::endl;
    Shader treeShader("./src/shaders/treeShader.vs", "./src/shaders/treeShader.fs");
    Tree* t =  new Tree(&treeShader, s, 40, 60, 60);
    std::cout << "TEST file:: positionTree execution"<< std::endl;
    //delete s; 

    //Node *root = new Node(); 
    // Node *a = new Node(root);
    //Node *b = new Node(root); 
    //new Node(root); 
    //new Node(root); 
    //new Node(root); 
    //new Node(a); 
    //new Node(b); 
    //Tree tree(&treeShader, root, 4, 4);

    float width = 1.0f ;
    float vertices[] = {
        width, -width, 0.0f, 
        width, width, 0.0f, 
        -width, width, 0.0f, 
        -width, -width, 0.0f, 
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(0); 

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Don't unbind the ebo while vao is active !
    glBindVertexArray(0);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // RENDER
    while(!glfwWindowShouldClose(window)){

        processInput(window); 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        t->positionTree(s);

        // treeShader.use();
        // glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);
        //t->drawTree(); 

        glfwSwapBuffers(window);  
        glfwPollEvents();
    }
    t->deleteAll();
    // TODO how to delete the buffers dynamically ? 
    //glDeleteBuffers(1, &VBO);
    delete a;
    delete a1;
    delete a2;
    delete b; 
    delete c; 
    delete c1; 
    delete c2; 
    delete c3; 
    delete c11; 
    delete c12; 
    delete c13; 
    delete c14; 
    delete c15; 
    delete t;

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

