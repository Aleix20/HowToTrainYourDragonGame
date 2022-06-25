//
//  playstage.h
//  TJE
//
//  Created by Alèxia  Lozano Pedrajas on 19/4/22.
//

#ifndef playstage_h
#define playstage_h

#include "stage.h"
#include "input.h"
#include "game.h"

class PlayStage: public Stage{
public:
    PlayStage();
    virtual void render(void) override; //empty body
    void checkMissions(World* world);
    virtual void update(double seconds_elapsed) override;//empty body
    virtual void onKeyDown(SDL_KeyboardEvent event)override;
    void decreaseBuild(World* world, EntityMesh*& currentBuild);
    void increaseBuild(World* world, EntityMesh*& currentBuild);
    void selectEntities(World* world);
    void checkTopOfDragon(World* world, EntityMesh* dragon, EntityCharacterDragon* dragonDynamic);
    virtual void onMouseButtonDown(SDL_MouseButtonEvent event)override;
    
};

#endif /* playstage_h */
