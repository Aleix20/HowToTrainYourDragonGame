#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "entityDragon.h"
#include "entityMesh.h"

#include <cmath>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
 std::string PATH = "data/";
#else
std::string PATH = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";
#endif
//some globals
Mesh* mesh = NULL;
Texture* texture = NULL;


Mesh* planeMesh = NULL;
Texture* planeTex = NULL;
Matrix44 planeModel;


Animation* anim = NULL;
float angle = 0;

FBO* fbo = NULL;

Game* Game::instance = NULL;


const int planes_width = 200;
const int planes_height = 200;
float padding = 20.0f;

float loadDistance = 200.0f;
float no_render_distance = 1000.0f;

std::vector<EntityMesh*> entities;
EntityDragon dragon;
EntityMesh island;
EntityMesh ground;
EntityMesh sky;
std::string s;

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
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective
	
	//load one texture without using the Texture Manager (Texture::Get would use the manager)
    
	
	
	dragon = EntityDragon(Mesh::Get((PATH + s.assign("NightFury/Toothless.obj")).c_str()), Texture::Get((PATH + s.assign("NightFury/Toothless.png")).c_str(), true), Matrix44());
	cameraLocked = true;
    
    shader = Shader::Get((PATH + s.assign("shaders/basic.vs")).c_str(), (PATH + s.assign("shaders/texture.fs")).c_str());
    island = EntityMesh();
    island.model = Matrix44();
    island.mesh = Mesh::Get((PATH + s.assign("island.ASE")).c_str());
    island.texture = Texture::Get((PATH + s.assign("island_color.tga")).c_str());
    
    ground = EntityMesh();
    ground.mesh = Mesh::Get((PATH + s.assign("terrain/terrain.ASE")).c_str());
    ground.texture = Texture::Get((PATH + s.assign("terrain/terrain.tga")).c_str());
    sky = EntityMesh();
    sky.mesh = Mesh::Get((PATH + s.assign("cielo/cielo.ASE")).c_str());
    sky.texture = Texture::Get((PATH + s.assign("cielo/cielo.tga")).c_str());
    
    
	// example of shader loading using the shaders manager
	
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}


void AddEntityInFront(Camera* cam) {

	Vector2 mouse = Input::mouse_position;
	Game* g = Game::instance;
	Vector3 dir = cam->getRayDirection(mouse.x, mouse.y, g->window_width, g->window_height);
	Vector3 rayOrigin = cam->eye;

	Vector3 spawnPos = RayPlaneCollision(Vector3(), Vector3(0, 1, 0), rayOrigin, dir);
	Matrix44 model;
	model.translate(spawnPos.x, spawnPos.y, spawnPos.z);
	EntityMesh* entity = new EntityMesh();
	entity->model = model;
	entity->mesh = Mesh::Get((PATH + s.assign("Dragon_Busts_Gerhald3D.obj")).c_str());
	entity->texture = Texture::Get((PATH + s.assign("BlackDragon_Horns2_Roughness.png")).c_str());

	entities.push_back(entity);
}
//no entiendo que hace aqui
void RayPickCheck (Camera* cam){
    //esto exactamente no se lo que hace
    Vector2 mouse = Input::mouse_position;
    Game* g = Game::instance;
    Vector3 dir = cam->getRayDirection(mouse.x, mouse.y, g->window_width, g->window_height);
    Vector3 rayOrigin = cam->eye;
    
    for (size_t i = 0; entities.size(); i ++){
        Entity* entity = entities[i];
        Vector3 pos;
        Vector3 normal;
        if (entity->mesh->testRayCollision(entity->model, rayOrigin, dir, pos, normal)){
            std::cout <<"selected" <<std::endl;
            break;
        }
    };
    
};
//void RotateSelected(float angleDegrees){
//    if (selectedEntity == NULL){
//        return;
//    };
//};
//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	if (cameraLocked) {

		Vector3 eye = dragon.model * Vector3(0.0f, 40.0f,30.0f);
		Vector3 center = dragon.model * Vector3(0.0f, 0.0f, -20.0f);
		Vector3 up = dragon.model.rotateVector(Vector3(0.0f, 1.0f, 0.0f));
		camera->enable();
		camera->lookAt(eye, center, up);

	}
    
    for(int i = 0; i < entities.size(); i++ ){
        entities[i]->render();
    }
	

	dragon.render();
//    island.render();
    ground.render();
    
    Matrix44 skyModel;
    skyModel.translate(camera->eye.x, camera->eye.y -40.0f, camera->eye.z);
    sky.model = skyModel;
    glDisable(GL_DEPTH_TEST);
    sky.render();
    glEnable(GL_DEPTH_TEST);
	drawGrid();

	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
	SDL_GL_SwapWindow(this->window);
}


void Game::update(double seconds_elapsed)
{
	 
	float speed = seconds_elapsed * mouse_speed;//the speed is defined by the seconds_elapsed so it goes constant
	//example
	angle += (float)seconds_elapsed * 10.0f;
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || Game::instance->mouse_locked) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_TAB)) cameraLocked = !cameraLocked;
	if(!cameraLocked) {

		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);

	}
	//mouse input to rotate the cam
	dragon.update(seconds_elapsed);
	//async input to move the camera around
	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
		case SDLK_2: AddEntityInFront(camera); break;
	}
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

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
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
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

