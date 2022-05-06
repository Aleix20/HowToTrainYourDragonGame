#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"

#include <cmath>

//some globals
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;

Mesh* planeMesh = NULL;
Texture* planeTex = NULL;
Matrix44 planeModel;
bool cameraLocked = true;

Animation* anim = NULL;
float angle = 0;
float mouse_speed = 100.0f;
FBO* fbo = NULL;

Game* Game::instance = NULL;


const int planes_width = 200;
const int planes_height = 200;
float padding = 20.0f;

float loadDistance = 200.0f;
float no_render_distance = 1000.0f;

class Entity {
public:
	Matrix44 model;
	Mesh* mesh;
	Texture* texture;

};

std::vector<Entity*> entities;

Entity drake;

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
	mesh = Mesh::Get("data/island.ASE");
	texture = Texture::Get("data/island_color.tga");

	drake.mesh= Mesh::Get("data/NightFury/Toothless.obj");
	drake.texture = Texture::Get("data/NightFury/Toothless.png", true);
	Matrix44 drakeModel;
	drakeModel.rotate(DEG2RAD * 180, Vector3(0, 1, 0));
	drakeModel.setTranslation(0,0,0);
	drake.model = drakeModel;





	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}
void RenderIslands() {
	if (shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", Game::instance->camera->viewprojection_matrix);
		shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_time", time);

		//float padding = 10000.0f;
		Matrix44 m;
		for (size_t i = 0; i < 10; i++)
		{
			for (size_t j = 0; j < 10; j++)
			{
				Vector3 size = mesh->box.halfsize * 2;
				m.setTranslation(size.x * i, 0.0f, size.z * j);
				shader->setUniform("u_model", m);
				mesh->render(GL_TRIANGLES);
			}
		}

		//do the draw call

		//disable shader
		shader->disable();
	}
}


void RenderMesh(Matrix44 model, Mesh* a_mesh, Texture* tex, Shader* a_shader, Camera* cam) {
	//assert(a_mesh != null, "mesh in renderMesh was null");
	if (!a_shader) return;

	//enable shader
	a_shader->enable();

	//upload uniforms
	a_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	a_shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
	a_shader->setUniform("u_texture", tex, 0);
	a_shader->setUniform("u_time", time);

	shader->setUniform("u_model", model);
	a_mesh->render(GL_TRIANGLES);

	//disable shader
	a_shader->disable();

}
void RenderPlanes() {
	

	//enable shader
	shader->enable();
	planeTex = Texture::Get("data/spitfire_axis_color_spec.tga");
	Camera* cam = Game::instance->camera;
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
	shader->setUniform("u_texture", planeTex, 0);
	shader->setUniform("u_time", time);


	for (size_t i = 0; i < planes_width; i++)
	{
		for (size_t j = 0; j < planes_height; j++)
		{
			Matrix44 model;
			model.translate(i * padding, 0.0f, j * padding);

			Vector3 planePos = model.getTranslation();

			Vector3 camPos = cam->eye;
			float dist = planePos.distance(camPos);

			if (dist > no_render_distance) {
				continue;
			}
			Mesh* mesh = Mesh::Get("data/spitfire_low2.ASE");
			if (dist < loadDistance) {
				mesh = Mesh::Get("data/spitfire.ASE");
			}
			BoundingBox worldAABB  = transformBoundingBox(model, mesh->box);
			if (!cam->testBoxInFrustum(worldAABB.center, worldAABB.halfsize)) {
				continue;
			}
			
			shader->setUniform("u_model", model);
			mesh->render(GL_TRIANGLES);
			
		}
	}
	//disable shader
	shader->disable();
}

void AddEntityInFront(Camera* cam) {

	Vector2 mouse = Input::mouse_position;
	Game* g = Game::instance;
	Vector3 dir = cam->getRayDirection(mouse.x, mouse.y, g->window_width, g->window_height);
	Vector3 rayOrigin = cam->eye;
	RayPlaneCollision(Vector3(), Vector3(0,1,0), rayOrigin, dir );


	Vector3 spawnPos = RayPlaneCollision(Vector3(), Vector3(0, 1, 0), rayOrigin, dir);
	Matrix44 model;
	model.translate(spawnPos.x, spawnPos.y, spawnPos.z);
	Entity* entity = new Entity();
	entity->model = model;
	entity->mesh = Mesh::Get("data/Dragon_Busts_Gerhald3D.obj");
	entity->texture = Texture::Get("data/BlackDragon_Horns2_Roughness.png");

	entities.push_back(entity);
}
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

		Vector3 eye = drake.model * Vector3(0.0f, 8.0f, -5.0f);
		Vector3 center = drake.model * Vector3(0.0f, 0.0f, 10.0f);
		Vector3 up = drake.model.rotateVector(Vector3(0.0f, 1.0f, 0.0f));
		camera->enable();
		camera->lookAt(eye, center, up);

	}
	
	Matrix44 islandModel;
	RenderMesh(islandModel, mesh, texture, shader, camera);
	
	
	RenderMesh(drake.model, drake.mesh, drake.texture, shader, camera);
	RenderPlanes();
	//mesh->renderBounding(model); //Ver boundings de un modelo
	//create model matrix for cube
	for (size_t i = 0; i < entities.size(); i++)
	{
		Entity* entity = entities[i];

		RenderMesh(entity->model, entity->mesh, entity->texture, shader, camera);
	}
	//RenderIslands();

	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}


void Game::update(double seconds_elapsed)
{
	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant

	//example
	angle += (float)seconds_elapsed * 10.0f;

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_TAB)) cameraLocked = !cameraLocked;
	if (cameraLocked) {
		float planeSpeed = 50.0f * elapsed_time; 
		float rotSpeed = 90.0f * DEG2RAD * elapsed_time;
		
		if (Input::isKeyPressed(SDL_SCANCODE_W)) drake.model.translate(0.0f, 0.0f, planeSpeed );
		if (Input::isKeyPressed(SDL_SCANCODE_S)) drake.model.translate(0.0f, 0.0f, -planeSpeed);
		if (Input::isKeyPressed(SDL_SCANCODE_A)) drake.model.rotate(-rotSpeed, Vector3(0.0f,1.0f,0.0f));
		if (Input::isKeyPressed(SDL_SCANCODE_D)) drake.model.rotate(rotSpeed, Vector3(0.0f, 1.0f, 0.0f));
		if (Input::isKeyPressed(SDL_SCANCODE_Q)) drake.model.rotate(rotSpeed, Vector3(0.0f, 0.0f, 1.0f));
		if (Input::isKeyPressed(SDL_SCANCODE_E)) drake.model.rotate(-rotSpeed, Vector3(0.0f, 0.0f, 1.0f));
	}
	else {

		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
		

	}
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

