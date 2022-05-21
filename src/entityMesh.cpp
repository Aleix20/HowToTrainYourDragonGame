//
//  entityMesh.cpp
//  TJE_XCODE
//
//  Created by Alèxia  Lozano Pedrajas on 12/5/22.
//  Copyright © 2022 Alèxia  Lozano Pedrajas. All rights reserved.
//

#include <stdio.h>
#include "entityMesh.h"
#include "shader.h"
EntityMesh::EntityMesh()
{
}
EntityMesh::EntityMesh(Mesh* mesh, Texture* texture,  Matrix44 model) {
    this->model = model;
    this->name = "Toothless";
    this->texture = texture;
    this->mesh = mesh;
}

void EntityMesh::render()
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

    //disable shader
    shader->disable();
    
    if (!Game::instance->cameraLocked) {
        mesh->renderBounding(model);
    }

}

void EntityMesh::update(float dt)
{}
