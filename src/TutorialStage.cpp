#include "TutorialStage.h"
TutorialStage::TutorialStage() {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	std::string PATH1 = "data/";
#else
	std::string PATH1 = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";
#endif
	quadTutorial = new Mesh();
	std::string a;
	tutorialImages.push_back(Texture::Get((PATH1 + a.assign("Tutorial/tutorial1.png")).c_str()));
	tutorialImages.push_back(Texture::Get((PATH1 + a.assign("Tutorial/tutorial2.png")).c_str()));
	tutorialImages.push_back(Texture::Get((PATH1 + a.assign("Tutorial/tutorial3.png")).c_str()));
	tutorialImages.push_back(Texture::Get((PATH1 + a.assign("Tutorial/tutorial4.png")).c_str()));
	tutorialImages.push_back(Texture::Get((PATH1 + a.assign("Tutorial/tutorial5.png")).c_str()));
	tutorialImages.push_back(Texture::Get((PATH1 + a.assign("Tutorial/tutorial6.png")).c_str()));
	tutorialImages.push_back(Texture::Get((PATH1 + a.assign("Tutorial/tutorial7.png")).c_str()));
}
void TutorialStage::render(void)
{
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

	quadTutorial->createQuad(Game::instance->window_width * 0.5, Game::instance->window_height * 0.5, Game::instance->window_width, Game::instance->window_height, false);
	RenderGUI(*quadTutorial, tutorialImages[currentTutorial], Vector4(1, 1, 1, 1));
	Texture* leftArrow = Texture::Get((PATH1 + a.assign("Tutorial/plain-arrow2.png")).c_str());
	Texture* rightArrow = Texture::Get((PATH1 + a.assign("Tutorial/plain-arrow.png")).c_str());
	World* world = Game::instance->world;
	if (currentTutorial != 0) {
		if (RenderButton(500, 550, 100, 100, leftArrow, wasLeftPressed)) {
			currentTutorial--;

		}
	}
	if (RenderButton(660, 550, 100, 100, rightArrow, wasLeftPressed)) {
		currentTutorial++;
		if (currentTutorial == 7) {
			world->playStage = true;
		}
	}




	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	wasLeftPressed = false;

	if (currentTutorial == 2){
		EntityMesh* Astrid = new EntityMesh();
		Astrid->texture = Texture::Get((PATH1 + a.assign("Astrid/Astrid.png")).c_str());
		Astrid->mesh = Mesh::Get((PATH1 + a.assign("Astrid/Astrid.mesh")).c_str());
		Astrid->animations.push_back(Animation::Get((PATH1 + a.assign("Astrid/Astrid.skanim")).c_str()));
		Astrid->model = Matrix44();
		Astrid->model.setTranslation(45, 0, 50);
		Astrid->model.scale(35, 35, 35);
		Astrid->model.rotate(40 * DEG2RAD, Vector3(1, 0, 0));
		Astrid->model.rotate(35 * DEG2RAD, Vector3(0, 1, 0));
		Astrid->render();

	}
	if (currentTutorial == 3) {
		EntityMesh* Fishlegs = new EntityMesh();
		Fishlegs->texture = Texture::Get((PATH1 + a.assign("Fishlegs/Fishlegs.png")).c_str());
		Fishlegs->mesh = Mesh::Get((PATH1 + a.assign("Fishlegs/Fishlegs.mesh")).c_str());
		Fishlegs->animations.push_back(Animation::Get((PATH1 + a.assign("Fishlegs/Fishlegs.skanim")).c_str()));
		Fishlegs->model = Matrix44();
		Fishlegs->model.setTranslation(45, 0, 50);
		Fishlegs->model.scale(35, 35, 35);
		Fishlegs->model.rotate(40 * DEG2RAD, Vector3(1, 0, 0));
		Fishlegs->model.rotate(-30 * DEG2RAD, Vector3(0, 1, 0));
		Fishlegs->render();
	}
	if (currentTutorial == 4) {
		EntityMesh* Snotlout = new EntityMesh();
		Snotlout->texture = Texture::Get((PATH1 + a.assign("Snotlout/Snotlout.png")).c_str());
		Snotlout->mesh = Mesh::Get((PATH1 + a.assign("Snotlout/Snotlout.mesh")).c_str());
		Snotlout->animations.push_back(Animation::Get((PATH1 + a.assign("Snotlout/Snotlout.skanim")).c_str()));
		Snotlout->model = Matrix44();
		Snotlout->model.setTranslation(45, 0, 50);
		Snotlout->model.scale(35, 35, 35);
		Snotlout->model.rotate(40 * DEG2RAD, Vector3(1, 0, 0));
		Snotlout->model.rotate(35 * DEG2RAD, Vector3(0, 1, 0));
		Snotlout->render();

	}
	if (currentTutorial == 5) {
		EntityMesh* Hiccup = new EntityMesh();
		Hiccup->texture = Texture::Get((PATH1 + a.assign("Hiccup/HiccupTeen.png")).c_str());
		Hiccup->mesh = Mesh::Get((PATH1 + a.assign("Hiccup/HiccupIntro.mesh")).c_str());
		Hiccup->animations.push_back(Animation::Get((PATH1 + a.assign("Hiccup/HiccupWalking.skanim")).c_str()));
		Hiccup->model = Matrix44();
		Hiccup->model.setTranslation(-40, 30, 50);
		Hiccup->model.scale(20, 20, 20);
		Hiccup->model.rotate(40 * DEG2RAD, Vector3(1, 0, 0));
		Hiccup->model.rotate(-35 * DEG2RAD, Vector3(0, 1, 0));
		Hiccup->render();

		EntityMesh* Toothless = new EntityMesh();
		Toothless->texture = Texture::Get((PATH1 + a.assign("NightFury/Toothless.png")).c_str());
		Toothless->mesh = Mesh::Get((PATH1 + a.assign("NightFury/Toothless.obj")).c_str());
		Toothless->model = Matrix44();
		Toothless->model.setTranslation(50, 20, 30);
		Toothless->model.scale(8, 8, 8);
		Toothless->model.rotate(40 * DEG2RAD, Vector3(1, 0, 0));
		Toothless->model.rotate(220 * DEG2RAD, Vector3(0, 1, 0));
		Toothless->render();
	}
	if (currentTutorial == 6) {
		EntityMesh* Stoick = new EntityMesh();
		Stoick->texture = Texture::Get((PATH1 + a.assign("Stoick/Stoick.png")).c_str());
		Stoick->mesh = Mesh::Get((PATH1 + a.assign("Stoick/Stoick.mesh")).c_str());
		Stoick->animations.push_back(Animation::Get((PATH1 + a.assign("Stoick/Stoick.skanim")).c_str()));
		Stoick->model = Matrix44();
		Stoick->model.setTranslation(-40, 0, 50);
		Stoick->model.scale(20, 20, 20);
		Stoick->model.rotate(40 * DEG2RAD, Vector3(1, 0, 0));
		Stoick->model.rotate(-35 * DEG2RAD, Vector3(0, 1, 0));
		Stoick->render();

		EntityMesh* Toothless = new EntityMesh();
		Toothless->texture = Texture::Get((PATH1 + a.assign("NightFury/Toothless.png")).c_str());
		Toothless->mesh = Mesh::Get((PATH1 + a.assign("NightFury/Toothless.obj")).c_str());
		Toothless->model = Matrix44();
		Toothless->model.setTranslation(50, 30, 30);
		Toothless->model.scale(6, 6, 6);
		Toothless->model.rotate(40 * DEG2RAD, Vector3(1, 0, 0));
		Toothless->model.rotate(220 * DEG2RAD, Vector3(0, 1, 0));
		Toothless->render();

		EntityMesh* Nadder = new EntityMesh();
		Nadder->texture = Texture::Get((PATH1 + a.assign("Nadder/Nadder.png")).c_str());
		Nadder->mesh = Mesh::Get((PATH1 + a.assign("Nadder/Nadder.obj")).c_str());
		Nadder->model = Matrix44();
		Nadder->model.setTranslation(50, 0, 30);
		Nadder->model.scale(6, 6, 6);
		Nadder->model.rotate(40 * DEG2RAD, Vector3(1, 0, 0));
		Nadder->model.rotate(220 * DEG2RAD, Vector3(0, 1, 0));
		Nadder->render();

		EntityMesh* LightFury = new EntityMesh();
		LightFury->texture = Texture::Get((PATH1 + a.assign("LightFury/LightFury.png")).c_str());
		LightFury->mesh = Mesh::Get((PATH1 + a.assign("LightFury/LightFury.obj")).c_str());
		LightFury->model = Matrix44();
		LightFury->model.setTranslation(70, -40, 30);
		LightFury->model.scale(6, 6, 6);
		LightFury->model.rotate(40 * DEG2RAD, Vector3(1, 0, 0));
		LightFury->model.rotate(220 * DEG2RAD, Vector3(0, 1, 0));
		LightFury->render();

	}
	

	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
	SDL_GL_SwapWindow(window);
}
void TutorialStage::update(double seconds_elapsed)
{
}
void TutorialStage::onKeyDown(SDL_KeyboardEvent event)
{
}
void TutorialStage::onMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {
		wasLeftPressed = true;
	}
}
;
