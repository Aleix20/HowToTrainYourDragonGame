#ifndef Entity_h
#define Entity_h
#include <vector>
#include "framework.h"
#include <string>
#include "texture.h"
#include "mesh.h"
#include "shader.h"
#include "animation.h"


class Entity
{
public:
    Entity(){}; //constructor
    virtual ~Entity() {}; //destructor

    //some attributes 
    std::string name;
    Matrix44 model;
    Mesh* mesh;
    Texture* texture;
    std::vector<Animation*> animations;
    float tiling = 1.0f;

    //methods overwritten by derived classes 
    virtual void render(){};
    virtual void update(float elapsed_time) {};

    //some useful methods...
    Vector3 getPosition() {return model.getTranslation(); };

};
#endif

