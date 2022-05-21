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
	

	this->sky = new EntityMesh();
	this->sky->mesh = Mesh::Get((PATH + s.assign("cielo/cielo.ASE")).c_str());
	this->sky->texture = Texture::Get((PATH + s.assign("cielo/cielo.tga")).c_str());


	EntityCharacterDragon* dragon1;
	dragon1 = new EntityCharacterDragon();
	dragon1->mesh = Mesh::Get((PATH + s.assign("NightFury/Toothless.obj")).c_str());
	dragon1->texture = Texture::Get((PATH + s.assign("NightFury/Toothless.png")).c_str(), true);
	dragon1->model.translate(5, 1.5, 5);
	dragon1->characterMesh = Mesh::Get((PATH + s.assign("Hiccup/Hiccup.obj")).c_str());
	dragon1->characterTex = Texture::Get((PATH + s.assign("Hiccup/HiccupTeen.png")).c_str(), true);
	dragon1->characterModel = Matrix44();
	dragon1->characterOffset.setTranslation(0.0f, 2.0f, 0.0f);

	EntityCharacterDragon* dragon2;
	dragon2 = new EntityCharacterDragon();
	dragon2->mesh = Mesh::Get((PATH + s.assign("Nadder/Nadder.obj")).c_str());
	dragon2->texture = Texture::Get((PATH + s.assign("Nadder/Nadder.png")).c_str(), true);
	dragon2->model.translate(5, 1.5, 5);
	dragon2->characterMesh = Mesh::Get((PATH + s.assign("Hiccup/Hiccup.obj")).c_str());
	dragon2->characterTex = Texture::Get((PATH + s.assign("Hiccup/HiccupTeen.png")).c_str(), true);
	dragon2->characterModel = Matrix44();
	dragon2->characterOffset.setTranslation(0.0f, 2.0f, 0.0f);

	EntityCharacterDragon* dragon3;
	dragon3 = new EntityCharacterDragon();
	dragon3->mesh = Mesh::Get((PATH + s.assign("LightFury/LightFury.obj")).c_str());
	dragon3->texture = Texture::Get((PATH + s.assign("LightFury/LightFury.png")).c_str(), true);
	dragon3->model.translate(5, 1.5, 5);
	dragon3->characterMesh = Mesh::Get((PATH + s.assign("Hiccup/Hiccup.obj")).c_str());
	dragon3->characterTex = Texture::Get((PATH + s.assign("Hiccup/HiccupTeen.png")).c_str(), true);
	dragon3->characterModel = Matrix44();
	dragon3->characterOffset.setTranslation(0.0f, 2.0f, 0.0f);


	this->dynamicEntitiesDragons.push_back(dragon1);
	this->dynamicEntitiesDragons.push_back(dragon2);
	this->dynamicEntitiesDragons.push_back(dragon3);

	EntityMesh* dragon1_static = new EntityMesh();
	dragon1_static->mesh = Mesh::Get((PATH + s.assign("NightFury/Toothless.obj")).c_str());
	dragon1_static->texture = Texture::Get((PATH + s.assign("NightFury/Toothless.png")).c_str(), true);
	Matrix44 dragon1_staticModel = Matrix44();
	dragon1_staticModel.translate(5, 1.5, 5);
	dragon1_static->model = dragon1_staticModel;

	EntityMesh* dragon2_static = new EntityMesh();
	dragon2_static->mesh = Mesh::Get((PATH + s.assign("Nadder/Nadder.obj")).c_str());
	dragon2_static->texture = Texture::Get((PATH + s.assign("Nadder/Nadder.png")).c_str(), true);
	Matrix44 dragon2_staticModel = Matrix44();
	dragon2_staticModel.translate(5, 1.5, 5);
	dragon2_static->model = dragon2_staticModel;

	EntityMesh* dragon3_static = new EntityMesh();
	dragon3_static->mesh = Mesh::Get((PATH + s.assign("LightFury/LightFury.obj")).c_str());
	dragon3_static->texture = Texture::Get((PATH + s.assign("LightFury/LightFury.png")).c_str(), true);
	Matrix44 dragon3_staticModel = Matrix44();
	dragon3_staticModel.translate(5, 1.5, 5);
	dragon3_static->model = dragon3_staticModel;

	this->staticEntitiesDragons.push_back(dragon1_static);
	this->staticEntitiesDragons.push_back(dragon2_static);
	this->staticEntitiesDragons.push_back(dragon3_static);

	this->mainCharacter = new EntityCharacter();
	this->mainCharacter->mesh = Mesh::Get((PATH + s.assign("Hiccup/Hiccup.obj")).c_str());
	this->mainCharacter->texture = Texture::Get((PATH + s.assign("Hiccup/HiccupTeen.png")).c_str(), true);

	EntityMesh* table = new EntityMesh();
	table->mesh = Mesh::Get((PATH + s.assign("aldeas/table.obj")).c_str());
	table->texture = Texture::Get((PATH + s.assign("aldeas/table.png")).c_str(), true);
	Matrix44 tableModel = Matrix44();
	tableModel.translate(4, 0, 3);
	table->model = tableModel;
	this->staticEntities.push_back(table);

	EntityMesh* stoick = new EntityMesh();
	stoick->name = "ChangeDragon";
	stoick->mesh = Mesh::Get((PATH + s.assign("Stoick/Stoick.obj")).c_str());
	stoick->texture = Texture::Get((PATH + s.assign("Stoick/Stoick.png")).c_str(), true);
	Matrix44 stoickModel = Matrix44();
	stoickModel.translate(20, 0, 0);
	stoick->model = stoickModel;
	this->staticEntitiesCharacter.push_back(stoick);


	EntityMesh* astrid = new EntityMesh();
	astrid->name = "Mission1";
	astrid->mesh = Mesh::Get((PATH + s.assign("Astrid/Astrid.obj")).c_str());
	astrid->texture = Texture::Get((PATH + s.assign("Astrid/Astrid.png")).c_str(), true);
	Matrix44 astridModel = Matrix44();
	astridModel.translate(15, 0, 0);
	astrid->model = astridModel;
	this->staticEntitiesCharacter.push_back(astrid);

	this->ground = new EntityMesh();
	this->ground->mesh = new Mesh();
	this->ground->mesh->createPlane(100);
	this->ground->texture = Texture::Get((PATH + s.assign("grass.jpg")).c_str());
	this->ground->tiling = 100.0f;



}
