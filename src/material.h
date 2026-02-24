#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "shader.h"
//Wrapper class for shaders (and textures if we have time...probably not)
class Material
{
public:
    Shader* shader = nullptr;

    void bind()
    {
        if (shader) shader->use();
    }
};

#endif