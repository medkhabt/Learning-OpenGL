#ifndef NODE_H 
#define NODE_H 
#include <string>
#include <iostream>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <font.h>
enum VAOTYPE{ 
    RECTANGLE = 0, 
    TEXT, 
    ARROW
};
//typedef enum VAOTYPE VAOTYPE; 
class Node {
    public:
        std::string name;
        Node* parent = NULL; 
        Node* firstChild = NULL; 
        Node* leftSibling = NULL; 
        Node* rightSibling = NULL; 
        Node* leftNeighbor = NULL; 
        unsigned int texture;  
        float width; 
        float height;
        float x; 
        float y; 
        float arrowAmp; 
        float arrowAngle;
        float prelimX; 
        float modifier; 

        Node(Node* parent, std::string name, float x, float y, Font* f):  name(name), parent(parent), x(x), y(y), font(f) {
            this->width = 20.0f + 20.0f * name.size() / 5 ; // ((log(name.size()) / log(2))  - 1);
            this->height = 20.0f;
            if(parent != NULL){
                if(parent->firstChild == NULL){
                    parent->firstChild = this; 
                }else {
                    Node* tempNode = parent->firstChild; 
                    // i dont get the error but doing this to avoid it.
                    while(tempNode->rightSibling != nullptr) {
                        tempNode = tempNode->rightSibling; 
                    }
                    tempNode->rightSibling = this; 
                    this->leftSibling = tempNode;
                }
            }
        }
        //TODO specify the width on the constructor.
        void buildNode(); 
        void drawNode(Shader* shader, Node* root, int maxLevel, int levelSeparation);
        // TODO DELETE all the subtree of the node.
        bool isLeaf(){
            return this->firstChild == NULL; 
        }
        bool hasLeftSibling(){
            return this->leftSibling != NULL;
        }
        bool hasRightSibling(){
            return this->rightSibling != NULL;  
        }
        bool hasChild() {
            return this->firstChild != NULL; 
        }
    private : 
        unsigned int VAOs[3];  
        Font* font;  

};
#endif 
