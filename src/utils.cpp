#include "utils.h"

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif


#include "game.h"
#include "shader.h"
#include "mesh.h"
#include "extra/stb_easy_font.h"
#include <sstream>
#include <string>
#include <iostream>
#include "input.h"

#pragma region EDITORMUNDO
void AddEntityInFront(Camera* cam, EntityMesh* entity, std::vector<EntityMesh*>& entities) {

	if (entity == NULL) return;
	std::string a;
	Vector2 mouse = Input::mouse_position;
	Game* g = Game::instance;

	Vector3 dir = cam->getRayDirection(mouse.x, mouse.y, g->window_width, g->window_height);
	Vector3 rayOrigin = cam->eye;

	Vector3 spawnPos = RayPlaneCollision(Vector3(), Vector3(0, 1, 0), rayOrigin, dir);
	Matrix44 model;
	model.translate(spawnPos.x, spawnPos.y, spawnPos.z);
	EntityMesh* newEntity = new EntityMesh();
	newEntity->mesh = entity->mesh;
	newEntity->texture = entity->texture;
	newEntity->model = model;


	entities.push_back(newEntity);
}
void RayPickCheck(Camera* cam, std::vector<EntityMesh*> entities) {
	//esto exactamente no se lo que hace
	Vector2 mouse = Input::mouse_position;
	Game* g = Game::instance;
	Vector3 dir = cam->getRayDirection(mouse.x, mouse.y, g->window_width, g->window_height);
	Vector3 rayOrigin = cam->eye;
	float distance = 1000.0f;
	for (size_t i = 0; i < entities.size(); i++) {
		EntityMesh* entity = entities[i];
		Vector3 pos;
		Vector3 normal;

		if (entity->mesh->testRayCollision(entity->model, rayOrigin, dir, pos, normal)) {
			std::cout << "selected" << std::endl;
			float distanceCamtoObj = cam->eye.distance(pos);
			if (distanceCamtoObj < distance) {
				distance = distanceCamtoObj;
				g->world->selectedEntity = entity;
			}

		}
	};

};

void RotateSelected(float angleDegrees, Vector3 rotationVector) {
	Game* g = Game::instance;
	if (g->world->selectedEntity == NULL) {
		return;
	}
	g->world->selectedEntity->model.rotate(angleDegrees * DEG2RAD, rotationVector);
}
void MoveSelected(float x, float y, float z) {
	Game* g = Game::instance;
	if (g->world->selectedEntity == NULL) {
		return;
	}
	g->world->selectedEntity->model.translate(x, y, z);

}
void ScaleSelected(float x, float y, float z) {
	Game* g = Game::instance;
	if (g->world->selectedEntity == NULL) {
		return;
	}
	g->world->selectedEntity->model.scale(x, y, z);

}

void RemoveSelected(std::vector<EntityMesh*>& entities, Entity* selectedEntity)
{
	if (selectedEntity == NULL) {
		return;
	}
	std::vector<EntityMesh*>::iterator it = std::find(entities.begin(), entities.end(), selectedEntity);
	entities.erase(it);
	selectedEntity = NULL;
}


#pragma endregion

#pragma region FRUSTRUM
void checkFrustrumStatic(std::vector<EntityMesh*>& entities, Vector3& camPos)
{
	Game* g = Game::instance;
	for (int i = 0; i < entities.size(); i++) {
		Vector3 entityPos = entities[i]->model.getTranslation();
		Mesh* entityMesh = entities[i]->mesh;
		float dist = entityPos.distance(camPos);
		if (dist > g->noRenderDistance) {
			continue;
		}
		if (!g->camera->testSphereInFrustum(entityPos, entityMesh->radius)) {
			continue;
		}
		entities[i]->render();
	}
}
void checkFrustrumEntity(EntityMesh*& entity, Vector3& camPos)
{
	Game* g = Game::instance;

	Vector3 entityPos = entity->model.getTranslation();
	Mesh* entityMesh = entity->mesh;
	float dist = entityPos.distance(camPos);
	if (dist > g->noRenderDistance) {
		return;
	}
	if (!g->camera->testSphereInFrustum(entityPos, entityMesh->radius)) {
		return;
	}
	entity->render();

}
#pragma endregion
void checkCollisionEntities(std::vector<EntityMesh*>& entitiesCollision, Vector3& character_center, float dt, Vector3& nexPos, Vector3& currentPos)
{
	World* world = Game::instance->world;
	for (size_t i = 0; i < entitiesCollision.size(); i++) {
		EntityMesh* currentEntity = entitiesCollision[i];
		Vector3 coll;
		Vector3 collnorm;
		//comprobamos si colisiona el objeto con la esfera (radio 3)
		if (!currentEntity->mesh->testSphereCollision(currentEntity->model, character_center, 0.5f, coll, collnorm))
			continue; //si no colisiona, pasamos al siguiente objeto

		//si la esfera está colisionando muevela a su posicion anterior alejandola del objeto
		Vector3 push_away = normalize(coll - character_center) * dt;
		nexPos = currentPos - push_away; //move to previous pos but a little bit further
		//cuidado con la Y, si nuestro juego es 2D la ponemos a 0
		if (!world->topOfDragon) {
			nexPos.y = 0;
		}

	}
}
void checkCollisionEntity(EntityMesh* entity, Vector3& character_center, float dt, Vector3& nexPos, Vector3& currentPos)
{
	World* world = Game::instance->world;
	EntityMesh* currentEntity = entity;
	Vector3 coll;
	Vector3 collnorm;
	//comprobamos si colisiona el objeto con la esfera (radio 3)
	if (!currentEntity->mesh->testSphereCollision(currentEntity->model, character_center, 0.5f, coll, collnorm))
		return;

	//si la esfera está colisionando muevela a su posicion anterior alejandola del objeto
	Vector3 push_away = normalize(coll - character_center) * dt;
	nexPos = currentPos - push_away; //move to previous pos but a little bit further
	//cuidado con la Y, si nuestro juego es 2D la ponemos a 0
	if (!world->topOfDragon) {
		nexPos.y = 0;
	}


}




void checkGameState()
{
	World* world = Game::instance->world;
	for (int i = 0; i < world->staticEntitiesCharacter.size(); i++) {
		EntityMesh* currentCharacter = world->staticEntitiesCharacter[i];
		Vector3 currentCharacterPosition = currentCharacter->getPosition();
		if (currentCharacterPosition.distance(world->mainCharacter->getPosition()) < 3.0f) {
			if (currentCharacter->name.compare("ChangeDragon") == 0) {
				if (world->currentDragon == (world->staticEntitiesDragons.size() - 1)) {
					world->currentDragon = 0;
				}
				else {
					world->currentDragon++;
				}
			}
			if (currentCharacter->name.compare("Mission1") == 0) {
				world->mission1 = true;
				world->mission1End = true;
				world->missionTime = 120.0f;
			}
			if (currentCharacter->name.compare("Mission2") == 0) {
				world->mission2 = true;
				world->mission2End = true;
				world->missionTime = 120.0f;
			}
		}

	}
}
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
void checkAudios()
{

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	std::string PATH1 = "data/";
#else
	std::string PATH1 = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";
#endif
	std::string s;
	World* world = Game::instance->world;
	
	if (!world->audioTimer) {
		for (int i = 0; i < world->staticEntitiesCharacter.size(); i++) {
			EntityMesh* currentCharacter = world->staticEntitiesCharacter[i];
			Vector3 currentCharacterPosition = currentCharacter->getPosition();
			if (currentCharacterPosition.distance(world->mainCharacter->getPosition()) < 10.0f) {

				if (currentCharacter->name.compare("Mission5") == 0) {
					world->audioTimer = true;
					world->timerAudio = 15;
					int randNum = rand() % 3;
					Audio::Play((PATH1 + s.assign(currentCharacter->audios[randNum])).c_str(), NULL);
				}
				if (currentCharacter->name.compare("Mission2") == 0) {
					world->audioTimer = true;
					world->timerAudio = 15;
					int randNum = rand() % 3;
					Audio::Play((PATH1 + s.assign(currentCharacter->audios[randNum])).c_str(), NULL);
				}
				if (currentCharacter->name.compare("Mission1") == 0) {
					world->audioTimer = true;
					world->timerAudio = 15;
					int randNum = rand() % 4;
					Audio::Play((PATH1 + s.assign(currentCharacter->audios[randNum])).c_str(), NULL);
				}
				if (currentCharacter->name.compare("Mission3") == 0) {
					world->audioTimer = true;
					world->timerAudio = 10;
					int randNum = rand() % 3;
					Audio::Play((PATH1 + s.assign(currentCharacter->audios[randNum])).c_str(), NULL);
				}
				if (currentCharacter->name.compare("ChangeDragon") == 0) {
					world->audioTimer = true;
					world->timerAudio = 10;
					int randNum = rand() % 3;
					Audio::Play((PATH1 + s.assign(currentCharacter->audios[randNum])).c_str(), NULL);
				}
				if (currentCharacter->name.compare("Dance") == 0) {
					world->audioTimer = false;
					world->timerAudio = 0;
					int randNum = 0;
					if (world->channelAudios == 0) {
						world->channelAudios = Audio::Play((PATH1 + s.assign(currentCharacter->audios[randNum])).c_str(), BASS_SAMPLE_LOOP);
					}
					
				}
				
			}
			else {
				if (world->channelAudios > 0 && currentCharacter->name.compare("Dance") == 0) {
					Audio::Stop(world->channelAudios);
					world->audioTimer = false;
					world->channelAudios = 0;
				}
			}

		}
	}
	
}
#endif
void setUpCamera(Matrix44& model, Vector3 eyeVec, Vector3 centerVec, Vector3 upVec, Camera* camera)
{
	Vector3 eye = model * eyeVec;
	Vector3 center = model * centerVec;
	Vector3 up = model.rotateVector(upVec);
	camera->enable();
	camera->lookAt(eye, center, up);
}
long getTime()
{
#ifdef WIN32
	return GetTickCount();
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (int)(tv.tv_sec * 1000 + (tv.tv_usec / 1000));
#endif
}

bool RenderButton(float x, float y, float w, float h, Texture* tex, bool wasLeftPressed)
{
	Vector2 mouse = Input::mouse_position;
	float halfWidth = w * 0.5;
	float halfHeight = h * 0.5;
	float min_x = x - halfWidth;
	float max_x = x + halfWidth;
	float min_y = y - halfHeight;
	float max_y = y + halfHeight;

	bool hover = mouse.x >= min_x && mouse.x <= max_x && mouse.y >= min_y && mouse.y <= max_y;
	Vector4 buttonColor = hover ? Vector4(1, 1, 1, 1) : Vector4(1, 1, 1, 0.7f);
	Mesh quad;
	quad.createQuad(x, y, w, h, false);
	RenderGUI(quad, tex, buttonColor);
	return wasLeftPressed && hover;
}
void RenderGUI(Mesh quad, Texture* tex, Vector4 color = Vector4(1, 1, 1, 1))
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	std::string PATH1 = "data/";
#else
	std::string PATH1 = "/Users/alexialozano/Documents/GitHub/JocsElectronicsClasse/data/";
#endif
	std::string a;
	int windowWidth = Game::instance->window_width;
	int windowHeight = Game::instance->window_height;



	Camera cam2D;
	cam2D.setOrthographic(0, windowWidth, windowHeight, 0, -1, 1);

	Shader* shader = Shader::Get((PATH1 + a.assign("shaders/basic.vs")).c_str(), (PATH1 + a.assign("shaders/gui.fs")).c_str());
	if (!shader) return;


	//enable shader
	shader->enable();

	//upload uniforms
	shader->setUniform("u_color", color);
	shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	shader->setUniform("u_texture", tex, 0);
	shader->setUniform("u_tex_tiling", 1.0f);
	shader->setUniform("u_time", time);
	shader->setUniform("u_tex_range", Vector4(0, 0, 1, 1));

	shader->setUniform("u_model", Matrix44());
	quad.render(GL_TRIANGLES);


	//disable shader
	shader->disable();
}


//this function is used to access OpenGL Extensions (special features not supported by all cards)
void* getGLProcAddress(const char* name)
{
	return SDL_GL_GetProcAddress(name);
}

//Retrieve the current path of the application
#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

#ifdef WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

std::string getPath()
{
	std::string fullpath;
	// ----------------------------------------------------------------------------
	// This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
#ifdef __APPLE__
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	char path[PATH_MAX];
	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8*)path, PATH_MAX))
	{
		// error!
	}
	CFRelease(resourcesURL);
	chdir(path);
	fullpath = path;
#else
	char cCurrentPath[1024];
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
		return "";

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
	fullpath = cCurrentPath;

#endif    
	return fullpath;
}

template <typename T>
std::string to_string(T value)
{
	//create an output string stream
	std::ostringstream os;

	//throw the value into the string stream
	os << value;

	//convert the string stream into a string and return
	return os.str();
}

bool readFile(const std::string& filename, std::string& content)
{
	content.clear();

	long count = 0;

	FILE* fp = fopen(filename.c_str(), "rb");
	if (fp == NULL)
	{
		std::cerr << "::readFile: file not found " << filename << std::endl;
		return false;
	}

	fseek(fp, 0, SEEK_END);
	count = ftell(fp);
	rewind(fp);

	content.resize(count);
	if (count > 0)
	{
		count = fread(&content[0], sizeof(char), count, fp);
	}
	fclose(fp);

	return true;
}

bool readFileBin(const std::string& filename, std::vector<unsigned char>& buffer)
{
	buffer.clear();
	FILE* fp = nullptr;
	fp = fopen(filename.c_str(), "rb");
	if (fp == nullptr)
		return false;
	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	rewind(fp);
	buffer.resize(size);
	fread(&buffer[0], sizeof(char), buffer.size(), fp);
	fclose(fp);
	return true;
}

void stdlog(std::string str)
{
	std::cout << str << std::endl;
}

bool checkGLErrors()
{
#ifdef _DEBUG
	GLenum errCode;
	const GLubyte* errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL Error: " << errString << std::endl;
		return false;
	}
#endif

	return true;
}

std::vector<std::string>& split(const std::string& s, char delim, std::vector<std::string>& elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

std::string join(std::vector<std::string>& strings, const char* delim)
{
	std::string str;
	for (int i = 0; i < strings.size(); ++i)
		str += strings[i] + (i < strings.size() - 1 ? std::string(delim) : "");
	return str;
}

Vector2 getDesktopSize(int display_index)
{
	SDL_DisplayMode current;
	// Get current display mode of all displays.
	int should_be_zero = SDL_GetCurrentDisplayMode(display_index, &current);
	return Vector2((float)current.w, (float)current.h);
}


bool drawText(float x, float y, std::string text, Vector3 c, float scale)
{
	static char buffer[99999]; // ~500 chars
	int num_quads;

	if (scale == 0)
		return true;

	x /= scale;
	y /= scale;

	if (Shader::current)
		Shader::current->disable();

	num_quads = stb_easy_font_print(x, y, (char*)(text.c_str()), NULL, buffer, sizeof(buffer));

	Matrix44 projection_matrix;
	projection_matrix.ortho(0, Game::instance->window_width / scale, Game::instance->window_height / scale, 0, -1, 1);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixf(Matrix44().m);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadMatrixf(projection_matrix.m);

	glColor3f(c.x, c.y, c.z);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 16, buffer);
	glDrawArrays(GL_QUADS, 0, num_quads * 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return true;
}

std::vector<std::string> tokenize(const std::string& source, const char* delimiters, bool process_strings)
{
	std::vector<std::string> tokens;

	std::string str;
	size_t del_size = strlen(delimiters);
	const char* pos = source.c_str();
	char in_string = 0;
	unsigned int i = 0;
	while (*pos != 0)
	{
		bool split = false;

		if (!process_strings || (process_strings && in_string == 0))
		{
			for (i = 0; i < del_size && *pos != delimiters[i]; i++);
			if (i != del_size) split = true;
		}

		if (process_strings && (*pos == '\"' || *pos == '\''))
		{
			if (!str.empty() && in_string == 0) //some chars remaining
			{
				tokens.push_back(str);
				str.clear();
			}

			in_string = (in_string != 0 ? 0 : *pos);
			if (in_string == 0)
			{
				str += *pos;
				split = true;
			}
		}

		if (split)
		{
			if (!str.empty())
			{
				tokens.push_back(str);
				str.clear();
			}
		}
		else
			str += *pos;
		pos++;
	}
	if (!str.empty())
		tokens.push_back(str);
	return tokens;
}

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

std::string getGPUStats()
{
	GLint nTotalMemoryInKB = 0;
	glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &nTotalMemoryInKB);
	GLint nCurAvailMemoryInKB = 0;
	glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &nCurAvailMemoryInKB);
	if (glGetError() != GL_NO_ERROR) //unsupported feature by driver
	{
		nTotalMemoryInKB = 0;
		nCurAvailMemoryInKB = 0;
	}

	std::string str = "FPS: " + to_string(Game::instance->fps) + " DCS: " + to_string(Mesh::num_meshes_rendered) + " Tris: " + to_string(long(Mesh::num_triangles_rendered * 0.001)) + "Ks  VRAM: " + to_string(int((nTotalMemoryInKB - nCurAvailMemoryInKB) * 0.001)) + "MBs / " + to_string(int(nTotalMemoryInKB * 0.001)) + "MBs";
	Mesh::num_meshes_rendered = 0;
	Mesh::num_triangles_rendered = 0;
	return str;
}

Mesh* grid = NULL;

void drawGrid()
{
	if (!grid)
	{
		grid = new Mesh();
		grid->createGrid(10);
	}

	glLineWidth(1);
	glEnable(GL_BLEND);
	glDepthMask(false);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Shader* grid_shader = Shader::getDefaultShader("grid");
	grid_shader->enable();
	Matrix44 m;
	m.translate(floor(Camera::current->eye.x / 100.0) * 100.0f, 0.0f, floor(Camera::current->eye.z / 100.0f) * 100.0f);
	grid_shader->setUniform("u_color", Vector4(0.7, 0.7, 0.7, 0.7));
	grid_shader->setUniform("u_model", m);
	grid_shader->setUniform("u_camera_position", Camera::current->eye);
	grid_shader->setUniform("u_viewprojection", Camera::current->viewprojection_matrix);
	grid->render(GL_LINES); //background grid
	glDisable(GL_BLEND);
	glDepthMask(true);
	grid_shader->disable();
}


char* fetchWord(char* data, char* word)
{
	int pos = 0;
	while (*data && *data != ',' && *data != '\n' && pos < 254) { word[pos++] = *data; data++; }
	word[pos] = 0;
	if (pos < 254)
		data++; //skip ',' or '\n'
	return data;
}

char* fetchFloat(char* data, float& v)
{
	char w[255];
	data = fetchWord(data, w);
	v = atof(w);
	return data;
}

char* fetchMatrix44(char* data, Matrix44& m)
{
	char word[255];
	for (int i = 0; i < 16; ++i)
	{
		data = fetchWord(data, word);
		m.m[i] = atof(word);
	}
	return data;
}

char* fetchEndLine(char* data)
{
	while (*data && *data != '\n') { data++; }
	if (*data == '\n')
		data++;
	return data;
}

char* fetchBufferFloat(char* data, std::vector<float>& vector, int num)
{
	int pos = 0;
	char word[255];
	if (num)
		vector.resize(num);
	else //read size with the first number
	{
		data = fetchWord(data, word);
		float v = atof(word);
		assert(v);
		vector.resize(v);
	}

	int index = 0;
	while (*data != 0) {
		if (*data == ',' || *data == '\n')
		{
			if (pos == 0)
			{
				data++;
				continue;
			}
			word[pos] = 0;
			float v = atof(word);
			vector[index++] = v;
			if (*data == '\n' || *data == 0)
			{
				if (*data == '\n')
					data++;
				return data;
			}
			data++;
			if (index >= vector.size())
				return data;
			pos = 0;
		}
		else
		{
			word[pos++] = *data;
			data++;
		}
	}

	return data;
}

char* fetchBufferVec3(char* data, std::vector<Vector3>& vector)
{
	//int pos = 0;
	std::vector<float> floats;
	data = fetchBufferFloat(data, floats);
	vector.resize(floats.size() / 3);
	memcpy(&vector[0], &floats[0], sizeof(float) * floats.size());
	return data;
}

char* fetchBufferVec2(char* data, std::vector<Vector2>& vector)
{
	//int pos = 0;
	std::vector<float> floats;
	data = fetchBufferFloat(data, floats);
	vector.resize(floats.size() / 2);
	memcpy(&vector[0], &floats[0], sizeof(float) * floats.size());
	return data;
}

char* fetchBufferVec3u(char* data, std::vector<Vector3u>& vector)
{
	//int pos = 0;
	std::vector<float> floats;
	data = fetchBufferFloat(data, floats);
	vector.resize(floats.size() / 3);
	for (int i = 0; i < floats.size(); i += 3)
		vector[i / 3].set(floats[i], floats[i + 1], floats[i + 2]);
	return data;
}

char* fetchBufferVec4ub(char* data, std::vector<Vector4ub>& vector)
{
	//int pos = 0;
	std::vector<float> floats;
	data = fetchBufferFloat(data, floats);
	vector.resize(floats.size() / 4);
	for (int i = 0; i < floats.size(); i += 4)
		vector[i / 4].set(floats[i], floats[i + 1], floats[i + 2], floats[i + 3]);
	return data;
}

char* fetchBufferVec4(char* data, std::vector<Vector4>& vector)
{
	//int pos = 0;
	std::vector<float> floats;
	data = fetchBufferFloat(data, floats);
	vector.resize(floats.size() / 4);
	memcpy(&vector[0], &floats[0], sizeof(float) * floats.size());
	return data;
}


