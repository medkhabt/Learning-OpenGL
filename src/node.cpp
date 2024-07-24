#include <node.h>
#include <map>
#include <font.h>

void Node::buildNode() {
    // Addind the spacing in the calculation ( half of the node width )
    // ------ BUILDING RECTANGLE --------
    float vertices[] = {
        this->width/2, -this->width/2, 0.0f,
        this->width/2, this->width/2, 0.0f,
        -this->width/2, this->width/2, 0.0f,
        -this->width/2, -this->width/2, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2, 
        0, 2, 3  
    };


    unsigned int VBO, EBO; 
    glGenVertexArrays(1, &this->VAOs[(VAOTYPE)RECTANGLE]); 
    std::cout << glGetError() << std::endl;
    glGenBuffers(1, &VBO);
    std::cout << glGetError() << std::endl;
    glGenBuffers(1, &EBO);
    std::cout << glGetError() << std::endl;

    glBindVertexArray(this->VAOs[(VAOTYPE)RECTANGLE]);
    std::cout << glGetError() << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    std::cout << glGetError() << std::endl;
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    std::cout << glGetError() << std::endl;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
    std::cout << glGetError() << std::endl;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    std::cout << glGetError() << std::endl;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
    std::cout << glGetError() << std::endl;
    glEnableVertexAttribArray(0); 
    std::cout << "glgeterror:" <<glGetError() << std::endl;
    // ------- BUILDING THE ARROW ----------

    float verticesArrow[] = {
        0.0f, 0.0f, 0.0f,
        -10.0f, 0.0f, 0.0f, 

    }; 
    unsigned int VBOA;
    glGenVertexArrays(1, &this->VAOs[ARROW]); 
    glGenBuffers(1, &VBOA);

    glBindVertexArray(this->VAOs[ARROW]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOA);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesArrow), verticesArrow, GL_STATIC_DRAW); 


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(0); 

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    // ------- BUILDING NAME OF THE NODE ----------
    glEnable(GL_TEXTURE_2D);
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
    for(size_t k = 0 ; k < this->name.size(); k++){
        if(FT_Load_Char(face, (unsigned char)this->name[k], FT_LOAD_RENDER)){
            //if(FT_Load_Char(face, (int)this->name[k], FT_LOAD_RENDER)){
            std::cout << "ERROR::FREETYPE: Failed to load Glyph for character " << this->name[k] << std::endl;
            continue; 
        }
        if (k == 0 ){
            max_dim = (1 + (static_cast<unsigned int>(face->glyph->metrics.width) >> 6)) * this->name.size();  
            tex_height = (1 + (static_cast<unsigned int>(face->glyph->metrics.height) >> 6)); 
            //max_dim = 16 * this->name.size();
            while(tex_width < max_dim) tex_width <<= 1;
            pixels = (char*)calloc(tex_width, 1);
        }
        FT_Bitmap * bmp = &face->glyph->bitmap; 

        for(unsigned int i = 0; i < bmp->rows; i++){
            for(unsigned int j =0; j < bmp->width; j++){
                int y = pen_y + j;  
                int x = i; 
                pixels[x * tex_width + y] = bmp->buffer[i * bmp->pitch + j]; 
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

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        unsigned int tex; 
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        this->texture = tex;
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
        float xpos = character.Bearing.x * scale; 
        float ypos = (character.Bearing.y - character.Size.y) * scale; 
        float w = character.Size.x * scale; 
        float h = character.Size.y * scale; 

        float verticesText[] = {
            xpos, ypos + h, 0.0f, 0.0f, 
            xpos, ypos, 0.0f, 1.0f, 
            xpos + w, ypos, 1.0f, 1.0f, 

            xpos, ypos + h, 0.0f, 0.0f, 
            xpos + w, ypos, 1.0f, 1.0f, 
            xpos + w, ypos + h, 1.0f, 0.0f
        };

        unsigned int VBOT; 
        glGenVertexArrays(1, &this->VAOs[(VAOTYPE)TEXT]); 
        glGenBuffers(1, &VBOT);

        glBindVertexArray(this->VAOs[(VAOTYPE)TEXT]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOT);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesText), verticesText, GL_STATIC_DRAW); 

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); 
        glEnableVertexAttribArray(0); 

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Don't unbind the ebo while vao is active !
        glBindVertexArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        //return VAO;
    }
    void Node::drawNode(Shader* shader, int maxLevel, int levelSeparation){
        glm::mat4 model = glm::mat4(1.0f); 
        //width = 20.0f;
        int xFinal = this->x - this->width / 2;
        int yFinal = this->y + this->width / 2;
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
            model = glm::translate(model, glm::vec3(this->x - this->width / 2 , this->y + this->width, 0.0f));
            model = glm::rotate(model, -this->arrowAngle, glm::vec3(0.0, 0.0, 1.0));
            model = glm::scale(model, glm::vec3(this->arrowAmp/10.0f, 1.0f, 1.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
            glDrawArrays(GL_LINES, 0, 2);
        }
    }

