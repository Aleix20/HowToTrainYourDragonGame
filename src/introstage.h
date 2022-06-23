//
//  introstage.h
//  TJE
//
//  Created by Alèxia  Lozano Pedrajas on 19/4/22.
//

#ifndef introstage_h
#define introstage_h

#include "stage.h"
#include "game.h"



class IntroStage: public Stage{
public:
   
    
    IntroStage();
    virtual void render(void) override; //empty body
    virtual void update(double seconds_elapsed) override;//empty body
    virtual void onKeyDown(SDL_KeyboardEvent event) override;
    virtual void onMouseButtonDown(SDL_MouseButtonEvent event)override;
    bool wasLeftPressed;
    
};
#endif /* introstage_h */
