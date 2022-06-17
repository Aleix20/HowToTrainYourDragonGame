//
//  introstage.h
//  TJE
//
//  Created by Alèxia  Lozano Pedrajas on 19/4/22.
//

#ifndef introstage_h
#define introstage_h

#include "stage.h"



class IntroStage: public Stage{
public:
   
    
    IntroStage();
    virtual void render(void) override; //empty body
    virtual void update(double seconds_elapsed) override;//empty body
    virtual void onKeyDown(SDL_KeyboardEvent event) override;

};
#endif /* introstage_h */
