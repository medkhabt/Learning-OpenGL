#ifndef FONT_H
#define FONT_H 

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <ft2build.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <shader.h>
#include FT_FREETYPE_H 
#define STB_IMAGE_IMPLEMENTATION

struct Character {
    unsigned int TextureID; 
    glm::ivec2 Size; 
    glm::ivec2 Bearing; 
    unsigned int Advance;
};

class Font{
    public:
        std::map<char, Character> Characters;
        Shader* shader; 
        unsigned int VAO;
        unsigned int VBO;
        FT_Face face; 

        Font(Shader* shader);
        void renderText(std::string text, float x, float y, float scale, glm::vec3 color);

    private :

        FT_Library ft; 

};
#endif
