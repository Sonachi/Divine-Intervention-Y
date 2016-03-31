//Author: Nathan Senter
//Date: 2/26/2016

#pragma once

#include <GameObject.h>
#include <RenderableComponent.h>

class GalaxyMap : public GameObject
{
public:
	GalaxyMap(Level *);
	~GalaxyMap();

	void Update(float) override;

	void SetIsRotating(bool);
	bool IsRotating();

	std::vector<GameObject *> nodes;
private:

	const int NUMBER_OF_NODES = 2;

	//2 planes so theyll both render when you rotate it around
	//Face-culling avoidance
	GameObject * plane1, * plane2;

	//galexy center
	GameObject *glow, *core;
	Sun_RenderableComponent * renderer;
	Glow_RenderableComponent * glow_renderer;

	//Gate for idle animation when title screen is up
	bool isRotating;
};

