#include "tree.h" 
#include "node.h"
#include <iostream>


int main(){

    Node* s = new Node(NULL, "S", 0, 0); 
    Node* a = new Node(s, "A", 0, 0); 
    Node* a1 = new Node(a, "A1", 0, 0); 
    Node* a2 = new Node(a, "A2", 0, 0); 
    Node* b = new Node(s, "B", 0, 0); 
    Node* c = new Node(s, "C", 0, 0); 
    Node* c1 = new Node(c, "C1", 0, 0); 
    Node* c2 = new Node(c, "C2", 0, 0); 
    Node* c3 = new Node(c, "C3", 0, 0); 
    Node* c11 = new Node(c1, "C11", 0, 0); 
    Node* c12 = new Node(c1, "C12", 0, 0); 
    Node* c13 = new Node(c1, "C13", 0, 0); 
    Node* c14 = new Node(c1, "C14", 0, 0); 
    Node* c15 = new Node(c1, "C15", 0, 0); 

    std::cout << "TEST file:: Creating tree" << std::endl;
    Tree* t =  new Tree(s, 1, 2, 10);
    std::cout << "TEST file:: positionTree execution"<< std::endl;
    t->positionTree(s);
    //delete s; 
    delete a;
    delete a1;
    delete a2;
    delete b; 
    delete c; 
    delete c1; 
    delete c2; 
    delete c3; 
    delete c11; 
    delete c12; 
    delete c13; 
    delete c14; 
    delete c15; 
    delete t;
    return 0;
}
