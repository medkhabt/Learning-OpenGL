#ifndef NODE_H 
#define NODE_H 
#include <iostream>
#include <string>

class Node {
    public:
        std::string name;
        Node* parent; 
        Node* firstChild; 
        Node* leftSibling; 
        Node* rightSibling; 
        float width; 
        float x; 
        float  y; 
        float prelimX; 
        float modifier; 
        Node* leftNeighbor; 


        unsigned int level; 
        unsigned int position; 
        std::vector<Node*> children; 


        Node(Node* parent, std::string name, float x, float y ):  name(name), parent(parent), x(x), y(y) {
            if(parent != NULL){
                if(parent->firstChild == NULL){
                    parent->firstChild = this; 
                }else {
                    Node* tempNode = parent->firstChild; 
                    while(tempNode != NULL && tempNode->rightSibling != NULL) {
                        tempNode = tempNode->rightSibling; 
                    }
                    tempNode->rightSibling = this; 
                    this->leftSibling = tempNode;
                }
                this->width = 20.0f;
                this->level = parent->level + 1;  
                position = parent->children.size();
                parent->children.push_back(this);

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
