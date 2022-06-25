#pragma once
#include "utils.h"
struct sBullet {
	Matrix44 model;
	Vector3 last_position;
	Vector3 velocity;
	float ttl;
	float power;
	int author;
	bool isActive() {
		return ttl <= 0.0f;
	};
};
class BulletManager
{
public:
	BulletManager();
	EntityMesh bullets;
	std::vector<sBullet> bullets_vector;
	void render();
	void update(float elapsed_time);
	void createBullet(Vector3 last_position, Vector3 vel, int author, float ttl, float power);
};

