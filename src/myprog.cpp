#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <map>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include FT_FREETYPE_H 
#include <shader.h>
#include <tree.h>
#include <node.h>


struct Character {
    unsigned int TextureID; 
    glm::ivec2 Size; 
    glm::ivec2 Bearing; 
    unsigned int Advance;
};

std::map<char, Character> Characters;
unsigned int VAO, VBO; 

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void drawNode(int position, int level, int maxVertexesPerLevel, unsigned int shaderID, unsigned int shapeID);
void renderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);
unsigned int buildRectangle(); 

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



    Node* s = new Node(NULL, "S", 0, 200); 
    Node* a = new Node(s, "A", 0, 0); 
    Node* a1 = new Node(a, "A1", 0, 0); 
    Node* a2 = new Node(a, "A2", 0, 0); 
    Node* b = new Node(s, "B", 0, 0); 
    Node* b1 = new Node(b, "B1", 0, 0); 
    Node* b2 = new Node(b, "B2", 0, 0); 
    Node* c = new Node(s, "C", 0, 0); 
    Node* c1 = new Node(c, "C1", 0, 0); 
    Node* c2 = new Node(c, "C2", 0, 0); 
    Node* c3 = new Node(c, "C3", 0, 0); 
    Node* c11 = new Node(c1, "C11", 0, 0); 
    Node* c12 = new Node(c1, "C12", 0, 0); 
    Node* c13 = new Node(c1, "C13", 0, 0); 
    Node* c14 = new Node(c1, "C14", 0, 0); 
    Node* c141 = new Node(c14, "C141", 0, 0); 
    Node* c1411 = new Node(c141, "C1411", 0, 0); 
    Node* c1412 = new Node(c141, "C1412", 0, 0); 
    Node* c1413 = new Node(c141, "C1413", 0, 0); 
    Node* c1414 = new Node(c141, "C1414", 0, 0); 
    Node* c1415 = new Node(c141, "C1415", 0, 0); 
    Node* c142 = new Node(c14, "C142", 0, 0); 
    Node* d = new Node(s, "D", 0, 0); 
    Node* e = new Node(s, "E", 0, 0); 
    Node* f = new Node(s, "F", 0, 0); 
    Node* g = new Node(s, "G", 0, 0); 

    std::cout << "TEST file:: Creating tree" << std::endl;
    Shader treeShader("./src/shaders/treeShader.vs", "./src/shaders/treeShader.fs");
    Tree* t =  new Tree(&treeShader, s, 40, 60, 60);
    t->positionTree();
    std::cout << "TEST file:: positionTree execution"<< std::endl;


    //************************* text
    //FT_Library ft; 
    //if(FT_Init_FreeType(&ft)){
    //    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    //}
    //FT_Face face; 
    //if(FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
    //std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    //FT_Set_Pixel_Sizes(face, 0, 48);
    /*for (unsigned char c = 0; c < 128; c++){
      if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
      std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
      continue; 
      }
      unsigned int texture; 
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(
      GL_TEXTURE_2D, 
      0,
      GL_RED, 
      face->glyph->bitmap.width, 
      face->glyph->bitmap.rows, 
      0,
      GL_RED,
      GL_UNSIGNED_BYTE, 
      face->glyph->bitmap.buffer
      );
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      Character character = {
      texture, 
      glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), 
      glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), 
      static_cast<unsigned int>(face->glyph->advance.x)
      };
      Characters.insert(std::pair<char, Character>(c,character));
      }

      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      FT_Done_Face(face); 
      FT_Done_FreeType(ft);

      glEnable(GL_BLEND); 
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

      glGenVertexArrays(1, &VAO); 
      glGenBuffers(1, &VBO); 
      glBindVertexArray(VAO); 
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*4, NULL, GL_DYNAMIC_DRAW); 
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0); 
      glBindBuffer(GL_ARRAY_BUFFER, 0); 
      glBindVertexArray(0);
      */

    //Shader shader("./src/shaders/textShader.vs", "./src/shaders/textShader.fs");
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
        glBindTexture(GL_TEXTURE_2D, texture); 
        glBindVertexArray(VAOT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        t->drawTree();
        // renderText(shader, "This is a sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));

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

    glfwTerminate();

    return 0;
}

void renderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color){
    s.use();
    glUniform3f(glGetUniformLocation(s.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0); 
    glBindVertexArray(VAO);

    std::string::const_iterator c; 
    for (c=text.begin(); c != text.end(); c++){
        Character ch = Characters[*c]; 

        float xpos = x + ch.Bearing.x * scale; 
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale; 

        float w = ch.Size.x * scale; 
        float h = ch.Size.y * scale; 

        float vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f}, 
            {xpos, ypos, 0.0f, 1.0f}, 
            {xpos + w, ypos, 1.0f, 1.0f}, 

            {xpos, ypos + h, 0.0f, 0.0f}, 
            {xpos + w, ypos, 1.0f, 1.0f}, 
            {xpos + w, ypos + h, 1.0f, 0.0f} 
        }; 
        glBindTexture(GL_TEXTURE_2D, ch.TextureID); 
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x+= (ch.Advance >> 6) * scale; 
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
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
