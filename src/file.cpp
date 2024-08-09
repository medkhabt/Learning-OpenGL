#include <fstream>
#include <iostream>
#include <string>
#include <exception>
#include <regex>

#include <file.h>
#include <node.h>


Node* FileManager::generateTree(std::string fileName, Font* font){
    Node * root = new Node(NULL, "S", 0, 200, font); 
    Node* currNode = root;  
    std::ifstream fileToRead(fileName); 
    std::string line; 
    bool firstLine = true;
    if(fileToRead.is_open()){
        while(getline(fileToRead, line)) {
            std::istringstream iss (line); 
            std::string result; 
            if( std::getline(iss, result, '-') ){
                std::cout << "left hand is : " << result << std::endl;
                result = std::regex_replace(result, std::regex("^ +| +$|( ) +"), "$1");
                if(firstLine){
                    currNode->name = result; 
                    firstLine = false; 
                }
                while(currNode->name != result && currNode->hasRightSibling()){
                    currNode = currNode->rightSibling;
                }
                while(currNode->name != result){
                    if(currNode->parent == NULL) {
                        return root;  
                    }
                    currNode = currNode->parent; 
                    while(currNode->name != result && currNode->hasRightSibling()) {
                        currNode = currNode->rightSibling; 
                    }
                }
                std::string token; 
                while( std::getline(iss, token, ' ')) {
                    if(token != ""){
                        Node* child = new Node(currNode, token, 0, 0, font); 
                        std::cout << "ele right hand:" << token << "." << std::endl; 
                    }
                }
                if(currNode->hasChild()) { 
                    currNode = currNode->firstChild; 
                } else {
                    std::cout << "it should have children !" <<std::endl; 
                }
                std::cout << std::endl;
            }
        }
        return root; 
    }
    throw std::runtime_error("shouldn't get to this point when reading the file");
}
