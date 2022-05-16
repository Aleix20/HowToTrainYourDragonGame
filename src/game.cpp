#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"


#include <cmath>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
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

EntityMesh* bomb = new EntityMesh();
Matrix44 bombOffset;

Animation* anim = NULL;
float angle = 0;

FBO* fbo = NULL;

Game* Game::instance = NULL;


const int planes_width = 200;
const int planes_height = 200;
float padding = 20.0f;

float loadDistance = 200.0f;
float no_render_distance = 1000.0f;

//std::vector<EntityMesh*> entities;



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
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

    
	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective
    shader = Shader::Get((PATH1 + a.assign("shaders/basic.vs")).c_str(), (PATH1 + a.assign("shaders/texture.fs")).c_str());
	
	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	world = new World();
    bombOffset.setTranslation(0.0f, 2.0f, 0.0f);
    bomb->mesh = Mesh::Get((PATH1 + a.assign("Hiccup/Hiccup.obj")).c_str());;
    bomb->texture = Texture::Get((PATH1 + a.assign("Hiccup/HiccupTeen.png")).c_str(), true);;
    bomb->model = Matrix44();
	cameraLocked = true;
    

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}


//void AddEntityInFront(Camera* cam) {
//
//	Vector2 mouse = Input::mouse_position;
//	Game* g = Game::instance;
//	Vector3 dir = cam->getRayDirection(mouse.x, mouse.y, g->window_width, g->window_height);
//	Vector3 rayOrigin = cam->eye;
//
//	Vector3 spawnPos = RayPlaneCollision(Vector3(), Vector3(0, 1, 0), rayOrigin, dir);
//	Matrix44 model;
//	model.translate(spawnPos.x, spawnPos.y, spawnPos.z);
//	EntityMesh* entity = new EntityMesh();
//	entity->model = model;
//	entity->mesh = Mesh::Get((PATH1 + a.assign("Dragon_Busts_Gerhald3D.obj")).c_str());
//	entity->texture = Texture::Get((PATH1 + a.assign("BlackDragon_Horns2_Roughness.png")).c_str());
//
//	entities.push_back(entity);
//}
//no entiendo que hace aqui
//void RayPickCheck (Camera* cam){
//    //esto exactamente no se lo que hace
//    Vector2 mouse = Input::mouse_position;
//    Game* g = Game::instance;
//    Vector3 dir = cam->getRayDirection(mouse.x, mouse.y, g->window_width, g->window_height);
//    Vector3 rayOrigin = cam->eye;
//    
//    for (size_t i = 0; entities.size(); i ++){
//        Entity* entity = entities[i];
//        Vector3 pos;
//        Vector3 normal;
//        if (entity->mesh->testRayCollision(entity->model, rayOrigin, dir, pos, normal)){
//            std::cout <<"selected" <<std::endl;
//            break;
//        }
//    };
//    
//};

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
    Matrix44 skyModel;
    skyModel.translate(camera->eye.x, camera->eye.y -40.0f, camera->eye.z);
	world->sky->model = skyModel;
    glDisable(GL_DEPTH_TEST);
	world->sky->render();
    glEnable(GL_DEPTH_TEST);
	//camera->enable();
	int currentDragon = world->currentDragon;
	if (cameraLocked && world->topOfDragon) {
		
		Matrix44 currentDragonModel = world->dynamicEntitiesDragons[currentDragon]->model;
		EntityCharacterDragon* currentEntityDragon = world->dynamicEntitiesDragons[currentDragon];
		Vector3 eye = currentDragonModel * Vector3(0.0f, 5.5f, 5.0f);
		Vector3 center = currentDragonModel * Vector3(0.0f, 5.0f, -5.0f);
		Vector3 up = currentDragonModel.rotateVector(Vector3(0.0f, 1.0f, 0.0f));
		camera->enable();
		camera->lookAt(eye, center, up);
		currentEntityDragon->render();
        bomb->model =  bombOffset* currentDragonModel ;
        bomb->render();

	}
	else if (cameraLocked && !world->topOfDragon) {
		EntityMesh* currentStaticDragon = world->staticEntitiesDragons[currentDragon];
		Matrix44 currentCharacterModel = world->mainCharacter->model;
		Vector3 eye = currentCharacterModel * Vector3(0.0f, 5.0f, 5.0f);
		Vector3 center = currentCharacterModel * Vector3(0.0f, 0.0f, -5.0f);
		Vector3 up = currentCharacterModel.rotateVector(Vector3(0.0f, 1.0f, 0.0f));
		camera->enable();
		camera->lookAt(eye, center, up);
		world->mainCharacter->render();
		currentStaticDragon->render();

		
	}

	
    /*for(int i = 0; i < world->staticEntities.size(); i++ ){
        world->staticEntities[i]->render();
    }*/
	
    for(int i = 0; i < world->staticEntitiesCharacter.size(); i++ ){
        EntityMesh* currentCharacter =world->staticEntitiesCharacter[i];
        currentCharacter->render();
       
    }
	world->staticEntities[0]->render();
	if (world->mission1) {
		world->staticEntities[1]->render();
	}
    
    
	
	//world->ground->render();
    
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
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
		//case SDLK_2: AddEntityInFront(camera); break;
		case SDLK_3:
			if (world->mainCharacter->getPosition().distance(Vector3(5, 0, 5))<10.0f) {

				if (world->topOfDragon) {
					world->topOfDragon = !world->topOfDragon;
				}
				else {
					world->topOfDragon = true;
				}
			}
			break;
        case SDLK_f:
            for(int i = 0; i < world->staticEntitiesCharacter.size(); i++ ){
                EntityMesh* currentCharacter =world->staticEntitiesCharacter[i];
                Vector3 currentCharacterPosition = currentCharacter->getPosition();
                if(currentCharacterPosition.distance(world->mainCharacter->getPosition()) < 10.0f){
					
                    if(currentCharacter->name.compare("Mission1")==0){
						world->mission1 = true;
                    }
                }
               
            }
            
            

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

