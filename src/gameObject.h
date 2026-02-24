#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "config.h"
#include "mesh.h"
#include "shader.h"
#include "material.h"

//helpful struct for abstracting model matrix
struct Transform
{
    glm::vec3 position {0.0f};
    glm::vec3 rotation {0.0f};
    glm::vec3 scale {1.0f};

    glm::mat4 getMatrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, position);

        model = glm::rotate(model, rotation.x, {0,0,0});
        model = glm::rotate(model, rotation.y, {0,0,0});
        model = glm::rotate(model, rotation.z, {0,0,0});

        model = glm::scale(model, scale);

        return model;
    }
};

//Abstract class template for all game object to be drawn
class GameObject{
public:
    GameObject(){
        material = nullptr;
        mesh = nullptr;
    }
    GameObject(Material* mat, Mesh* mes){
        material = mat;
        mesh = mes;
    }
    ~GameObject(){
        
    }
    void draw(){
    material->bind();

    material->shader->setMat4("model", transform.getMatrix());

    mesh->draw();
    }
    Material* material;
    Mesh* mesh;
    Transform transform;
};

#endif