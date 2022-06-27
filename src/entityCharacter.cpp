//
//  entityCharacter.cpp
//  TJE_XCODE
//
//  Created by Alèxia  Lozano Pedrajas on 12/5/22.
//  Copyright © 2022 Alèxia  Lozano Pedrajas. All rights reserved.
//

#include <stdio.h>
#include "input.h"
#include "entityCharacter.h"
#include "utils.h"

EntityCharacter::EntityCharacter()
{
}
EntityCharacter::EntityCharacter(Mesh* mesh, Texture* texture, Matrix44 model) {
	this->model = model;
	this->texture = texture;
	this->mesh = mesh;
}

void EntityCharacter::render()
{


	Game* g = Game::instance;
	float time = Game::instance->time;
	std::string a;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	std::string PATH1 = "data/";
#else
	std::string PATH1 = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";
#endif
	Shader* shader = Shader::Get((PATH1 + a.assign("shaders/skinning.vs")).c_str(), (PATH1 + a.assign("shaders/texture.fs")).c_str());
	model.scale(0.01f, 0.01f, 0.01f);
	if (!shader) return;

	
	Skeleton resultSk;
	Animation* idle = animations[0];
	Animation* walk = animations[1];
	Animation* run = animations[2];
	Animation* dance = animations[3];
	Animation* pet = animations[4];

	float t = fmod(time, walk->duration) / walk->duration;
	walk->assignTime(t* walk->duration);
	run->assignTime(t * run->duration);
	idle->assignTime(time);
	dance->assignTime(time);
	pet->assignTime(time);
	float velFactor = playerVel.length() * 0.1f;
	if (velFactor > 1.0f) {
		blendSkeleton(&walk->skeleton, &run->skeleton, velFactor, &resultSk);

	} else if(velFactor < 1.0f) {

		blendSkeleton(&idle->skeleton, &walk->skeleton, velFactor, &resultSk);
	}
	if (velFactor <=0.01 && g->world->channelAudios !=0) {
		resultSk = dance->skeleton;
	}
	if (g->world->pet) {
		resultSk = pet->skeleton;
	}
	//enable shader
	shader->enable();

	//upload uniforms
	Camera* cam = Game::instance->camera;
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
	shader->setUniform("u_texture", texture, 0);
	shader->setUniform("u_time", time);
	//ROTAR MODELOS CON PROBLEMAS APP JAVI
	if (name.compare("Mission3") == 0) {
		model.rotate(-90 * DEG2RAD, Vector3(1, 0, 0));
	}
	Matrix44 visualModel = model;
	visualModel.rotate(180 * DEG2RAD, Vector3(0,1,0));
	shader->setUniform("u_model", visualModel);
	shader->setUniform("u_tex_tiling", tiling);
	

		
	mesh->renderAnimated(GL_TRIANGLES, &resultSk);
	model.scale(100.0f, 100.0f, 100.0f);





	//disable shader
	shader->disable();

}


void EntityCharacter::update(float dt)
{

	Game* g = Game::instance;
	bool cameraLocked = g->cameraLocked;
	Camera* cam = g->camera;
	if (cameraLocked) {
		float playerSpeed = 100.0f * dt;
		float rotSpeed = 120.0f * dt;
		float drag = 10.0f * dt;
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) playerSpeed *= 2;
		if (Input::isKeyPressed(SDL_SCANCODE_A)) angle -= rotSpeed;
		if (Input::isKeyPressed(SDL_SCANCODE_D)) angle += rotSpeed;

		Matrix44 playerRotation;
		playerRotation.rotate(angle * DEG2RAD, Vector3(0, 1, 0));

		Vector3 forward = playerRotation.rotateVector(Vector3(0, 0, 1));
		Vector3 right = playerRotation.rotateVector(Vector3(1, 0, 0));



		if (Input::isKeyPressed(SDL_SCANCODE_W)) playerVel = playerVel - (forward * playerSpeed);
		if (Input::isKeyPressed(SDL_SCANCODE_S)) playerVel = playerVel + (forward * playerSpeed);

		Vector3 nexPos = getPosition() + (playerVel * dt);
		Vector3 character_center = nexPos + Vector3(0, 2, 0);

		std::vector<EntityMesh*> entities = g->world->staticEntities;
		Vector3 current = getPosition();
		checkCollisionEntities(entities, character_center, dt, nexPos, current);
		std::vector<EntityMesh*> entities_Ch = g->world->staticEntitiesCharacter;
		checkCollisionEntities(entities_Ch, character_center, dt, nexPos, current);

		checkCollisionEntity(g->world->staticEntitiesDragons[g->world->currentDragon], character_center, dt, nexPos, current);
		playerVel = playerVel - (playerVel * drag);

		model.setTranslation(nexPos.x, nexPos.y, nexPos.z);
		model.rotate(angle * DEG2RAD, Vector3(0, 1, 0));

	}
}


