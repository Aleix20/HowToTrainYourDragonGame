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


	bool cameraLocked = Game::instance->cameraLocked;
	Camera* cam = Game::instance->camera;

	if (cameraLocked) {
		float playerSpeed = 50.0f * dt;
		float rotSpeed = 90.0f * DEG2RAD * dt;
        Game* g = Game::instance;
        
        Vector3 playerRotation;
        Vector3 playerMovement;
        if (Input::isKeyPressed(SDL_SCANCODE_A)) {
            playerRotation = Vector3(0.0f, -1.0f, 0.0f);
            //model.rotate(-rotSpeed, Vector3(0.0f, 1.0f, 0.0f));
        }
        if (Input::isKeyPressed(SDL_SCANCODE_D)){
            playerRotation = Vector3(0.0f, 1.0f, 0.0f);
            //model.rotate(rotSpeed, Vector3(0.0f, 1.0f, 0.0f));
            
        } else {
            playerRotation = Vector3(0.0f,0.0f,0.0f);
        }
		Vector3 forward = Vector3(0,0,-1);
        if (Input::isKeyPressed(SDL_SCANCODE_W)) {
                //Calculem quin seria el moviment del personatge
            playerMovement = (forward * playerSpeed);
                //model.translate(0.0f, 0.0f, -playerSpeed);
        }
        if (Input::isKeyPressed(SDL_SCANCODE_S)) {
            playerMovement = (forward * (-playerSpeed));
                //model.translate(0.0f, 0.0f, playerSpeed);
        }
        
            //calculamos el centro de la esfera de colisión del player elevandola hasta la cintura
        Vector3 nexPos = getPosition() + playerMovement;
        Vector3 character_center = nexPos + Vector3(0, 2, 0);
            
        
        std::vector<EntityMesh*> entities = g->world->staticEntities;
        for (size_t i = 0; i < entities.size(); i++)
        {
            EntityMesh* currentEntity = entities[i];
            Vector3 coll;
            Vector3 collnorm;
            //comprobamos si colisiona el objeto con la esfera (radio 3)
            if (!currentEntity->mesh->testSphereCollision(currentEntity->model, character_center, 0.5f, coll, collnorm))
                    
                model.setTranslation(nexPos.x, nexPos.y, nexPos.z);
                continue; //si no colisiona, pasamos al siguiente objeto

            //si la esfera está colisionando muevela a su posicion anterior alejandola del objeto
            //nexPos = getPosition() - push_away; //move to previous pos but a little bit further
            //nexPos.y = 0;

            }
        if (playerRotation.x != 0.0f && playerRotation.y != 0.0f && playerRotation.z != 0.0f ) model.rotate(rotSpeed, playerRotation);
        
        }

}

