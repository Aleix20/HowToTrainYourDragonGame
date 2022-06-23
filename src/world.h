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
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "audio.h"
#endif

class EntityMesh;
class EntityCharacterDragon;
class EntityCharacter;
class Entity;


class World
{
public:
	World();
	int selectedEntities = 0;
	int selectedBuild = 0;
	EntityMesh* sky;
	EntityMesh* ocean;

	Entity* selectedEntity = NULL;
	EntityMesh* ground;

	bool audioTimer;
	float timerAudio;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	HCHANNEL channelAudios = 0;
#endif

	bool mission1 = false;
	bool mission1End = true;
	bool mission2 = false;
	bool mission2End = false;
	bool mission3 = false;
	bool mission3End = false;
	bool topOfDragon = false;
	float missionTime=120;
	int currentDragon = 0;
	std::vector<EntityMesh*> staticEntities;
	std::vector<EntityMesh*> staticEntitiesDragons;
	std::vector<EntityMesh*> staticEntitiesCharacter;
	std::vector<EntityMesh*> staticEntitiesPlants;
	std::vector<EntityMesh*> buildWorld;
	std::vector<EntityMesh*> mission1Entities;
	std::vector<EntityMesh*> mission2Entities;

	std::vector<EntityMesh*> mission1EntitiesCopy;
	std::vector<EntityMesh*> mission2EntitiesCopy;
	std::vector<EntityCharacterDragon*> dynamicEntitiesDragons;

	EntityCharacter* mainCharacter;
	bool playStage = false;
	void loadResources();
	void loadObjectFile(const char* path);
	void writeObjectFile(const char* path);
	void writeDynamicDragons(std::ofstream& outdata,  EntityCharacterDragon* entity, std::string type);
	void staticEntitiesWrite(std::ofstream& outdata, EntityMesh* entity, std::string type);
	void readEntitiesCharacterDragonAttributes(std::stringstream& ss, std::string& out, bool& entityB, EntityCharacterDragon*& entityDragon, std::string& PATH2, std::vector<EntityCharacterDragon*>* entitiesDragons);
	void readEntitiesAttributes(std::stringstream& ss, std::string& out, bool& entityB, EntityMesh*& entity, std::string& PATH2, std::vector<EntityMesh*>* entities);
	//void staticCharacterDragonRead(std::string& type, std::stringstream& ss, std::string& out, bool& entityB, EntityMesh*& entity, std::string& PATH2, std::vector<EntityMesh*>* entities);
    void Mision1(std::vector<EntityMesh*>& entities);
	void Mision2(std::vector<EntityMesh*>& entities);
};
#endif
