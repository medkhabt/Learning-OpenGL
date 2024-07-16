#ifndef TREE_H 
#define TREE_H

#include <vector>
#include <queue>
#include <iostream>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "node.h"
#include "shader.h"
class Tree {
    public : 
        Shader* shader; 
        unsigned int vaoId;   
        Node* root; 
        unsigned int siblingSeparation; 
        unsigned int subtreeSeparation; 
        float xTopAdjustment; 
        float yTopAdjustment; 
        unsigned int levelSeparation;
        std::vector<Node*> prevNode; 



        std::vector<int> levels; 

        Tree(Shader* shader, Node* root, unsigned int siblingSep, unsigned int subtreeSep): shader(shader), root(root), siblingSeparation(siblingSep), subtreeSeparation(subtreeSep) {
            this->prevNode.push_back(root);
            this->vaoId = buildRectangle();  
        }
        Tree(Node* root, unsigned int siblingSep, unsigned int subtreeSep, unsigned levelSeparation):  root(root), siblingSeparation(siblingSep), subtreeSeparation(subtreeSep), levelSeparation(levelSeparation){
            this->prevNode.push_back(root);
        }
        // TODO implememnt this
        unsigned int getMaxVertexesInLevel(){
            return 5;  
        }
        ~Tree() {
            delete this->root;  
        }
        // *************** Walker algorithm for general tree visuzaliztion ************ 
        void initPrevNodeList() {
            for(size_t i = 0 ; i < this->prevNode.size() && this->prevNode[i] != NULL ; i++){
                this->prevNode[i] = NULL;
            }
        }

        float meanNodeSize(Node* leftNode, Node* rightNode) {
            float nodeSize = 0.0f; 
            if (leftNode != NULL) {
                // does this uniary op work for floats properly? 
                nodeSize += leftNode->width/ 2;   
            }
            if (rightNode != NULL) {
                nodeSize += rightNode->width/2;    
            } 
            return nodeSize;
        }
        Node* getLeftMost(Node* node, unsigned int level, unsigned int depth){
            if (level >= depth ) {
                return node; 
            } else if (node->isLeaf()) {
                return NULL;  
            } else {
                Node* rightMost = node->firstChild; 
                Node* leftMost = getLeftMost(rightMost, level + 1, depth) ;
                while(leftMost != NULL && rightMost->hasRightSibling()){
                    rightMost = rightMost->rightSibling; 
                    leftMost = getLeftMost(rightMost, level + 1, depth) ;
                }
                return leftMost; 
            }
        }
        void apportion(Node* node, unsigned int level){
            Node* leftMost = node->firstChild; 
            Node* neighbor = leftMost->leftNeighbor;  
            unsigned int compareDepth = 1; 
            // int depthToStop = this->MaxDepth - level; 
            Node* ancestorLeftMost, *ancestorNeighbor;
            int leftModSum;  
            int rightModSum;
            while(leftMost != NULL && neighbor != NULL){
                leftModSum = 0;
                rightModSum = 0; 
                ancestorLeftMost = leftMost; 
                ancestorNeighbor = neighbor; 
                for (size_t i = 0; i < compareDepth; i++){
                    ancestorLeftMost = ancestorLeftMost->parent;  
                    ancestorNeighbor = ancestorNeighbor->parent;
                    rightModSum += ancestorLeftMost->modifier; 
                    leftModSum += ancestorNeighbor->modifier; 
                }
                float moveDistance = (neighbor->prelimX + 
                        leftModSum +
                        this->subtreeSeparation + 
                        meanNodeSize(leftMost, neighbor)) - 
                    (leftMost->prelimX + rightModSum);
                if (moveDistance > 0) {
                    Node* tempPtr = node; 
                    int leftSiblings = 0;
                    while(tempPtr != NULL && tempPtr != ancestorNeighbor) {
                        leftSiblings++;  
                        tempPtr = tempPtr->leftSibling; 
                    }
                    if(tempPtr != NULL){
                        float portion = moveDistance / leftSiblings;  
                        tempPtr = node; 
                        while(tempPtr == ancestorNeighbor) {
                            tempPtr->prelimX += moveDistance; 
                            tempPtr->modifier += moveDistance; 
                            moveDistance -= portion; 
                            tempPtr = tempPtr->leftSibling;  
                        }
                    } else {
                        return;  
                    }
                }

                compareDepth++; 
                if (leftMost->isLeaf()) {
                    leftMost = getLeftMost(node, 0, compareDepth) ; 
                } else {
                    leftMost = leftMost->firstChild;
                }
            }
        }
        void firstWalk(Node* node, unsigned int level){
            node->leftNeighbor = this->prevNode[level]; 
            // This node becomes the prevNode in its level.
            this->prevNode[level] = node;  
            node->modifier = 0; 
            // in case i want to introduce maxdepth
            //if(node.isLeaf() || level == MaxDepth){
            if(node->isLeaf()){
                if(node->hasLeftSibling()) {
                    node->prelimX = node->leftSibling->prelimX + this->siblingSeparation + meanNodeSize(node->leftSibling, node);
                } else {
                    node->prelimX = 0;  
                }
            } else {
                Node* leftMost = node->firstChild;
                Node* rightMost = leftMost;   
                firstWalk(leftMost, level + 1);
                while(rightMost->hasRightSibling()){
                    rightMost = rightMost->rightSibling; 
                    firstWalk(rightMost, level + 1);
                }
                // parent node position calculation ? 
                float midPoint = ( leftMost->prelimX + rightMost->prelimX ) / 2;  
                if(node->hasLeftSibling()) {
                    node->prelimX = node->leftSibling->prelimX  + 
                        this->siblingSeparation + 
                        meanNodeSize(node->leftSibling, node);
                    // TODO i want to understand this part. 
                    node->modifier = node->prelimX - midPoint; 
                    apportion(node, level);
                } else {
                    node->prelimX = midPoint; 
                }
            }

        }
        // I don't think i need this function for now. as the clipping 
        // matrix does that. I think.
        bool checkextentsrange(float x, float y){
            return true;
        }
        bool secondWalk(Node* node, unsigned int level, int modSum){
            // max depth can be impl here. 
            float xTemp = this->xTopAdjustment + node->prelimX + modSum; 
            float yTemp = this->yTopAdjustment + ( level * this->levelSeparation ) ;
            bool result = true;
            if (checkextentsrange(xTemp, yTemp)){
                node->x = xTemp;  
                node->y = yTemp; 
                std::cout << "Node " << node->name << " :(" << node->x << "," << node->y << ")"<< std::endl;
                if (node->hasChild()){
                    result = secondWalk(node->firstChild, level + 1, modSum + node->modifier); 
                }
                if(result && node->hasRightSibling()){
                    result = secondWalk(node->rightSibling, level, modSum); 
                }
            } else {
                return false; 
            }
            return result ;
        }
        bool positionTree(Node* node){
            if (node != NULL) {
                // INITPREVNODELIST    
                std::cout << "TREE.H:: execute initPrevNodeList" << std::endl;
                initPrevNodeList();

                std::cout << "TREE.H:: execute first firstWalk" << std::endl;
                firstWalk(node, 0);
                std::cout << "TREE.H:: end the recursion of firstWalk " <<std::endl;
                this->xTopAdjustment = node->x - node->prelimX; 
                this->yTopAdjustment = node->y;

                std::cout << "TREE.H:: execute first secondWalk" << std::endl;
                return secondWalk(node, 0, 0);

            } else {
                return true; 
            }
        } 

        // ******************* END WALKER ALGO ***********************************
        void drawTree() {
            // drawNode(), getChildren 
            std::queue<Node*> nodesQueue;
            Node* currNode; 
            nodesQueue.push(root); 
            this->shader->use();
            int i = 0; 
            while(!nodesQueue.empty()){
                currNode = nodesQueue.front();
                //std::cout << "front is : node with (pos =  " << currNode->position << ", level= " << currNode->level << ")" << std::endl; 
                i++;
                nodesQueue.pop();

                drawNode(currNode->position, currNode->level);

                if(this->levels.size() <= currNode->level){
                    this->levels.push_back(1);
                } else {
                    this->levels[currNode->level]++; 
                }

                for(auto &c : currNode->children){
                    nodesQueue.push(c);
                }

            }

            //int maxVertexesPerLevel = 10;
            //unsigned int treeLayout[3] = {1, 2, maxVertexesPerLevel};  

            //for(int i = 0 ; i < sizeof(treeLayout)/sizeof(treeLayout[0]); i++){
            //for ( int j = 0; j < treeLayout[i]; j++) {
            //drawNode(j, i); 
            //}
            //}
        }
        void drawNode(int position, int level) {
            unsigned int maxVertexesPerLevel = this->getMaxVertexesInLevel();
            float x = (200.0f * 2.0f) / float(3 * maxVertexesPerLevel - 2);  
            float gx = ( maxVertexesPerLevel + 2 ) * 200.0f / ((maxVertexesPerLevel - 1) * (3 * maxVertexesPerLevel + 2)) ;
            float gy = ( 3 + 2 ) * 200.0f / ((3 - 1) * (3 * 3 + 2)) ;
            glm::mat4 model = glm::mat4(1.0f); 
            // TODO make the translation dynamic on the y axis
            model = glm::translate(model, glm::vec3(-100.0f + x/2 + position * (x + gx), 100.0f - x/2 - level * (x + gx/3) , 0.0f));
            model = glm::scale(model, glm::vec3(x/20.0f, x/20.0f, 1.0f));
            int modelLoc = glGetUniformLocation(shader->ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.0f, 10.0f); 
            int projectionLoc = glGetUniformLocation(shader->ID, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBindVertexArray(this->vaoId);
            std::cout << "drawing pos: " << position << ", level: " << level << std::endl;
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
        unsigned int buildRectangle() {
            // Addind the spacing in the calculation ( half of the node width )
            float vertices[] = {
                10.0f, -10.0f, 0.0f, 
                10.0f, 10.0f, 0.0f, 
                -10.0f, 10.0f, 0.0f, 
                -10.0f, -10.0f, 0.0f, 
            };

            unsigned int indices[] = {
                0, 1, 2, 
                0, 2, 3  
            };

            unsigned int VBO, VAO, EBO; 
            glGenVertexArrays(1, &VAO); 
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
            glEnableVertexAttribArray(0); 

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Don't unbind the ebo while vao is active !
            glBindVertexArray(0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            return VAO;
        }
        void deleteAll() {
            glDeleteVertexArrays(1, &vaoId); 
            glDeleteProgram(shader->ID); 
        }
        };
#endif 
