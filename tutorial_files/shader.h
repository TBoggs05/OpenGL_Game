#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/*
A shader class that can:
-read shaders from disk
-compile and link shaders
-check for errors
-easy use!
*/
class Shader
{
public:
    //the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    //use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:

    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif