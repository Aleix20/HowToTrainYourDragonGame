#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include "framework.h"
#include <string>
#include "texture.h"
#include "mesh.h"
class Entity
{
public:
    Entity(); //constructor
    virtual ~Entity() {}; //destructor

    //some attributes 
    std::string name;
    Matrix44 model;


    //methods overwritten by derived classes 
    virtual void render(){};
    virtual void update(float elapsed_time) {};

    //some useful methods...
    Vector3 getPosition() {return model.getTranslation(); };

};
#endif

