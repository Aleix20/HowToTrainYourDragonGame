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

	if (BASS_Init(-1, 44100, 0, 0, NULL) == false) //-1 significa usar el por defecto del sistema operativo
	{
		//error abriendo la tarjeta de sonido...
	}
	Audio::Play((PATH1 + a.assign("queXulo.wav")).c_str());
	
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
	

	Vector3 camPos = camera->eye;
	std::vector<EntityMesh*> entities = world->staticEntities;
	checkFrustrumStatic(entities, camPos);
	entities = world->staticEntitiesCharacter;
	checkFrustrumStatic(entities, camPos);
	
	for (size_t i = 0; i < world->staticEntitiesPlants.size(); i++)
	{
		world->staticEntitiesPlants[i]->render();
	}

	//Check static dragon with camera unlocked
	if (!cameraLocked) {
		EntityMesh* currentStaticDragon = world->staticEntitiesDragons[currentDragon];
		currentStaticDragon->render();
		entities = world->mission1Entities;
		checkFrustrumStatic(entities, camPos);
	}




	if (world->mission1) {
		//entities = world->mission1Entities;
		
		if (world->mission1End) {
			world->mission1EntitiesCopy = world->mission1Entities;
			world->mission1End = false;
		}
        //entitiesCopy = entities;
		if (world->missionTime > 0) {
			world->Mision1(world->mission1EntitiesCopy);
		}
		else if(world->missionTime<=0 || world->mission1EntitiesCopy.empty()) {
			world->mission1End = true;
			world->mission1 = false;

		}
        
		
	}



	drawGrid();

	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
	SDL_GL_SwapWindow(this->window);
}






void Game::update(double seconds_elapsed)
{
	if (world->mission1) {
		world->missionTime -= seconds_elapsed;
	}
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

	Vector3 scale = Vector3(1, 1, 1);
	EntityMesh* dragon = world->staticEntitiesDragons[world->currentDragon];
	EntityCharacterDragon* dragonDynamic = world->dynamicEntitiesDragons[world->currentDragon];
	switch (event.keysym.sym)
	{
	case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
	case SDLK_F1: Shader::ReloadAll(); break;
	case SDLK_F2: MoveSelected(0, -30.0f * elapsed_time, 0); break;
	case SDLK_F3: MoveSelected(0, 30.0f * elapsed_time, 0); break;
	case SDLK_F4: scale = scale - Vector3(0.1, 0.1, 0.1); ScaleSelected(scale.x, scale.y, scale.z); break;
	case SDLK_F5: scale = scale + Vector3(0.1, 0.1, 0.1); ScaleSelected(scale.x, scale.y, scale.z); break;
	case SDLK_F6:
		switch (selectedEntities) {
		case 0:
			RemoveSelected(world->staticEntities, world->selectedEntity);
			break;
		case 1:
			RemoveSelected(world->staticEntitiesCharacter,world->selectedEntity);
			break;
		case 2:
			RemoveSelected(world->staticEntitiesDragons,world->selectedEntity);
			break;
		case 3:
			RemoveSelected(world->mission1Entities,world->selectedEntity);
			break;
		case 4:
			RemoveSelected(world->staticEntitiesPlants,world->selectedEntity);
			break;
		}
		break;  //remove
	case SDLK_g: world->writeObjectFile((PATH1 + a.assign("objects.txt")).c_str()); break;
	case SDLK_2:
		switch (selectedEntities) {
		case 0:
			AddEntityInFront(camera, currentBuild, world->staticEntities);
			break;
		case 3:
			AddEntityInFront(camera, currentBuild, world->mission1Entities);
			break;
		case 4:
			AddEntityInFront(camera, currentBuild, world->staticEntitiesPlants);
			break;
		default:
			AddEntityInFront(camera, currentBuild, world->staticEntities);
			break;
		}
		
		break;
	case SDLK_UP: if (world->selectedEntity == NULL) { break; } MoveSelected(0, 0, -30.0f * elapsed_time); break;
	case SDLK_DOWN: if (world->selectedEntity == NULL) { break; } MoveSelected(0, 0, 30.0f * elapsed_time); break;
	case SDLK_LEFT: if (world->selectedEntity == NULL) { break; } MoveSelected(-30.0f * elapsed_time, 0, 0); break;
	case SDLK_RIGHT: if (world->selectedEntity == NULL) { break; } MoveSelected(30.0f * elapsed_time, 0, 0); break;
	case SDLK_3:
		
		if (world->mainCharacter->getPosition().distance(dragon->getPosition()) < 10.0f) {

			if (world->topOfDragon && dragonDynamic->checkInsidePlane() && dragonDynamic->getPosition().y < 1) {
				Vector3 position = dragon->getPosition();
				Vector3 positionDynamic = dragonDynamic->getPosition();
				world->topOfDragon = !world->topOfDragon;
				//world->mainCharacter->model.setTranslation((position.x-7), 0, position.z);
				dragon->model = dragonDynamic->model;
				float angle3 = dragonDynamic->angle3;
				float angle2 = dragonDynamic->angle2;
				
				dragon->model.rotate(-angle3 * DEG2RAD, Vector3(1, 0, 0));
				dragonDynamic->model.rotate(-angle3 * DEG2RAD, Vector3(1, 0, 0));
				dragon->model.rotate(-angle2 * DEG2RAD, Vector3(0, 0, 1));
				dragonDynamic->model.rotate(-angle2 * DEG2RAD, Vector3(0, 0, 1));
				dragonDynamic->angle3 = 0;
				dragonDynamic->angle2 = 0;
				
			}
			else {
				
				world->topOfDragon = true;
			}
		}
		break;
	case SDLK_f:checkGameState(); break;
	case SDLK_MINUS: RotateSelected(40.0f * elapsed_time); break;
	case SDLK_PLUS:  RotateSelected(-40.0f * elapsed_time); break;
	case SDLK_z:
		if (selectedEntities == 4) {
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
		case 3:
			std::cout << "Mission1 ITEMS" << std::endl;
			break;
		case 4:
			std::cout << "Plants" << std::endl;
			break;
		}
		break;

	case SDLK_PERIOD:
		if (selectedBuild == world->buildWorld.size() - 1) {
			selectedBuild = 0;
			currentBuild = world->buildWorld[selectedBuild];
			std::cout << currentBuild->name << std::endl;
		}
		else {
			selectedBuild++;
			currentBuild = world->buildWorld[selectedBuild];
			std::cout << currentBuild->name << std::endl;
		}
		break;
	case SDLK_COMMA:
		if (selectedBuild == 0) {
			selectedBuild = world->buildWorld.size() - 1;
			currentBuild = world->buildWorld[selectedBuild];
			std::cout << currentBuild->name << std::endl;
		}
		else {
			selectedBuild--;
			currentBuild = world->buildWorld[selectedBuild];
			std::cout << currentBuild->name << std::endl;
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
	if (event.button == SDL_BUTTON_LEFT) {
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
		case 3:
			RayPickCheck(camera, world->mission1Entities);
			break;
		case 4:
			RayPickCheck(camera, world->staticEntitiesPlants);
			break;
		}
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

