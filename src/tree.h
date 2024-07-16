#ifndef TREE_H 
#define TREE_H

#include <vector>
#include <queue>
#include "node.h"
class Tree {
    public : 
      Shader* shader; 
      unsigned int vaoId;   
      Node* root; 
      std::vector<int> levels; 
      Tree(Shader* shader, Node* root): shader(shader), root(root){
        this->vaoId = buildRectangle();  
      }
      // TODO implememnt this
      unsigned int getMaxVertexesInLevel(){
        return 5;  
      }
      ~Tree() {
        delete this->root;  
      }
      
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
