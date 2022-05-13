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
    
    EntityCharacterDragon* firstDragon;

   /* island = EntityMesh();
    island.mesh = Mesh::Get((PATH1 + s.assign("island.ASE")).c_str());
    island.texture = Texture::Get((PATH1+ s.assign("island_color.tga")).c_str());
    this->staticEntities.push_back(&island);*/

    this->ground = new EntityMesh();
    this->ground->model = Matrix44();
    this->ground->mesh = Mesh::Get("data/terrain/terrain.ASE");
    this->ground->texture = Texture::Get((PATH + s.assign("terrain/terrain.tga")).c_str());
    //this->staticEntities.push_back(&ground);

    this->sky = new EntityMesh();
    this->sky->mesh = Mesh::Get((PATH + s.assign("cielo/cielo.ASE")).c_str());
    this->sky->texture = Texture::Get((PATH + s.assign("cielo/cielo.tga")).c_str());
    //this->staticEntities.push_back(sky);

    firstDragon = new EntityCharacterDragon();
    firstDragon->mesh = Mesh::Get((PATH + s.assign("NightFury/Toothless.obj")).c_str());
    firstDragon->texture = Texture::Get((PATH + s.assign("NightFury/Toothless.png")).c_str(), true);
    firstDragon->characterMesh = Mesh::Get((PATH + s.assign("Hiccup/Hiccup.obj")).c_str());
    firstDragon->characterTex = Texture::Get((PATH + s.assign("Hiccup/HiccupTeen.png")).c_str());

    this->dynamicEntitiesDragons.push_back(firstDragon);
}
