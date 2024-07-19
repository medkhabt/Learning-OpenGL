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
        Font(Shader* shader): shader(shader){
            FT_Library ft; 
            if(FT_Init_FreeType(&ft)){
                std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            }
            FT_Face face; 
            if(FT_New_Face(ft, ".resources/fonts/Roboto-Regular.ttf", 0, &face))

                std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

            FT_Set_Pixel_Sizes(face, 0, 48);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // just ascii for now.
            for (unsigned char c = 0; c < 128; c++){
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

            glGenVertexArrays(1, &this->VAO); 
            glGenBuffers(1, &this->VBO); 
            glBindVertexArray(VAO); 
            glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*4, NULL, GL_DYNAMIC_DRAW); 
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0); 
            glBindBuffer(GL_ARRAY_BUFFER, 0); 
            glBindVertexArray(0);
        }    
        void renderText(std::string text, float x, float y, float scale, glm::vec3 color){
            shader->use();
            glUniform3f(glGetUniformLocation(this->shader->ID, "textColor"), color.x, color.y, color.z);
            glActiveTexture(GL_TEXTURE0); 
            glBindVertexArray(this->VAO);

            size_t nc = text.size(); 
            std::string::const_iterator c; 
            size_t originC = nc/2; 
            bool isOdd = (nc%2); 
            float xAdv, xBck; 
            xAdv = x; 
            xBck = x; 
            for (size_t i=originC; i < text.size(); i++){
                Character ch = Characters[text[i]]; 
                Character chMirr = Characters[text[originC - (i - originC) - (isOdd ? 0 : 1)]]; 
                if(!isOdd) {
                    xBck-= (chMirr.Advance >> 6) * scale; 
                }

                float xpos = xAdv - (isOdd ? ch.Size.x * scale / 2 : 0) + ch.Bearing.x * scale; 
                float ypos = y - (ch.Size.y - ch.Bearing.y) * scale; 
                float xposMirr = xBck  - (isOdd ? chMirr.Size.x * scale / 2 : 0) + chMirr.Bearing.x * scale; 
                float yposMirr = y - (chMirr.Size.y - chMirr.Bearing.y) * scale; 

                float w = ch.Size.x * scale; 
                float h = ch.Size.y * scale; 
                float wMirr = chMirr.Size.x * scale; 
                float hMirr = chMirr.Size.y * scale; 

                float vertices[6][4] = {
                    {xpos, ypos + h, 0.0f, 0.0f}, 
                    {xpos, ypos, 0.0f, 1.0f}, 
                    {xpos + w, ypos, 1.0f, 1.0f}, 

                    {xpos, ypos + h, 0.0f, 0.0f}, 
                    {xpos + w, ypos, 1.0f, 1.0f}, 
                    {xpos + w, ypos + h, 1.0f, 0.0f},

                }; 
                glm::mat4 projection = glm::mat4(1.0f);
                projection = glm::ortho(-400.0f, 400.0f, -225.0f, 225.0f); 
                int projectionLoc = glGetUniformLocation(this->shader->ID, "projection");
                glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
                glBindTexture(GL_TEXTURE_2D, ch.TextureID); 
                glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                glBindBuffer(GL_ARRAY_BUFFER, 0); 

                glDrawArrays(GL_TRIANGLES, 0, 6);

                if(!(i == originC &&  isOdd)){
                    //if(false){
                    float mirrVertices[6][4] = {
                        {xposMirr, yposMirr + hMirr, 0.0f, 0.0f}, 
                        {xposMirr, yposMirr, 0.0f, 1.0f}, 
                        {xposMirr + wMirr, yposMirr, 1.0f, 1.0f}, 

                        {xposMirr, yposMirr + hMirr, 0.0f, 0.0f}, 
                        {xposMirr + wMirr, yposMirr, 1.0f, 1.0f}, 
                        {xposMirr + wMirr, yposMirr + hMirr, 1.0f, 0.0f} 
                    };

                    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
                    glBindTexture(GL_TEXTURE_2D, chMirr.TextureID); 
                    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(mirrVertices), mirrVertices);
                    glBindBuffer(GL_ARRAY_BUFFER, 0); 

                    glDrawArrays(GL_TRIANGLES, 0, 6);
                }
                xAdv+= (ch.Advance >> 6) * scale; 
                xBck-= (chMirr.Advance >> 6) * scale; 
                //xBck-= (chMirr.Advance >> 6) * scale; 
                }

                glBindVertexArray(0);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        };
#endif
