#ifndef MESH_H_
#define MESH_H_

#include "config.h"
#include "shader.h"
#include <string>
class Mesh{

public:
    void setup(); // calls the setup functions that act on the vertex attribute + buffer
    ~Mesh(){
        delete vertices;
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    void draw();
    Mesh(std::string preset){
        this->preset = preset;
        setup();
    }
private:
    float* vertices;
    unsigned int numVertices;
    unsigned int VAO; //Vertex attr obj for this gameObject
    unsigned int VBO;
    unsigned long long stride;
    std::string preset; //determines which vertex set to use out of our presets defined in this class
    void generateVBO();
    void generateVAO();
    void bindVAO();
    void bindAndUploadVBO();
    void setVertexAttribPointer();
    void unbind();
};


#endif