//
//  stageplay.cpp
//  TJE
//
//  Created by Alèxia  Lozano Pedrajas on 16/4/22.
//

#include <stdio.h>
#include "playstage.h"

std::string b;
PlayStage::PlayStage(){}

float jumpSpeed =1.0f;
float moveSpeed = 40.0f;
void PlayStage :: render(){
    World* world = Game::instance->world;
    Camera* camera = Game::instance->camera;
    bool* cameraLocked = &Game::instance->cameraLocked;
    SDL_Window* window = Game::instance->window;
    //set the clear color (the background color)
    glClearColor(0.529f, 0.807f, 0.90f, 0.90f);

    // Clear the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //set the camera as default

    //set flags
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    Matrix44 skyModel;
    skyModel.translate(camera->eye.x, camera->eye.y -20- world->dynamicEntitiesDragons[world->currentDragon]->getPosition().y, camera->eye.z);
    world->sky->model = skyModel;
    glDisable(GL_DEPTH_TEST);
    world->sky->render();
  /*  skyModel.rotate(-180 *DEG2RAD, Vector3(1,0,0));
    world->sky->model = skyModel;
    world->sky->render();
    skyModel.rotate(180 * DEG2RAD, Vector3(0, 1, 0));*/
    glEnable(GL_DEPTH_TEST);
    world->ocean->render();
    checkFrustrumEntity(world->ground, camera->eye);
    //camera->enable();
    int currentDragon = world->currentDragon;
    if (*cameraLocked && world->topOfDragon) {

        Matrix44 currentDragonModel = world->dynamicEntitiesDragons[currentDragon]->model;
        EntityCharacterDragon* currentEntityDragon = world->dynamicEntitiesDragons[currentDragon];
        setUpCamera(currentDragonModel, Vector3(0.0f, 5.5f, 5.0f), Vector3(0.0f, 5.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), camera);
        currentEntityDragon->characterModel = currentEntityDragon->characterOffset * currentEntityDragon->model;
        currentEntityDragon->render();

    }
    else if (*cameraLocked && !world->topOfDragon) {

        EntityMesh* currentStaticDragon = world->staticEntitiesDragons[currentDragon];
        Matrix44 currentCharacterModel = world->mainCharacter->model;
        setUpCamera(currentCharacterModel, Vector3(0.0f, 5.0f, 5.0f), Vector3(0.0f, 0.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), camera);
        world->mainCharacter->render();
        checkFrustrumEntity(currentStaticDragon, camera->eye);

    }
    

    Vector3 camPos = camera->eye;
    std::vector<EntityMesh*> entities = world->staticEntities;
    checkFrustrumStatic(entities, camPos);
    entities = world->staticEntitiesCharacter;
    checkFrustrumStatic(entities, camPos);
    
    for (size_t i = 0; i < world->staticEntitiesPlants.size(); i++)
    {
        world->staticEntitiesPlants[i]->render();

    }

    //Check static dragon with camera unlocked
    if (!*cameraLocked) {
        EntityMesh* currentStaticDragon = world->staticEntitiesDragons[currentDragon];
        currentStaticDragon->render();
        //entities = world->mission1Entities;
        //checkFrustrumStatic(entities, camPos);
        //entities = world->mission2Entities;
        //checkFrustrumStatic(entities, camPos);
    }




    if (world->mission1) {
        //entities = world->mission1Entities;
        
        if (world->mission1End) {
            world->mission1EntitiesCopy = world->mission1Entities;
            world->mission1End = false;
        }
        //entitiesCopy = entities;
        if (world->missionTime > 0) {
            world->Mision1(world->mission1EntitiesCopy);
        }
        else if(world->missionTime<=0 || world->mission1EntitiesCopy.empty()) {
            world->mission1End = true;
            world->mission1 = false;
            std::cout << "Mission1 completed" << std::endl;

        }
        
        
    }else if (world->mission2) {
        //entities = world->mission1Entities;

        if (world->mission2End) {
            world->mission2EntitiesCopy = world->mission2Entities;
            world->mission2End = false;
        }
        //entitiesCopy = entities;
        if (world->missionTime > 0) {
            world->Mision2(world->mission2EntitiesCopy);
        }
        else if (world->missionTime <= 0 || world->mission2EntitiesCopy.empty()) {
            world->mission2End = true;
            world->mission2 = false;
            std::cout << "Mission2 completed" << std::endl;

        }


    }
    if (!world->topOfDragon) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        checkAudios();
#endif
    }

    //drawGrid();

    //drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
    SDL_GL_SwapWindow(window);
};

void PlayStage:: update(double seconds_elapsed){
    World* world = Game::instance->world;
    Camera* camera = Game::instance->camera;
    bool* cameraLocked = &Game::instance->cameraLocked;
    float mouse_speed = Game::instance->mouse_speed;
    float angle = Game::instance->angle;
    bool mouse_locked = Game::instance->mouse_locked;
    
    if (world->audioTimer) {
        world->timerAudio -= seconds_elapsed;
        if (world->timerAudio <= 0) {
            world->audioTimer = false;
        }
    }
    if (world->mission1) {
        world->missionTime -= seconds_elapsed;
    }
    float speed = seconds_elapsed * mouse_speed;//the speed is defined by the seconds_elapsed so it goes constant
    //example
    angle += (float)seconds_elapsed * 10.0f;
    if ((Input::mouse_state & SDL_BUTTON_LEFT) || Game::instance->mouse_locked) //is left button pressed?
    {
        camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
        camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
    }
    if (Input::wasKeyPressed(SDL_SCANCODE_TAB)) *cameraLocked = !*cameraLocked;
    if (!*cameraLocked) {

        if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
        if (Input::isKeyPressed(SDL_SCANCODE_W)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
        if (Input::isKeyPressed(SDL_SCANCODE_S)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
        if (Input::isKeyPressed(SDL_SCANCODE_A)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
        if (Input::isKeyPressed(SDL_SCANCODE_D)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);

    }

    if (world->topOfDragon) {
        int currentDragon = world->currentDragon;
        EntityCharacterDragon* currentDragonEntity = world->dynamicEntitiesDragons[currentDragon];
        currentDragonEntity->update(seconds_elapsed);
    }

    world->mainCharacter->update(seconds_elapsed);
    //mouse input to rotate the cam

    //async input to move the camera around
    //to navigate with the mouse fixed in the middle
    if (mouse_locked)
        Input::centerMouse();
    
};

void PlayStage::onKeyDown(SDL_KeyboardEvent event){
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    std::string PATH1 = "data/";
    #else
    std::string PATH1 = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";
    #endif
    World* world = Game::instance->world;
    bool must_exit = Game::instance->must_exit;
    float elapsed_time = Game::instance->elapsed_time;
    int selectedEntities = Game::instance->selectedEntities;
    Camera* camera = Game::instance->camera;
    EntityMesh* currentBuild = Game::instance->currentBuild;
    int selectedBuild = Game::instance->selectedBuild;
    
    Vector3 scale = Vector3(1, 1, 1);
    EntityMesh* dragon = world->staticEntitiesDragons[world->currentDragon];
    EntityCharacterDragon* dragonDynamic = world->dynamicEntitiesDragons[world->currentDragon];
    switch (event.keysym.sym)
    {
    case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
    case SDLK_F1: Shader::ReloadAll(); break;
    case SDLK_F2: MoveSelected(0, -30.0f * elapsed_time, 0); break;
    case SDLK_F3: MoveSelected(0, 30.0f * elapsed_time, 0); break;
    case SDLK_F4: scale = scale - Vector3(0.1, 0.1, 0.1); ScaleSelected(scale.x, scale.y, scale.z); break;
    case SDLK_F5: scale = scale + Vector3(0.1, 0.1, 0.1); ScaleSelected(scale.x, scale.y, scale.z); break;
    case SDLK_F6:
        switch (selectedEntities) {
        case 0:
            RemoveSelected(world->staticEntities, world->selectedEntity);
            break;
        case 1:
            RemoveSelected(world->staticEntitiesCharacter,world->selectedEntity);
            break;
        case 2:
            RemoveSelected(world->staticEntitiesDragons,world->selectedEntity);
            break;
        case 3:
            RemoveSelected(world->mission1Entities,world->selectedEntity);
            break;
        case 4:
            RemoveSelected(world->staticEntitiesPlants,world->selectedEntity);
            break;
        case 5:
            RemoveSelected(world->mission2Entities, world->selectedEntity);
            break;
        }
        break;  //remove
    case SDLK_g: world->writeObjectFile((PATH1 + b.assign("objects.txt")).c_str()); break;
    case SDLK_2:
        switch (selectedEntities) {
        case 0:
            AddEntityInFront(camera, currentBuild, world->staticEntities);
            break;
        case 3:
            AddEntityInFront(camera, currentBuild, world->mission1Entities);
            break;
        case 4:
            AddEntityInFront(camera, currentBuild, world->staticEntitiesPlants);
            break;
        case 5:
            AddEntityInFront(camera, currentBuild, world->mission2Entities);
            break;
        default:
            AddEntityInFront(camera, currentBuild, world->staticEntities);
            break;
        }
        
        break;
    case SDLK_UP: if (world->selectedEntity == NULL) { break; } MoveSelected(0, 0, -30.0f * elapsed_time); break;
    case SDLK_DOWN: if (world->selectedEntity == NULL) { break; } MoveSelected(0, 0, 30.0f * elapsed_time); break;
    case SDLK_LEFT: if (world->selectedEntity == NULL) { break; } MoveSelected(-30.0f * elapsed_time, 0, 0); break;
    case SDLK_RIGHT: if (world->selectedEntity == NULL) { break; } MoveSelected(30.0f * elapsed_time, 0, 0); break;
    case SDLK_0:
        world->playStage = !world->playStage;
        break;
    case SDLK_3:
        
        if (world->mainCharacter->getPosition().distance(dragon->getPosition()) < 10.0f) {

            if (world->topOfDragon && dragonDynamic->checkInsidePlane() && dragonDynamic->getPosition().y < 1) {
                Vector3 position = dragon->getPosition();
                Vector3 positionDynamic = dragonDynamic->getPosition();
                world->topOfDragon = !world->topOfDragon;
                //world->mainCharacter->model.setTranslation((position.x-7), 0, position.z);
                dragon->model = dragonDynamic->model;
                float angle3 = dragonDynamic->angle3;
                float angle2 = dragonDynamic->angle2;
                
                dragon->model.rotate(-angle3 * DEG2RAD, Vector3(1, 0, 0));
                dragonDynamic->model.rotate(-angle3 * DEG2RAD, Vector3(1, 0, 0));
                dragon->model.rotate(-angle2 * DEG2RAD, Vector3(0, 0, 1));
                dragonDynamic->model.rotate(-angle2 * DEG2RAD, Vector3(0, 0, 1));
                dragonDynamic->angle3 = 0;
                dragonDynamic->angle2 = 0;
                
            }
            else {
                
                world->topOfDragon = true;
            }
        }
        break;
    case SDLK_f:checkGameState();  break;
    case SDLK_MINUS: RotateSelected(40.0f * elapsed_time, Vector3(0, 1, 0)); break;
    case SDLK_PLUS:  RotateSelected(-40.0f * elapsed_time, Vector3(0, 1, 0)); break;
    case SDLK_l: RotateSelected(40.0f * elapsed_time, Vector3(1, 0, 0)); break;
    case SDLK_k:  RotateSelected(-40.0f * elapsed_time, Vector3(1, 0, 0)); break;
    case SDLK_z:
        if (selectedEntities == 5) {
            selectedEntities = 0;
        }
        else {

            selectedEntities++;
        }
        switch (selectedEntities) {
        case 0:
            std::cout << "staticEnties" << std::endl;
            break;
        case 1:
            std::cout << "staticEntitiesCharacter" << std::endl;
            break;
        case 2:
            std::cout << "staticEntitiesDragons" << std::endl;
            break;
        case 3:
            std::cout << "Mission1 ITEMS" << std::endl;
            break;
        case 4:
            std::cout << "Plants" << std::endl;
            break;
        case 5:
            std::cout << "Mission2 ITEMS" << std::endl;
            break;
        }
        break;

    case SDLK_PERIOD:
        if (selectedBuild == world->buildWorld.size() - 1) {
            selectedBuild = 0;
            currentBuild = world->buildWorld[selectedBuild];
            std::cout << currentBuild->name << std::endl;
        }
        else {
            selectedBuild++;
            currentBuild = world->buildWorld[selectedBuild];
            std::cout << currentBuild->name << std::endl;
        }
        break;
    case SDLK_COMMA:
        if (selectedBuild == 0) {
            selectedBuild = world->buildWorld.size() - 1;
            currentBuild = world->buildWorld[selectedBuild];
            std::cout << currentBuild->name << std::endl;
        }
        else {
            selectedBuild--;
            currentBuild = world->buildWorld[selectedBuild];
            std::cout << currentBuild->name << std::endl;
        }
        break;



    }
    
}
void PlayStage::onMouseButtonDown(SDL_MouseButtonEvent event)
{
    World* world = Game::instance->world;
    int selectedEntities = Game::instance->selectedEntities;
    Camera* camera = Game::instance->camera;
    bool mouse_locked = Game::instance->mouse_locked;
    if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
    {
        mouse_locked = !mouse_locked;
        SDL_ShowCursor(!mouse_locked);
    }
    if (event.button == SDL_BUTTON_LEFT) {
        switch (selectedEntities) {
        case 0:
            RayPickCheck(camera, world->staticEntities);
            break;
        case 1:
            RayPickCheck(camera, world->staticEntitiesCharacter);
            break;
        case 2:
            RayPickCheck(camera, world->staticEntitiesDragons);
            break;
        case 3:
            RayPickCheck(camera, world->mission1Entities);
            break;
        case 4:
            RayPickCheck(camera, world->staticEntitiesPlants);
            break;
        case 5:
            RayPickCheck(camera, world->mission2Entities);
            break;
        }
    }
}
;
