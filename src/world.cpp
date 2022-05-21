#include "world.h"



std::string s;
World::World()
{

	loadResources();
}

void World::loadResources()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	std::string PATH = "data/";
#else
	std::string PATH = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";
#endif

	loadObjectFile((PATH + s.assign("objects.txt")).c_str());
	this->sky = new EntityMesh();
	this->sky->mesh = Mesh::Get((PATH + s.assign("cielo/cielo.ASE")).c_str());
	this->sky->texture = Texture::Get((PATH + s.assign("cielo/cielo.tga")).c_str());

	ocean = new EntityMesh();
	ocean->mesh = Mesh::Get((PATH + s.assign("water_deep.ASE")).c_str());
	ocean->texture = Texture::Get((PATH + s.assign("water_deep.tga")).c_str());

	mainCharacter = new EntityCharacter();
	mainCharacter->mesh = Mesh::Get((PATH + s.assign("Hiccup/Hiccup.obj")).c_str());
	mainCharacter->texture = Texture::Get((PATH + s.assign("Hiccup/HiccupTeen.png")).c_str(), true);
	mainCharacter->model.setTranslation(5, 0, 15);


	this->ground = new EntityMesh();
	this->ground->mesh = new Mesh();
	this->ground->mesh->createPlane(100);
	this->ground->texture = Texture::Get((PATH + s.assign("grass.jpg")).c_str());
	this->ground->tiling = 100.0f;


}
void World::loadObjectFile(const char* path)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	std::string PATH2 = "data/";
#else
	std::string PATH2 = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";
#endif
	std::string content = "";
	readFile(path, content);
	std::stringstream ss(content);
	std::vector<EntityMesh*>* entities;
	std::vector<EntityCharacterDragon*>* entitiesDragons;
	std::string type = "";
	bool entityB = false;
	EntityMesh* entity;
	EntityCharacterDragon* entityDragon;

	while (!ss.eof()) {
		std::string out;
		ss >> out;
		if (strcmp(out.c_str(), "TYPE") == 0) {
			ss >> out;
			type.assign(out);
			if (strcmp(type.c_str(), "STATICENTITIES") == 0) {
				entities = &this->staticEntities;
			}
			else if (strcmp(type.c_str(), "STATICCHARACTER") == 0) {
				entities = &this->staticEntitiesCharacter;
			}
			else if (strcmp(type.c_str(), "STATICDRAGONS") == 0) {
				entities = &this->staticEntitiesDragons;
			}
			else if (strcmp(type.c_str(), "DYNAMICDRAGONS") == 0) {
				entitiesDragons = &this->dynamicEntitiesDragons;
			}
		}

		if (strcmp(type.c_str(), "DYNAMICDRAGONS") != 0) {

			readEntitiesAttributes(ss, out, entityB, entity, PATH2, entities);
		}
		else {
			readEntitiesCharacterDragonAttributes(ss, out, entityB, entityDragon, PATH2, entitiesDragons);
		}

		//staticCharacterDragonRead(type, ss, out, entityB, entity, PATH2, entities);
	}

}

void World::readEntitiesCharacterDragonAttributes(std::stringstream& ss, std::string& out, bool& entityB, EntityCharacterDragon*& entityDragon, std::string& PATH2, std::vector<EntityCharacterDragon*>* entitiesDragons)
{
	ss >> out;
	if (!entityB) {


		entityDragon = new EntityCharacterDragon();
		entityB = true;
	}
	if (strcmp(out.c_str(), "MESH") == 0) {
		ss >> out;
		entityDragon->mesh = Mesh::Get((PATH2 + out).c_str());
		ss >> out;
	}
	if (strcmp(out.c_str(), "TEX") == 0) {
		ss >> out;
		entityDragon->texture = Texture::Get((PATH2 + out).c_str(), true);
		ss >> out;
	}
	if (strcmp(out.c_str(), "MESH2") == 0) {
		ss >> out;
		entityDragon->characterMesh = Mesh::Get((PATH2 + out).c_str());
		ss >> out;
	}
	if (strcmp(out.c_str(), "TEX2") == 0) {
		ss >> out;
		entityDragon->characterTex = Texture::Get((PATH2 + out).c_str(), true);
		ss >> out;
	}
	if (strcmp(out.c_str(), "POS") == 0) {
		float x, y, z;
		ss >> x >> y >> z;
		entityDragon->model.translate(x, y, z);
		ss >> out;

	}
	if (strcmp(out.c_str(), "OFFSET") == 0) {
		float x, y, z;
		ss >> x >> y >> z;
		entityDragon->characterOffset.translate(x, y, z);
		ss >> out;

	}
	if (strcmp(out.c_str(), "ROT") == 0) {
		float x, y, z;
		ss >> x >> y >> z;
		entityDragon->model.rotate(x * DEG2RAD, Vector3(1, 0, 0));
		entityDragon->model.rotate(y * DEG2RAD, Vector3(0, 1, 0));
		entityDragon->model.rotate(z * DEG2RAD, Vector3(0, 0, -1));
		ss >> out;
	}
	if (strcmp(out.c_str(), "NAME") == 0) {
		ss >> out;
		entityDragon->name = out;
		ss >> out;
	}
	if (strcmp(out.c_str(), "END") == 0) {
		entityB = false;
		entitiesDragons->push_back(entityDragon);

	}
}

void World::readEntitiesAttributes(std::stringstream& ss, std::string& out, bool& entityB, EntityMesh*& entity, std::string& PATH2, std::vector<EntityMesh*>* entities)
{
	ss >> out;
	if (!entityB) {


		entity = new EntityMesh();
		entityB = true;
	}
	if (strcmp(out.c_str(), "MESH") == 0) {
		ss >> out;
		entity->mesh = Mesh::Get((PATH2 + out).c_str());
		ss >> out;
	}
	if (strcmp(out.c_str(), "TEX") == 0) {
		ss >> out;
		entity->texture = Texture::Get((PATH2 + out).c_str(), true);
		ss >> out;
	}
	if (strcmp(out.c_str(), "POS") == 0) {
		float x, y, z;
		ss >> x >> y >> z;
		entity->model.translate(x, y, z);
		ss >> out;

	}
	if (strcmp(out.c_str(), "ROT") == 0) {
		float x, y, z;
		ss >> x >> y >> z;
		entity->model.rotate(x * DEG2RAD, Vector3(1, 0, 0));
		entity->model.rotate(y * DEG2RAD, Vector3(0, 1, 0));
		entity->model.rotate(z * DEG2RAD, Vector3(0, 0, -1));
		ss >> out;
	}
	if (strcmp(out.c_str(), "NAME") == 0) {
		ss >> out;
		entity->name = out;
		ss >> out;
	}
	if (strcmp(out.c_str(), "END") == 0) {
		entityB = false;
		entities->push_back(entity);

	}
}

//void World::staticCharacterDragonRead(std::string& type, std::stringstream& ss, std::string& out, bool& entityB, EntityMesh*& entity, std::string& PATH2, std::vector<EntityMesh*>* entities)
//{
//	if (strcmp(type.c_str(), "STATICCHARACTER") == 0 || strcmp(type.c_str(), "STATICDRAGONS") == 0) {
//		ss >> out;
//		if (!entityB) {
//
//
//			entity = new EntityMesh();
//			entityB = true;
//		}
//		if (strcmp(out.c_str(), "MESH") == 0) {
//			ss >> out;
//			entity->mesh = Mesh::Get((PATH2 + out).c_str());
//			ss >> out;
//		}
//		if (strcmp(out.c_str(), "TEX") == 0) {
//			ss >> out;
//			entity->texture = Texture::Get((PATH2 + out).c_str(), true);
//			ss >> out;
//		}
//		if (strcmp(out.c_str(), "POS") == 0) {
//			float x, y, z;
//			ss >> x >> y >> z;
//			entity->model.translate(x, y, z);
//			ss >> out;
//
//		}
//		if (strcmp(out.c_str(), "ROT") == 0) {
//			float x, y, z;
//			ss >> x >> y >> z;
//			entity->model.rotate(x * DEG2RAD, Vector3(1, 0, 0));
//			entity->model.rotate(y * DEG2RAD, Vector3(0, 1, 0));
//			entity->model.rotate(z * DEG2RAD, Vector3(0, 0, -1));
//			ss >> out;
//		}
//		if (strcmp(out.c_str(), "NAME") == 0) {
//			ss >> out;
//			entity->name = out;
//			ss >> out;
//		}
//		if (strcmp(out.c_str(), "END") == 0) {
//			entityB = false;
//			entities->push_back(entity);
//		}
//	}
//}


