//
//  stage.h
//  TJE
//
//  Created by Alèxia  Lozano Pedrajas on 16/4/22.
//

#ifndef STAGE_H
#define STAGE_H
#include "input.h"


enum STAGE_ID {
    INTRO = 0,
    PLAY = 1
};

class Stage
{
public:
    Stage(){};
    virtual void render(void) {}; //empty body
    virtual void update(double seconds_elapsed) {}; //empty body
    virtual void onKeyDown(SDL_KeyboardEvent event){};
    virtual void onMouseButtonDown(SDL_MouseButtonEvent event) {};
};
#endif
