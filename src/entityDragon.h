#ifndef ENTITYDRAGON_H
#define ENTITYDRAGON_H
#include "entity.h"
#include "shader.h"
#include "game.h"
#include "camera.h"
class EntityDragon:public Entity
{
public:
	Mesh* mesh;
	Shader* shader;
	Texture* texture;
	EntityDragon();
	EntityDragon(Mesh* mesh, Texture* texture, Shader* shader, Matrix44 model);
	virtual void render() override;
	virtual void update(float dt) override;

};
#endif
