#ifndef WORLD_H
#define WORLD_H
#include "entityMesh.h"
#include "entityCharacter.h"
#include "entityCharacterDragon.h"
#include "entity.h"
#include "includes.h"
#include "camera.h"
#include "utils.h"
#include "shader.h"

class EntityMesh;
class EntityCharacterDragon;
class EntityCharacter;
class Entity;


class World
{
public:
	World();
	EntityMesh* ground;
	EntityMesh* sky;
	Entity* selectedEntity;
	bool mission1 = false;
	bool topOfDragon = false;
	int currentDragon = 0;
	std::vector<EntityMesh*> staticEntities;
	std::vector<EntityMesh*> staticEntitiesDragons;
	std::vector<EntityMesh*> staticEntitiesCharacter;
	std::vector<EntityCharacterDragon*> dynamicEntitiesDragons;
	EntityCharacter* mainCharacter;

	void loadResources();

};
#endif
