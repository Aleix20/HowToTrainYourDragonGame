
#ifndef TutorialStage_h
#define TutorialStage_h

#include "stage.h"
#include "input.h"
#include "game.h"

class TutorialStage: public Stage
{
public:
    std::vector<Texture*> tutorialImages; 
    Mesh* quadTutorial;
    int currentTutorial = 0;
    bool wasLeftPressed;
	TutorialStage();
    virtual void render(void) override; //empty body
    virtual void update(double seconds_elapsed) override;//empty body
    virtual void onKeyDown(SDL_KeyboardEvent event)override;
    virtual void onMouseButtonDown(SDL_MouseButtonEvent event)override;

};
#endif /*Tutorial_h*/
