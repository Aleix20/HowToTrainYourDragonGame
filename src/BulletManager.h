#pragma once
#include "utils.h"

class Bullet
{
public:
	Bullet();
	EntityMesh bullet;
	Matrix44 model;
	Vector3 current_position;
	Vector3 last_position;
	Vector3 velocity;
	float ttl;
	float power;
	int author;
	bool isActive();
	void render();
	void update(float elapsed_time);
	void createBullet(Vector3 last_position, Vector3 vel, int author, float ttl, float power);
};

