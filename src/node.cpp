#include <node.h>
#include <map>
#include <font.h>

Node::~Node(){
    Node *next; 
    if(this->hasChild()) {
        next = this->firstChild;  
        while(next->hasRightSibling()) {
            next = next->rightSibling;  
            delete next->leftSibling; 
        }
        delete next; 
    }
}
glm::vec2 Node::bernsteinPoly(float t, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2){
    return (- t * t - t) * p0 + (-2 * t*t + 2*t) * p1 + t * t * p2; 
}
glm::vec2 Node::bernsteinPolyQuadraticCurve(float t, glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3){
    return (-t*t*t+3*t*t-3*t+1) * p0 + (3*t*t*t-6*t*t+3*t) * p1 + (-3*t*t*t+3*t*t) * p2 + t*t*t * p3;
}
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

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBO);

    // ------- BUILDING THE ARROW ----------
    //model = glm::translate(model, glm::vec3(this->x - this->width / 2 , this->y + this->height, 0.0f));
    //model = glm::rotate(model, -this->arrowAngle, glm::vec3(0.0, 0.0, 1.0));
    //model = glm::scale(model, glm::vec3(this->arrowAmp/10.0f, 1.0f, 1.0f));
    // I could just normalize the coordnates.., but i also didn't centralize the variable defining the width and height of the camera/world(for now they are the same). 
    std::cout << "the angle is in the buidling section : " << this->arrowAngle << std::endl;
    glm::vec2 parentCoord(10.0f * cos( this->arrowAngle),  10.0f * sin( this->arrowAngle)); 
    std::cout << "parent coord (" << parentCoord.x << "," << parentCoord.y << ")" << std::endl;
    glm::vec2 inmiddle; 
    float verticesArrow[20*3]; 
    glm::vec2 p1 = glm::vec2(parentCoord.x * 0.8, parentCoord.y * 0.25);
    glm::vec2 p2 = glm::vec2(parentCoord.x * 1.0, - parentCoord.y * 0.25);
    for (size_t t = 0; t < 20; t++){
        //inmiddle = Node::bernsteinPoly(t*0.05, glm::vec2(0.0f, 0.0f), glm::vec2(0.7 * parentCoord.x, parentCoord.y * 0.5f), parentCoord );  
        inmiddle = Node::bernsteinPolyQuadraticCurve(t*0.05, glm::vec2(0.0f, 0.0f), p1, p2, parentCoord);
        verticesArrow[t*3] = inmiddle.x;
        verticesArrow[t*3 + 1] = inmiddle.y;
        verticesArrow[t*3 + 2] = 0.0f;

    }
    //glm::vec2 inmiddle = Node::bernsteinPoly(0.5, parentCoord, glm::vec2(0.0f, parentCoord.y), glm::vec2(0.0f, 0.0f));


    std::cout << "in middle is (" << inmiddle.x << "," << inmiddle.y << ")" << std::endl;
    //float verticesArrow[] = {
    //0.0f, 0.0f, 0.0f,
    //inmiddle.x, inmiddle.y, 0.0f, 
    //parentCoord.x, parentCoord.y, 0.0f, 
    //}; 
    unsigned int VBOA;
    glGenVertexArrays(1, &this->VAOs[(VAOTYPE)ARROW]); 
    glGenBuffers(1, &VBOA);

    glBindVertexArray(this->VAOs[(VAOTYPE)ARROW]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOA);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesArrow), verticesArrow, GL_STATIC_DRAW); 


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(0); 

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBOA);

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
            std::cout << "ERROR::FREETYPE: Failed to load Glyph for character " << this->name[k] << std::endl;
            continue; 
        }
        max_dim +=  static_cast<unsigned int>(face->glyph->metrics.width) >> 6;
        int h = static_cast<unsigned int>(face->glyph->metrics.height) >> 6;
        if(tex_height < h ){
            tex_height = h;  
        }
    }
    while(tex_width < max_dim) tex_width <<= 1;
    pixels = (char*)calloc(tex_width * tex_height, 1);
    for(size_t k = 0 ; k < this->name.size(); k++){
        if(FT_Load_Char(face, (unsigned char)this->name[k], FT_LOAD_RENDER)){
            std::cout << "ERROR::FREETYPE: Failed to load Glyph for character " << this->name[k] << std::endl;
            continue; 
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



    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBOT);
}
void Node::drawNode(Shader* shader, Node* root, float* canvas){
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
    //std::cout << "yAxis : +/-" << yAxisProjection << " , level sep = "<< levelSeparation << ", maxlevel = " << maxLevel << std::endl; 
    projection = glm::ortho(canvas[0], canvas[1], canvas[2], canvas[3], 0.0f, 10.0f); 
    int projectionLoc = glGetUniformLocation(shader->ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(this->VAOs[RECTANGLE]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(this->VAOs[ARROW]); 
    if(this->parent != NULL){

        model = glm::mat4(1.0f); 
        model = glm::translate(model, glm::vec3(this->x - this->width / 2 , this->y + this->height, 0.0f));
        //model = glm::rotate(model, this->arrowAngle, glm::vec3(0.0, 0.0, 1.0));
        model = glm::scale(model, glm::vec3(this->arrowAmp/10.0f, this->arrowAmp/10.0f, 1.0f));
        //model = glm::scale(model, glm::vec3(this->arrowAmp, 1.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glDrawArrays(GL_LINE_STRIP, 0, 20);
    }


    //std::cout << "RENDERTEXT:1::" << glGetError() << std::endl;
    this->font->shader->use();
    glBindVertexArray(this->VAOs[TEXT]); 
    modelLoc = glGetUniformLocation(this->font->shader->ID, "model");
    model = glm::mat4(1.0f); 
    model = glm::translate(model, glm::vec3(xFinal, yFinal, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    projectionLoc = glGetUniformLocation(this->font->shader->ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    //std::cout << "RENDERTEXT:2::" << glGetError() << std::endl;
    //std::cout << "RENDERTEXT:3::" << glGetError() << std::endl;
    glBindTexture(GL_TEXTURE_2D, this->texture); 
    //std::cout << "RENDERTEXT:4::" << glGetError() << std::endl;
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //std::cout << "RENDERTEXT:5::" << glGetError() << std::endl;
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

