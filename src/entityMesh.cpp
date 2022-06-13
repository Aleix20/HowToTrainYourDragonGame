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
EntityMesh::EntityMesh(Mesh* mesh, Texture* texture, Matrix44 model) {
	this->model = model;
	this->name = "Toothless";
	this->texture = texture;
	this->mesh = mesh;
}

void EntityMesh::render()
{
	float time = Game::instance->time;
	std::string a;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	std::string PATH1 = "data/";
#else
	std::string PATH1 = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";
#endif
	Shader* shader;
	if (animations.size() != 0) {
		shader = Shader::Get((PATH1 + a.assign("shaders/skinning.vs")).c_str(), (PATH1 + a.assign("shaders/texture.fs")).c_str());
		model.scale(0.01f, 0.01f, 0.01f);

	}
	else {

		shader = Game::instance->shader;
	}
	if (!shader) return;


	//enable shader
	shader->enable();

	//upload uniforms
	Camera* cam = Game::instance->camera;
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", cam->viewprojection_matrix);
	shader->setUniform("u_texture", texture, 0);
	shader->setUniform("u_time", time);
	//ROTAR MODELOS CON PROBLEMAS APP JAVI
	if (name.compare("Mission3")==0) {
		model.rotate(-90* DEG2RAD, Vector3(1,0,0));
	}
	shader->setUniform("u_model", model);
	shader->setUniform("u_tex_tiling", tiling);
	if (animations.size() != 0) {
		for (size_t i = 0; i < animations.size(); i++)
		{
			animations[i]->assignTime(time);
			//animations[i]->skeleton.renderSkeleton(cam, model);
			mesh->renderAnimated(GL_TRIANGLES, &animations[i]->skeleton);
			model.scale(100.0f, 100.0f, 100.0f);
		}
	}

	else {
		mesh->render(GL_TRIANGLES);

	}
	//ROTAR MODELOS CON PROBLEMAS APP JAVI
	if (name.compare("Mission3")==0) {
		model.rotate(90 * DEG2RAD, Vector3(1, 0, 0));
	}
	//disable shader
	shader->disable();

	if (!Game::instance->cameraLocked) {
		mesh->renderBounding(model);
	}

}

void EntityMesh::update(float dt)
{}
