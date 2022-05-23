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
	Shader* shader = g->shader;
	if (!shader) return;


	//enable shader
	shader->enable();

	//upload uniforms
	Camera* cam = g->camera;
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
	shader->setUniform("u_texture", texture, 0);
	shader->setUniform("u_time", time);
	shader->setUniform("u_model", model);
	shader->setUniform("u_tex_tiling", tiling);


	mesh->render(GL_TRIANGLES);

	//disable shader
	shader->disable();

}


void EntityCharacter::update(float dt)
{

	Game* g = Game::instance;
	bool cameraLocked = g->cameraLocked;
	Camera* cam = g->camera;
	if (cameraLocked) {
		float playerSpeed = 50.0f * dt;
		float rotSpeed = 120.0f * dt;



		if (Input::isKeyPressed(SDL_SCANCODE_A)) angle += rotSpeed; 
		if (Input::isKeyPressed(SDL_SCANCODE_D)) angle -= rotSpeed; 

		Matrix44 playerRotation;
		playerRotation.rotate(angle * DEG2RAD, Vector3(0, 1, 0));

		Vector3 forward = playerRotation.rotateVector(Vector3(0, 0, 1));
		Vector3 right = playerRotation.rotateVector(Vector3(1, 0, 0));

		Vector3 playerVel;

		if (Input::isKeyPressed(SDL_SCANCODE_W)) playerVel = playerVel - (forward * playerSpeed);
		if (Input::isKeyPressed(SDL_SCANCODE_S)) playerVel = playerVel + (forward * playerSpeed);

		Vector3 nexPos = getPosition() + playerVel;
		Vector3 character_center = nexPos + Vector3(0, 2, 0);

		std::vector<EntityMesh*> entities = g->world->staticEntities;

		for (size_t i = 0; i < entities.size(); i++) {
			EntityMesh* currentEntity = entities[i];
			Vector3 coll;
			Vector3 collnorm;
			//comprobamos si colisiona el objeto con la esfera (radio 3)
			if (!currentEntity->mesh->testSphereCollision(currentEntity->model, character_center, 0.5f, coll, collnorm))
				continue; //si no colisiona, pasamos al siguiente objeto

			//si la esfera está colisionando muevela a su posicion anterior alejandola del objeto
			Vector3 push_away = normalize(coll - character_center) * dt;
			nexPos = getPosition() - push_away; //move to previous pos but a little bit further

			//cuidado con la Y, si nuestro juego es 2D la ponemos a 0
			nexPos.y = 0;

		}
		model.setTranslation(nexPos.x, nexPos.y, nexPos.z);
		model.rotate(angle * DEG2RAD, Vector3(0, 1, 0));

		

	}
}

