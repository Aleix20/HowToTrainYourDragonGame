#include "entityCharacterDragon.h"
#include "input.h"

EntityCharacterDragon::EntityCharacterDragon()
{
}

void EntityCharacterDragon::render()
{
	Shader* shader = Game::instance->shader;
	if (!shader) return;


	//enable shader
	shader->enable();

	//upload uniforms
	Camera* cam = Game::instance->camera;
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
	shader->setUniform("u_texture", texture, 0);
	shader->setUniform("u_time", time);
	shader->setUniform("u_model", model);
	shader->setUniform("u_tex_tiling", tiling);
	mesh->render(GL_TRIANGLES);
	shader->setUniform("u_texture", characterTex, 0);
	shader->setUniform("u_model", characterModel);
	characterMesh->render(GL_TRIANGLES);

	//disable shader
	shader->disable();
}

void EntityCharacterDragon::update(float dt)
{
	bool cameraLocked = Game::instance->cameraLocked;
	Camera* cam = Game::instance->camera;

	if (cameraLocked) {
		float rotSpeed = 90.0f * DEG2RAD * dt;

		if (Input::isKeyPressed(SDL_SCANCODE_W)) model.translate(0.0f, 0.0f, -dragonSpeed * dt);
		if (Input::isKeyPressed(SDL_SCANCODE_S)) model.translate(0.0f, 0.0f, dragonSpeed * dt);
		if (Input::isKeyPressed(SDL_SCANCODE_A)) model.rotate(-rotSpeed, Vector3(0.0f, 1.0f, 0.0f));
		if (Input::isKeyPressed(SDL_SCANCODE_D)) model.rotate(rotSpeed, Vector3(0.0f, 1.0f, 0.0f));
		if (Input::isKeyPressed(SDL_SCANCODE_Q)) model.rotate(-rotSpeed, Vector3(0.0f, 0.0f, 1.0f));
		if (Input::isKeyPressed(SDL_SCANCODE_E)) model.rotate(rotSpeed, Vector3(0.0f, 0.0f, 1.0f));
	}
}
