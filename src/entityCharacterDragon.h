#ifndef entityCharacterDragon_h
#define entityCharacterDragon_h
#include "entity.h"
#include "game.h"
#include "camera.h"
class EntityCharacterDragon:public Entity
{
public:
	Mesh* characterMesh;
	Texture* characterTex;
	float dragonSpeed = 50.0f;
	EntityCharacterDragon();
	virtual void render() override;
	virtual void update(float dt) override;
};
#endif


