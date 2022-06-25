#include "BulletManager.h"
#define MAX_BULLETS 500
BulletManager::BulletManager()
{
	bullets_vector[MAX_BULLETS];
}

void BulletManager::render()
{
	glEnable(GL_POINTS);
	for (int i = 0; i < bullets_vector.size(); i++) {
		sBullet current = bullets_vector[i];
		if (current.isActive()) { 

		}
	};
}

void BulletManager::update(float elapsed_time)
{
	//mover cada bala
	//eliminar balas que excedan su tiempo de vida
	for (int i = 0; i < bullets_vector.size(); i++) {
		sBullet current = bullets_vector[i];
		current.last_position = current.last_position + current.velocity * elapsed_time;

		current.ttl -= elapsed_time;
		//.isActive()) {
		//};
	};
	/*for (int i = 0; i < bullets_vector.size(); i++) {
		sbullet current = bullets_vector[i];
		if (current.isactive()) {
			continue; 
			}
		else {   }
		}*/
	
	//testear que balas han colisionado con alguna entidad
}

void BulletManager::createBullet(Vector3 last_position, Vector3 vel, int author, float ttl, float power)
{
	sBullet newBullet;
	newBullet.last_position = last_position;
	newBullet.velocity = vel;
	newBullet.power = power;
	newBullet.ttl = ttl;
	newBullet.author = author;
	bullets_vector.push_back(newBullet);
	

}
