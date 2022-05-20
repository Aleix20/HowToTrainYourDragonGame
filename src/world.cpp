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
    //EntityMesh island;
    

   /* island = EntityMesh();
    island.mesh = Mesh::Get((PATH1 + s.assign("island.ASE")).c_str());
    island.texture = Texture::Get((PATH1+ s.assign("island_color.tga")).c_str());
    this->staticEntities.push_back(&island);*/

   
    //this->staticEntities.push_back(&ground);

    this->sky = new EntityMesh();
    this->sky->mesh = Mesh::Get((PATH + s.assign("cielo/cielo.ASE")).c_str());
    this->sky->texture = Texture::Get((PATH + s.assign("cielo/cielo.tga")).c_str());
    //this->staticEntities.push_back(sky);


    EntityCharacterDragon* firstDragon;
    firstDragon = new EntityCharacterDragon();
    firstDragon->mesh = Mesh::Get((PATH + s.assign("NightFury/Toothless.obj")).c_str());
    firstDragon->texture = Texture::Get((PATH + s.assign("NightFury/Toothless.png")).c_str(), true);
    firstDragon->characterMesh = Mesh::Get((PATH + s.assign("Hiccup/Hiccup.obj")).c_str());
    firstDragon->characterTex = Texture::Get((PATH + s.assign("Hiccup/HiccupTeen.png")).c_str(),true);
    firstDragon->characterModel = Matrix44();
    firstDragon->characterOffset.setTranslation(0.0f, 2.0f, 0.0f);
    

    this->dynamicEntitiesDragons.push_back(firstDragon);

    EntityMesh* defaultDragon = new EntityMesh();
    defaultDragon->mesh = Mesh::Get((PATH + s.assign("NightFury/Toothless.obj")).c_str());
    defaultDragon->texture = Texture::Get((PATH + s.assign("NightFury/Toothless.png")).c_str(), true);
    Matrix44 staticDragonModel = Matrix44();
    staticDragonModel.translate(5,1.5,5);
    defaultDragon->model = staticDragonModel;

    this->staticEntitiesDragons.push_back(defaultDragon);

    this->mainCharacter = new EntityCharacter();
    this->mainCharacter->mesh = Mesh::Get((PATH + s.assign("Hiccup/Hiccup.obj")).c_str());
    this->mainCharacter->texture = Texture::Get((PATH + s.assign("Hiccup/HiccupTeen.png")).c_str(), true);
    
    EntityMesh* table = new EntityMesh();
    table->mesh = Mesh::Get((PATH + s.assign("aldeas/table.obj")).c_str());
    table->texture = Texture::Get((PATH + s.assign("aldeas/table.png")).c_str(),true);
    Matrix44 tableModel = Matrix44();
    tableModel.translate(4,0,3);
    table->model = tableModel;
    this->staticEntities.push_back(table);
    
    EntityMesh* table1 = new EntityMesh();
    table1->mesh = Mesh::Get((PATH + s.assign("aldeas/table.obj")).c_str());
    table1->texture = Texture::Get((PATH + s.assign("aldeas/table.png")).c_str(),true);
    Matrix44 tableModel1 = Matrix44();
    tableModel1.translate(4, 5, 3);
    table1->model = tableModel1;
    this->staticEntities.push_back(table1);
    

    EntityMesh* astrid = new EntityMesh();
    astrid->name = "Mission1";
    astrid->mesh = Mesh::Get((PATH + s.assign("Astrid/Astrid.obj")).c_str());
    astrid->texture = Texture::Get((PATH + s.assign("Astrid/Astrid.png")).c_str(), true);
    Matrix44 astridModel = Matrix44();
    astridModel.translate(15,0,0);
    astrid->model = astridModel;
    this->staticEntitiesCharacter.push_back(astrid);

    this->ground = new EntityMesh();
    this->ground->mesh = new Mesh();
    this->ground->mesh->createPlane(100);
    this->ground->texture = Texture::Get((PATH + s.assign("grass.jpg")).c_str());
    this->ground->tiling = 100.0f;
    
    
    
}
