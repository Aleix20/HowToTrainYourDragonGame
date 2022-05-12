#ifndef ENTITYDRAGON_H
#define ENTITYDRAGON_H
#include "entity.h"
#include "game.h"
#include "camera.h"

class EntityDragon:public Entity
{
public:
	
	EntityDragon();
	EntityDragon(Mesh*mesh, Texture* texture, Matrix44 model);
	virtual void render() override;
	virtual void update(float dt) override;

};
#endif
