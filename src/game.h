/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "camera.h"
#include "utils.h"
#include "shader.h"
#include "world.h"
#include "audio.h"


class World;
class Game
{
public:
	static Game* instance;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;
    Shader* shader;
	World* world;
	int selectedEntities=0;
	int selectedBuild = 0;
	EntityMesh* currentBuild;
	float noRenderDistance = 250.0f;
	//some globals
	long frame;
    float time;
	float elapsed_time;
	int fps;
	bool must_exit;
	bool cameraLocked;
	float mouse_speed = 100.0f;
    float angle = 0;
	//some vars
	Camera* camera; //our global camera
	bool mouse_locked; //tells if the mouse is locked (not seen)

	Game( int window_width, int window_height, SDL_Window* window );

	//main functions
	void render( void );


	void update( double dt );

	//events
	void onKeyDown( SDL_KeyboardEvent event );
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);

};


#endif 
