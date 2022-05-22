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


	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}




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
	skyModel.translate(camera->eye.x, camera->eye.y - 40.0f, camera->eye.z);
	world->sky->model = skyModel;
	glDisable(GL_DEPTH_TEST);
	world->sky->render();
	glEnable(GL_DEPTH_TEST);
	world->ocean->render();
	checkFrustrumEntity(world->ground, camera->eye);
	//camera->enable();
	int currentDragon = world->currentDragon;
	if (cameraLocked && world->topOfDragon) {

		Matrix44 currentDragonModel = world->dynamicEntitiesDragons[currentDragon]->model;
		EntityCharacterDragon* currentEntityDragon = world->dynamicEntitiesDragons[currentDragon];
		setUpCamera(currentDragonModel, Vector3(0.0f, 5.5f, 5.0f), Vector3(0.0f, 5.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), camera);
		currentEntityDragon->characterModel = currentEntityDragon->characterOffset * currentEntityDragon->model;
		currentEntityDragon->render();

	}
	else if (cameraLocked && !world->topOfDragon) {

		EntityMesh* currentStaticDragon = world->staticEntitiesDragons[currentDragon];
		Matrix44 currentCharacterModel = world->mainCharacter->model;
		setUpCamera(currentCharacterModel, Vector3(0.0f, 5.0f, 5.0f), Vector3(0.0f, 0.0f, -5.0f), Vector3(0.0f, 1.0f, 0.0f), camera);
		world->mainCharacter->render();
		checkFrustrumEntity(currentStaticDragon, camera->eye);

	}
	//Check static dragon with camera unlocked
	if (!cameraLocked) {
		EntityMesh* currentStaticDragon = world->staticEntitiesDragons[currentDragon];
		currentStaticDragon->render();
	}

	Vector3 camPos = camera->eye;
	std::vector<EntityMesh*> entities = world->staticEntities;
	checkFrustrumStatic(entities, camPos);
	entities = world->staticEntitiesCharacter;
	checkFrustrumStatic(entities, camPos);






	if (world->mission1) {
		EntityMesh* table1 = new EntityMesh();
		table1->mesh = Mesh::Get((PATH1 + a.assign("aldeas/table.obj")).c_str());
		table1->texture = Texture::Get((PATH1 + a.assign("aldeas/table.png")).c_str(), true);
		Matrix44 tableModel1 = Matrix44();
		tableModel1.translate(4, 5, 3);
		table1->model = tableModel1;
		table1->render();
	}



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
	if (!cameraLocked) {

		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);

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
void Game::onKeyDown(SDL_KeyboardEvent event)
{

	Vector3 scale = Vector3(1,1,1);
	switch (event.keysym.sym)
	{
	case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
	case SDLK_F1: Shader::ReloadAll(); break;
	case SDLK_F2: MoveSelected(0, -10.0f * elapsed_time, 0); break;
	case SDLK_F3: MoveSelected(0, 10.0f * elapsed_time, 0); break;
	case SDLK_F4: scale = scale - Vector3(0.1, 0.1, 0.1); ScaleSelected(scale.x,scale.y,scale.z); break;
	case SDLK_F5: scale = scale + Vector3(0.1, 0.1, 0.1); ScaleSelected(scale.x, scale.y, scale.z); break;
	case SDLK_F6:break; //remove
	case SDLK_g: world->writeObjectFile((PATH1+ a.assign("objects.txt")).c_str()); break;
	case SDLK_1:
		switch (selectedEntities) {
		case 0:
			RayPickCheck(camera, world->staticEntities);
			break;
		case 1:
			RayPickCheck(camera, world->staticEntitiesCharacter);
			break;
		case 2:
			RayPickCheck(camera, world->staticEntitiesDragons);
			break;
		}
		break;
	case SDLK_2: AddEntityInFront(camera); break;
	case SDLK_UP: MoveSelected(0, 0, -10.0f * elapsed_time); break;
	case SDLK_DOWN:MoveSelected(0, 0, 10.0f * elapsed_time); break;
	case SDLK_LEFT: MoveSelected(-10.0f * elapsed_time, 0, 0); break;
	case SDLK_RIGHT:MoveSelected(10.0f * elapsed_time, 0, 0); break;
	case SDLK_3:
		if (world->mainCharacter->getPosition().distance(Vector3(5, 1.5, 5)) < 10.0f) {

			if (world->topOfDragon) {
				world->topOfDragon = !world->topOfDragon;
			}
			else {
				world->dynamicEntitiesDragons[world->currentDragon]->model.setTranslation(5, 1.5, 5);
				world->topOfDragon = true;
			}
		}
		break;
	case SDLK_f:checkGameState(); break;
	
	case SDLK_MINUS: RotateSelected(20.0f * elapsed_time); break;
	case SDLK_PLUS:  RotateSelected(-20.0f * elapsed_time); break;
	case SDLK_LESS:
		if (selectedEntities == 2) {
			selectedEntities = 0;
		}
		else {

			selectedEntities++;
		}
		switch (selectedEntities) {
		case 0:
			std::cout << "staticEnties" << std::endl;
			break;
		case 1:
			std::cout << "staticEntitiesCharacter" << std::endl;
			break;
		case 2:
			std::cout << "staticEntitiesDragons" << std::endl;
			break;
		}
		break;




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

void Game::onMouseButtonDown(SDL_MouseButtonEvent event)
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
	glViewport(0, 0, width, height);
	camera->aspect = width / (float)height;
	window_width = width;
	window_height = height;
}

