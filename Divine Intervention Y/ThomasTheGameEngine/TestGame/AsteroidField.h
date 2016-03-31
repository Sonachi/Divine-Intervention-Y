#pragma once
#include <GameObject.h>

class Level;

class AsteroidField :
	public GameObject
{
public:
	AsteroidField(Level *, Vec3, float _radius, float _numAsteroids);
	~AsteroidField();

private:
	std::vector<GameObject *> asteroids;
};

