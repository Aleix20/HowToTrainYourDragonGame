#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "playstage.h"
#include "introstage.h"
#include "stage.h"

#include <cmath>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
HCHANNEL channel;
std::string PATH1 = "data/";
#else
std::string PATH1 = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";
#endif
//some globals
Mesh* mesh = NULL;
Texture* texture = NULL;



Mesh* planeMesh = NULL;
Texture* planeTex = NULL;
Matrix44 planeModel;


Animation* anim = NULL;


FBO* fbo = NULL;

Game* Game::instance = NULL;


const int planes_width = 200;
const int planes_height = 200;
float padding = 20.0f;

float loadDistance = 200.0f;
float no_render_distance = 1000.0f;

//std::vector<EntityMesh*> entities;

STAGE_ID currentStage;
std::vector<Stage*> stages;


Stage* getStage(STAGE_ID id){
    return stages[(int)id];
};

Stage* getCurrentStage(){
    return getStage(currentStage);
};

 void setStage(STAGE_ID id){
    currentStage = id;
};

void initStages(){
    stages.reserve(2);
    stages.push_back(new IntroStage());
    stages.push_back(new PlayStage());
    

};


std::string a;
Game::Game(int window_width, int window_height, SDL_Window* window)
{

	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	//OpenGL flags
	glEnable(GL_CULL_FACE); //render both sides of every triangle
	glEnable(GL_DEPTH_TEST); //check the occlusions using the Z buffer


	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 1000.f); //set the projection, we want to be perspective
	shader = Shader::Get((PATH1 + a.assign("shaders/basic.vs")).c_str(), (PATH1 + a.assign("shaders/texture.fs")).c_str());

	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	world = new World();
	cameraLocked = true;
    initStages();
    setStage(STAGE_ID::INTRO);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	if (BASS_Init(-1, 44100, 0, 0, NULL) == false) //-1 significa usar el por defecto del sistema operativo
	{
		//error abriendo la tarjeta de sonido...
	}
	channel = Audio::Play((PATH1 + a.assign("sounds/background_music.wav")).c_str(), BASS_SAMPLE_LOOP);
#endif
	
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
    
}




void Game::render(void)
{
    getCurrentStage()->render();
}






void Game::update(double seconds_elapsed)
{
	if (world->playStage && currentStage == STAGE_ID::INTRO) {
		//BOOL error = Audio::Stop(channel);
		setStage(STAGE_ID::PLAY);
	}
	if(!world->playStage && currentStage == STAGE_ID::PLAY) {
		setStage(STAGE_ID::INTRO);

	}
    getCurrentStage()->update(seconds_elapsed);
}

//Keyboard event handler (sync input)
void Game::onKeyDown(SDL_KeyboardEvent event)
{
	getCurrentStage()->onKeyDown(event);
	
}



void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown(SDL_MouseButtonEvent event)
{
	getCurrentStage()->onMouseButtonDown(event);

}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1 : 0.9;
}

void Game::onResize(int width, int height)
{
	std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport(0, 0, width, height);
	camera->aspect = width / (float)height;
	window_width = width;
	window_height = height;
}

