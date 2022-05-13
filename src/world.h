#ifndef WORLD_H
#define WORLD_H
#include "entityMesh.h"
#include "entityCharacter.h"
#include "entityCharacterDragon.h"



class World
{
public:
	World();
	EntityMesh ground;
	EntityMesh sky;
	int currentDragon = 0;
	std::vector<EntityMesh*> staticEntities;
	std::vector<EntityMesh*> staticEntitiesDragons;
	std::vector<EntityMesh*> staticEntitiesCharacter;
	std::vector<EntityCharacterDragon*> dynamicEntitiesDragons;
	EntityCharacter mainCharacter;

	void loadResources();

};
#endif
