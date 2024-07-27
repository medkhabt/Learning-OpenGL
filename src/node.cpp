#include <node.h>
#include <map>
#include <font.h>

void Node::buildNode() {
    // Addind the spacing in the calculation ( half of the node width )
    // ------ BUILDING RECTANGLE --------
    float height = 20.0f; 

    float vertices[] = {
        this->width / 2, -this->height/ 2, 0.0f,
        this->width / 2, this->height/ 2, 0.0f,
        -this->width / 2, this->height / 2, 0.0f,
        -this->width / 2, -this->height / 2, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2, 
        0, 2, 3  
    };


    unsigned int VBO, EBO; 
    unsigned int VAO; 
    glGenVertexArrays(1, &VAO); 
    std::cout << "1-" << glGetError() << std::endl;
    this->VAOs[(VAOTYPE)RECTANGLE] = VAO;
    glGenBuffers(1, &VBO);
    std::cout << "2-" << glGetError() << std::endl;
    glGenBuffers(1, &EBO);
    std::cout << "3-" << glGetError() << std::endl;

    glBindVertexArray(this->VAOs[(VAOTYPE)RECTANGLE]);
    std::cout << "4-" << glGetError() << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    std::cout << "5-" << glGetError() << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    std::cout << "6-" << glGetError() << std::endl;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
    std::cout << "7-" << glGetError() << std::endl;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    std::cout << "8-" << glGetError() << std::endl;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
    std::cout << "9-" << glGetError() << std::endl;
    glEnableVertexAttribArray(0); 
    std::cout << "10-" << "glgeterror:" <<glGetError() << std::endl;
    // ------- BUILDING THE ARROW ----------

    float verticesArrow[] = {
        0.0f, 0.0f, 0.0f,
        -10.0f, 0.0f, 0.0f, 

    }; 
    unsigned int VBOA;
    glGenVertexArrays(1, &this->VAOs[(VAOTYPE)ARROW]); 
    glGenBuffers(1, &VBOA);

    glBindVertexArray(this->VAOs[(VAOTYPE)ARROW]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOA);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesArrow), verticesArrow, GL_STATIC_DRAW); 


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(0); 

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    // ------- BUILDING NAME OF THE NODE ----------
    //glEnable(GL_TEXTURE_2D);
    FT_Library ft; 
    FT_Face face; 

    if(FT_Init_FreeType(&ft)){
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
    if(FT_New_Face(ft, ".resources/fonts/Roboto-Regular.ttf", 0, &face))

        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    FT_Set_Pixel_Sizes(face, 0, 48);
    std::map<char, Character> Characters;
    Character character; 
    // TODO this might be not correct, in case of bug look at this closely
    int max_dim = 0; 
    int tex_width = 1; 
    int tex_height = 0;
    char *pixels; 
    int pen_y = 0 ;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned int tex; 
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    for(size_t k = 0 ; k < this->name.size(); k++){
        if(FT_Load_Char(face, (unsigned char)this->name[k], FT_LOAD_RENDER)){
            //if(FT_Load_Char(face, (int)this->name[k], FT_LOAD_RENDER)){
            std::cout << "ERROR::FREETYPE: Failed to load Glyph for character " << this->name[k] << std::endl;
            continue; 
        }
        if (k == 0){
            max_dim = (1 + (static_cast<unsigned int>(face->glyph->metrics.width) >> 6)) * this->name.size();  
            tex_height = (1 + (static_cast<unsigned int>(face->glyph->metrics.height) >> 6)); 
            //max_dim = 16 * this->name.size();
            while(tex_width < max_dim) tex_width <<= 1;
            pixels = (char*)calloc(tex_width * tex_height, 1);
            /*    glTexImage2D(
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
                  */
        }
        FT_Bitmap * bmp = &face->glyph->bitmap; 

        for(unsigned int i = 0; i < bmp->rows; i++){
            for(unsigned int j =0; j < bmp->width; j++){
                int y = pen_y + j;  
                int x = i; 
                pixels[i * tex_width + y] = bmp->buffer[i * bmp->pitch + j]; 
            } 
        }
        character = {
            texture, 
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), 
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), 
            static_cast<unsigned int>(face->glyph->advance.x)

        };
        Characters.insert(std::pair<char, Character>(this->name[k],character));
        pen_y += bmp->width + 1;
        }

        glTexImage2D(
                GL_TEXTURE_2D, 
                0,
                GL_RED, 
                tex_width,  
                tex_height,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE, 
                pixels 
                );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // TODO where to put scale ? 
        float scale = 1.0;
        //float xpos = character.Bearing.x * scale; 
        //float ypos = (character.Bearing.y - character.Size.y) * scale; 
        float xpos = 20.0f * character.Bearing.x/character.Size.x; 
        float ypos = 20.0f * (1.0f - character.Bearing.y/character.Size.y) ; 
        float w = character.Size.x * scale; 
        float h = character.Size.y * scale; 

        float maxX = (xpos + w)/100; 
        float maxY = (ypos + h)/100;
        /*float verticesText[] = {
          xpos/maxX, (ypos + h)/maxY , 0.0f, 0.0f, 
          xpos/maxX, ypos/maxY, 0.0f, 1.0f, 
          (xpos + w)/maxX, ypos/maxY, 1.0f, 1.0f, 

          xpos/maxX, (ypos + h)/maxY, 0.0f, 0.0f, 
          (xpos + w)/maxX, ypos/maxY, 1.0f, 1.0f, 
          (xpos + w)/maxX, (ypos + h)/maxY, 1.0f, 0.0f
          };*/
        // TODO scale it down in the model matrix.
        size_t nc = this->name.size(); 
        float verticesText[] = {
            (-10.0f * nc  + xpos)/2, (10.0f + ypos)/2, 0.0f, 0.0f, 
            (-10.0f * nc + xpos)/2, (-10.0f + ypos)/2, 0.0f, 1.0f, 
            (10.0f * nc + xpos)/2, (-10.0f + ypos)/2, 1.0f, 1.0f, 

            (-10.0f * nc + xpos)/2, (10.0f + ypos)/2, 0.0f, 0.0f, 
            (10.0f * nc + xpos)/2, (-10.0f + ypos)/2, 1.0f, 1.0f, 
            (10.0f * nc + xpos)/2, (10.0f + ypos)/2, 1.0f, 0.0f
        };

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        unsigned int VBOT; 
        glGenVertexArrays(1, &this->VAOs[(VAOTYPE)TEXT]); 
        glGenBuffers(1, &VBOT);

        glBindVertexArray(this->VAOs[(VAOTYPE)TEXT]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOT);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesText), verticesText, GL_STATIC_DRAW); 

        glEnableVertexAttribArray(0); 
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); 


        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void Node::drawNode(Shader* shader, int maxLevel, int levelSeparation){
        shader->use();
        glm::mat4 model = glm::mat4(1.0f); 
        //width = 20.0f;
        int xFinal = this->x - this->width / 2;
        int yFinal = this->y + this->height/ 2;
        model = glm::translate(model, glm::vec3(xFinal, yFinal, 0.0f));
        //model = glm::scale(model, glm::vec3(x/20.0f, x/20.0f, 1.0f));
        int modelLoc = glGetUniformLocation(shader->ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glm::mat4 projection = glm::mat4(1.0f);
        float yAxisProjection = (float(maxLevel * levelSeparation * 3)/4); 
        projection = glm::ortho(-400.0f, 400.0f, -yAxisProjection, yAxisProjection , 0.0f, 10.0f); 
        int projectionLoc = glGetUniformLocation(shader->ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(this->VAOs[RECTANGLE]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(this->VAOs[ARROW]); 
        if(this->parent != NULL){

            model = glm::mat4(1.0f); 
            model = glm::translate(model, glm::vec3(this->x - this->width / 2 , this->y + this->height, 0.0f));
            model = glm::rotate(model, -this->arrowAngle, glm::vec3(0.0, 0.0, 1.0));
            model = glm::scale(model, glm::vec3(this->arrowAmp/10.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
            glDrawArrays(GL_LINES, 0, 2);
        }


        std::cout << "RENDERTEXT:1::" << glGetError() << std::endl;
        this->font->shader->use();
        glBindVertexArray(this->VAOs[TEXT]); 
        modelLoc = glGetUniformLocation(this->font->shader->ID, "model");
        model = glm::mat4(1.0f); 
        model = glm::translate(model, glm::vec3(xFinal, yFinal, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        projectionLoc = glGetUniformLocation(this->font->shader->ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        std::cout << "RENDERTEXT:2::" << glGetError() << std::endl;
        std::cout << "RENDERTEXT:3::" << glGetError() << std::endl;
        glBindTexture(GL_TEXTURE_2D, this->texture); 
        std::cout << "RENDERTEXT:4::" << glGetError() << std::endl;
        glDrawArrays(GL_TRIANGLES, 0, 6);

        std::cout << "RENDERTEXT:5::" << glGetError() << std::endl;
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
