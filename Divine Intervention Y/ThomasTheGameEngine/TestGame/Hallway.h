#pragma once
#include <GameObject.h>

class Level;

class Hallway :
	public GameObject
{
public:
	Hallway(Level *, Vec3);
	~Hallway();

	GameObject * floor;
	GameObject * wall1;
	GameObject * wall2;
	GameObject * cieling;

	void letThereBeLight();
};

