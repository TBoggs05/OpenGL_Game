/*
This class will include the vertices and attributes for a plane the player and necessary objects may sit on.
*/
#ifndef GROUND_H_
#define GROUND_H_

#include "config.h"
#include "shader.h"
class ground{

public:




private:

    Shader groundShader("shaders/groundShader.vs", "shaders/groundShader.fs");
    float vertices[64];
    unsigned int indices[64];
    





};

#endif