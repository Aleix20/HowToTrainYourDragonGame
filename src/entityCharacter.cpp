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
        float playerMovement = 50.0f * dt;
        float rotSpeed = 90.0f * DEG2RAD * dt;

        if (Input::isKeyPressed(SDL_SCANCODE_W)) model.translate(0.0f, 0.0f, -playerMovement);
        if (Input::isKeyPressed(SDL_SCANCODE_S)) model.translate(0.0f, 0.0f, playerMovement);
        if (Input::isKeyPressed(SDL_SCANCODE_A)) model.rotate(-rotSpeed, Vector3(0.0f, 1.0f, 0.0f));
        if (Input::isKeyPressed(SDL_SCANCODE_D)) model.rotate(rotSpeed, Vector3(0.0f, 1.0f, 0.0f));
        
    }
    
}

