#include "BulletManager.h"
Bullet::Bullet()
{
	
}

bool Bullet::isActive()
{
	return ttl <= 0.0f;
}

void Bullet::render()
{
	
}

void Bullet::update(float elapsed_time)
{
	
}

void Bullet::createBullet(Vector3 last_position, Vector3 vel, int author, float ttl, float power)
{

}
