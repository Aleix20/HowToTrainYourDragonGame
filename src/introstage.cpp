//
//  introstage.cpp
//  TJE
//
//  Created by Alèxia  Lozano Pedrajas on 19/4/22.
//


#include "introstage.h"


IntroStage::IntroStage() {




};
void IntroStage::render() {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	std::string PATH1 = "data/";
#else
	std::string PATH1 = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";
#endif
	std::string a;
	Camera* camera = Game::instance->camera;
	bool cameraLocked = Game::instance->cameraLocked;
	SDL_Window* window = Game::instance->window;
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//drawGrid();

	//GUI
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Texture* tex = Texture::Get((PATH1 + a.assign("fondo.png")).c_str());
	Mesh quad;
	quad.createQuad(Game::instance->window_width*0.5, Game::instance->window_height * 0.5 , Game::instance->window_width, Game::instance->window_height, false);
	RenderGUI(quad, tex, Vector4(1,1,1,1));
	tex = Texture::Get((PATH1 + a.assign("dragonIcon.png")).c_str());
	if (RenderButton(200, 140, 312, 143, tex, wasLeftPressed)) {
		Game::instance->world->playStage = true;

	}
	tex = Texture::Get((PATH1 + a.assign("tutorial.png")).c_str());
	if (RenderButton(200, 290, 312, 143, tex, wasLeftPressed)) {
		Game::instance->world->tutorialStage = true;
	}
	tex = Texture::Get((PATH1 + a.assign("exitDoor.png")).c_str());
	if (RenderButton(200, 440, 312, 143, tex, wasLeftPressed)) {
		Game::instance->must_exit = true;
	}
	


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	wasLeftPressed = false;
    EntityMesh* Hiccup = new EntityMesh();
    Hiccup->texture = Texture::Get((PATH1 + a.assign("Hiccup/HiccupTeen.png")).c_str());
    Hiccup->mesh = Mesh::Get((PATH1 + a.assign("Hiccup/HiccupIntro2.mesh")).c_str());
    Hiccup->animations.push_back(Animation::Get((PATH1 + a.assign("Hiccup/HiccupIntro2.skanim")).c_str()));
    Hiccup->model = Matrix44();
    Hiccup->model.setTranslation(45, 0, 50);
    Hiccup->model.scale(35,35,35);
    Hiccup->model.rotate(40*DEG2RAD, Vector3(1,0,0));
    Hiccup->model.rotate(35*DEG2RAD, Vector3(0,1,0));
    Hiccup->render();
    
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
	SDL_GL_SwapWindow(window);
};

void IntroStage::update(double seconds_elapsed) {

};

void IntroStage::onKeyDown(SDL_KeyboardEvent event) {
	World* world = Game::instance->world;
	switch (event.keysym.sym)
	{
	case SDLK_0:
		world->playStage = !world->playStage;
		break;
	}
}

void IntroStage::onMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {
		wasLeftPressed = true;
	}
}


;
