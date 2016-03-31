#pragma once

#include <GameObject.h>

class Camera;
class Light;
class ParticleSystem;

class Player :
	public GameObject
{
public:
	Player(Level *, Camera *);
	~Player();

	void Update(float _deltaTime) override;

	GameObject * lightHolder;

	Camera * fpsCamera;
	Light * flashLight;

	float t;
	Vec3 previousPosition;

	ParticleSystem * feetMagic;
};

