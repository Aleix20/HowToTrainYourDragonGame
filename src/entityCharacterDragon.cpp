#include "entityCharacterDragon.h"
#include "input.h"
#include "utils.h"

EntityCharacterDragon::EntityCharacterDragon()
{
}

void EntityCharacterDragon::render()
{
	float time = Game::instance->time;
	std::string a;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	std::string PATH1 = "data/";
#else
	std::string PATH1 = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";
#endif
	Shader* shader;
	shader = Game::instance->shader;
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
	shader->disable();

	if (animations.size() != 0) {
		shader = Shader::Get((PATH1 + a.assign("shaders/skinning.vs")).c_str(), (PATH1 + a.assign("shaders/texture.fs")).c_str());
		characterModel.scale(0.01f, 0.01f, 0.01f);

	}

	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
	shader->setUniform("u_time", time);
	shader->setUniform("u_tex_tiling", tiling);
	shader->setUniform("u_texture", characterTex, 0);
	characterModel.rotate(180 * DEG2RAD, Vector3(0, 1, 0));
	shader->setUniform("u_model", characterModel);
	if (animations.size() != 0) {
		for (size_t i = 0; i < animations.size(); i++)
		{
			animations[i]->assignTime(time);
			//animations[i]->skeleton.renderSkeleton(cam, model);
			characterMesh->renderAnimated(GL_TRIANGLES, &animations[i]->skeleton);
			characterModel.scale(100.0f, 100.0f, 100.0f);
		}
	}




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
		if (!Input::isKeyPressed(SDL_SCANCODE_A)) {
			if (angle2 < 0) {
				angle2 += rotSpeed * 0.7;
			}
		}
		if (!Input::isKeyPressed(SDL_SCANCODE_D)) {
			if (angle2 > 0) {
				angle2 -= rotSpeed *0.7;
			}
		}
		
		
		

        
		Matrix44 dragonRotation;
		dragonRotation.rotate(angle * DEG2RAD, Vector3(0, 1, 0));

		Vector3 forward = dragonRotation.rotateVector(Vector3(0, 0, 1));
        Vector3 v = dragonRotation.rotateVector(Vector3(0, 1, 0));

		Vector3 dragonVel;
		if (g->world->mission2) {
			if (Input::wasKeyPressed(SDL_SCANCODE_LEFT)) {
				std::string s;
				std::string PATH = "data/";
				Audio::Play((PATH + s.assign("sounds/disparo.wav")).c_str(), NULL);
				Vector3 bulletVelocity = Vector3(0.0f, 0.0f, 0.0f);
				bulletVelocity = bulletVelocity - (forward * 10000.01f * dt);
				Matrix44 bulletModel;
				Vector3 currentPosdrag = model.getTranslation();
				bulletModel.setTranslation(currentPosdrag.x, currentPosdrag.y+2, currentPosdrag.z);
				if (spawnBullet(bulletModel, Vector3(), bulletVelocity, 100)) {
					std::cout << "bullet spawned" << std::endl;
					

				}

			}
		}
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
		if (!Input::isKeyPressed(SDL_SCANCODE_UP)) {
			if (angle3 < 0) {
				angle3 += rotSpeed * 0.6;
			}
		}
		if (!Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
			if (angle3 > 0) {
				angle3 -= rotSpeed * 0.6;
			}
		}
        
		nexPos = getPosition() + dragonVel;
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
		EntityMesh* dragon = g->world->staticEntitiesDragons[g->world->currentDragon];
		g->world->mainCharacter->model.setTranslation(current.x - 7 , 0, current.z );
		dragon->model.setTranslation(current.x , 0, current.z );
        model.rotate(angle * DEG2RAD, Vector3(0, 1, 0));
        model.rotate(angle2 * DEG2RAD, Vector3(0, 0, 1));
		model.rotate(angle3 * DEG2RAD, Vector3(1, 0, 0));

	}
}

bool EntityCharacterDragon::checkInsidePlane()
{
	Vector3 position = getPosition();
	if (position.x < 100 && position.x > -100 && position.z < 100 && position.z > -100) return true;
	return false;
}
