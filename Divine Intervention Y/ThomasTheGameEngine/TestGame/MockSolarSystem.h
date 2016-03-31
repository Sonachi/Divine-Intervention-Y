#pragma once
#include <GameObject.h>
#include "AsteroidField.h"

class Level;

class MockSolarSystem :
	public GameObject
{
public:
	MockSolarSystem(Level *, Vec3);
	~MockSolarSystem();

	void Update(float _deltaTime) override;

//private:
	std::vector<GameObject *> layers;

	AsteroidField * af;

	GameObject * f;
	GameObject * f2;

	GameObject * aruba;
	GameObject * Rim;
	GameObject * miner;
};

