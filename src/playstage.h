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

class PlayStage: public Stage{
public:
    PlayStage();
    virtual void render(void) override; //empty body
    virtual void update(double seconds_elapsed) override;//empty body
    virtual void onKeyDown(SDL_KeyboardEvent event)override;
    
    
};

#endif /* playstage_h */
