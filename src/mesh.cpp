#include "mesh.h"
//vertices
     float ground_vertices[] = {
    // positions        // tex coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f
};

void Mesh::setup(){
    generateVBO();
     generateVAO();
      bindVAO();
       bindAndUploadVBO();
        setVertexAttribPointer();
         unbind();
}
void Mesh::generateVBO(){
    glGenBuffers(1, &VBO);
}
void Mesh::generateVAO(){
    glGenVertexArrays(1, &VAO);
}   
void Mesh::bindVAO(){
    glBindVertexArray(VAO);
}
void Mesh::bindAndUploadVBO(){
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //bind the VBO pointer to the target GL_ARRAY_BUFFER
    if(preset == "ground"){
        glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertices), ground_vertices, GL_STATIC_DRAW); //fill vertex buffer memory and send it to GPU
    }
    
}
void Mesh::setVertexAttribPointer(){
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
}
void Mesh::unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void Mesh::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    std::cout << "drawing mesh" << std::endl;
}