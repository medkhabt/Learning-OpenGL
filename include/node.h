#ifndef NODE_H 
#define NODE_H 
#include <iostream>
#include <string>

class Node {
    public:
        std::string name;
        Node* parent = NULL; 
        Node* firstChild = NULL; 
        Node* leftSibling = NULL; 
        Node* rightSibling = NULL; 
        Node* leftNeighbor = NULL; 
        float width; 
        float x; 
        float y; 
        float arrowAmp; 
        float arrowAngle;
        float prelimX; 
        float modifier; 

        Node(Node* parent, std::string name, float x, float y):  name(name), parent(parent), x(x), y(y) {
            this->width = 20.0f;
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
        // TODO DELETE all the subtree of the node.
        //~Node(){
        //}
        //}
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

};
#endif 
