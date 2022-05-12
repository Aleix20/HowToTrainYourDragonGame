//
//  entityMesh.h
//  TJE_XCODE
//
//  Created by Alèxia  Lozano Pedrajas on 12/5/22.
//  Copyright © 2022 Alèxia  Lozano Pedrajas. All rights reserved.
//

#ifndef entityMesh_h
#define entityMesh_h

#include "entity.h"
#include "game.h"
#include "camera.h"

class EntityMesh:public Entity
{
public:
    
    EntityMesh();
    EntityMesh(Mesh*mesh, Texture* texture,  Matrix44 model);
    virtual void render() override;
    virtual void update(float dt) override;

};

#endif /* entityMesh_h */
