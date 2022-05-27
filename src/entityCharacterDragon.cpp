#include "entityCharacterDragon.h"
#include "input.h"
#include "utils.h"
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
	Game* g = Game::instance;
	bool cameraLocked = g->cameraLocked;
	Camera* cam = g->camera;

	if (cameraLocked) {

		float rotSpeed = 110.0f * dt;


		if (Input::isKeyPressed(SDL_SCANCODE_A)) {
			angle -= (rotSpeed * 0.5f);
			if (angle2 > -35.0f) {
				angle2 -= rotSpeed;
			}
		}
		if (Input::isKeyPressed(SDL_SCANCODE_D)) {
			angle += (rotSpeed * 0.5f);
			if (angle2 < 35.0f) {
				angle2 += rotSpeed;
			}
		}

        
		Matrix44 dragonRotation;
		dragonRotation.rotate(angle * DEG2RAD, Vector3(0, 1, 0));

		Vector3 forward = dragonRotation.rotateVector(Vector3(0, 0, 1));
        Vector3 v = dragonRotation.rotateVector(Vector3(0, 1, 0));

		Vector3 dragonVel;

		if (Input::isKeyPressed(SDL_SCANCODE_W)) {
			dragonVel = dragonVel - (forward * dragonSpeed * dt);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_S)) {
			dragonVel = dragonVel + (forward * dragonSpeed * dt);
			
		}
		if (Input::isKeyPressed(SDL_SCANCODE_UP)) {
			dragonVel = dragonVel + (v * dragonSpeed * dt);
			if (angle3 > -25.0f) {
				angle3 -= (rotSpeed * 0.5f) ;
			}
		}
		if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
			dragonVel = dragonVel - (v * dragonSpeed * dt);
			if (angle3 < 25.0f) {
				angle3 += (rotSpeed * 0.5f);
			}
		}
        
		Vector3 nexPos = getPosition() + dragonVel;
		Vector3 character_center = nexPos + Vector3(0, 2, 0);
        std::vector<EntityMesh*> entities2 = g->world->staticEntities;
        Vector3 current = getPosition();
        checkCollisionEntities(entities2, character_center, dt, nexPos, current);
        entities2 = g->world->staticEntitiesCharacter;
        checkCollisionEntities(entities2, character_center, dt, nexPos, current);
        //No se puede volar por debajo del suelo
        if (nexPos.y < 0) nexPos.y = current.y;
        //'Muros invisibles'
        if (nexPos.x > 200 || nexPos.x < -200) nexPos.x = current.x;
        if (nexPos.z > 200 || nexPos.z < -200) nexPos.z = current.z;
        model.setTranslation(nexPos.x, nexPos.y, nexPos.z);
        model.rotate(angle * DEG2RAD, Vector3(0, 1, 0));
        model.rotate(angle2 * DEG2RAD, Vector3(0, 0, 1));
		model.rotate(angle3 * DEG2RAD, Vector3(1, 0, 0));
	}
}
