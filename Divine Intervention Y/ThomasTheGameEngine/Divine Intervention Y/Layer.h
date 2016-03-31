#pragma once
#include <GameObject.h>
#include <RenderableComponent.h>
#include <Rigidbody.h>
#include <dependencies\TinyXML\tinyxml2.h>

#include "Obstacles.h"
#include "Planet.h"

class DIY_Level;

class Layer :
	public GameObject
{
public:
	Layer(Level *, tinyxml2::XMLElement *, int);
	~Layer();

	std::vector<GameObject *> objects;

	void SetEnabled(bool);
	int GetIndex();
private:
	Generic_RenderableComponent * r;
	DIY_Level * dl;

	Wormhole * wormHole;
	Planet * planet;

	int index;
	float distanceFromSun;

	void CreateObjects(const char *, const Vec2);
};


