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

	EntityMesh* sky;
	EntityMesh* ocean;
	Entity* selectedEntity;
	EntityMesh* ground;
	bool mission1 = false;
	bool topOfDragon = false;
	int currentDragon = 0;
	std::vector<EntityMesh*> staticEntities;
	std::vector<EntityMesh*> staticEntitiesDragons;
	std::vector<EntityMesh*> staticEntitiesCharacter;
	std::vector<EntityCharacterDragon*> dynamicEntitiesDragons;
	EntityCharacter* mainCharacter;

	void loadResources();
	void loadObjectFile(const char* path);
	void writeObjectFile(const char* path);
	void writeDynamicDragons(std::ofstream& outdata,  EntityCharacterDragon* entity, std::string type);
	void staticEntitiesWrite(std::ofstream& outdata, EntityMesh* entity, std::string type);
	void readEntitiesCharacterDragonAttributes(std::stringstream& ss, std::string& out, bool& entityB, EntityCharacterDragon*& entityDragon, std::string& PATH2, std::vector<EntityCharacterDragon*>* entitiesDragons);
	void readEntitiesAttributes(std::stringstream& ss, std::string& out, bool& entityB, EntityMesh*& entity, std::string& PATH2, std::vector<EntityMesh*>* entities);
	//void staticCharacterDragonRead(std::string& type, std::stringstream& ss, std::string& out, bool& entityB, EntityMesh*& entity, std::string& PATH2, std::vector<EntityMesh*>* entities);
};
#endif
