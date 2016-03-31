#include "AsteroidField.h"
#include <Level.h>
#include <stdio.h>
#include <RenderableComponent.h>

AsteroidField::AsteroidField(Level * _level, Vec3 _position, float _radius, float _numAsteroids) : GameObject(_level, _position)
{
	srand(rand() % 101);
	for (int i = 0; i < _numAsteroids; i++)
	{
		//float theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2 * M_PI));
		//float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / _radius));
		float theta = (rand() % (int)(2 * M_PI * 10000)) / 10000.0f;
		float r = (rand() % (int)(_radius*10000)) / 10000.0f;

		int model = rand() % 4;

		asteroids.push_back(new GameObject(_level, _position));
		asteroids[asteroids.size() - 1]->position = Vec3(asteroids[asteroids.size() - 1]->position.x + r * cos(theta), _position.y + 0.2f, asteroids[asteroids.size() - 1]->position.z + r * sin(theta));
		asteroids[asteroids.size() - 1]->Scale(Vec3(0.001, 0.001, 0.001));
		addChild(asteroids[asteroids.size() - 1]);

		switch (model)
		{
		case 0:
			new RenderableComponent("meteor1", "meteorTex1", asteroids[asteroids.size() - 1]);
			break;
		case 1:
			new RenderableComponent("meteor2", "meteorTex2", asteroids[asteroids.size() - 1]);
			break;
		case 2:
			new RenderableComponent("meteor3", "meteorTex3", asteroids[asteroids.size() - 1]);
			break;
		default:
			break;
		}
	}
}


AsteroidField::~AsteroidField()
{
}
