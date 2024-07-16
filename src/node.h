#ifndef NODE_H 
#define NODE_H 
#include <iostream>

class Node {
    public:
        unsigned int level; 
        unsigned int position; 
        Node* parent; 
        std::vector<Node*> children; 
        Node(): level(0), position(0){} 
        Node(Node* parent): parent(parent) {
            this->level = parent->level + 1;  
            position = parent->children.size();
            parent->children.push_back(this);
        }
};
#endif 
