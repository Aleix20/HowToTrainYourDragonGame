//
//  entityCharacter.h
//  TJE_XCODE
//
//  Created by Alèxia  Lozano Pedrajas on 12/5/22.
//  Copyright © 2022 Alèxia  Lozano Pedrajas. All rights reserved.
//

#ifndef entityCharacter_h
#define entityCharacter_h

#include "entity.h"
#include "shader.h"
#include "game.h"
#include "camera.h"
class EntityCharacter:public Entity
{
public:
    
    EntityCharacter();
    EntityCharacter(Mesh*mesh, Texture* texture, Matrix44 model);
    virtual void render() override;
    virtual void update(float dt) override;
    float tiling = 1.0f;
    float angle = 0.0f;
};

#endif /* entityCharacter_h */
