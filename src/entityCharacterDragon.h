#ifndef entityCharacterDragon_h
#define entityCharacterDragon_h
#include "entity.h"
#include "game.h"
#include "camera.h"
#include "framework.h"
class EntityCharacterDragon:public Entity
{
public:
	Mesh* characterMesh;
	Texture* characterTex;
    Matrix44 characterModel;
    Matrix44 characterOffset;
	float dragonSpeed = 50.0f;
	EntityCharacterDragon();
	virtual void render() override;
	virtual void update(float dt) override;
	float tiling = 1.0f;
	float angle = 0.0f;
	float angle2 = 0.0f;
	float angle3 = 0.0f;
};
#endif


