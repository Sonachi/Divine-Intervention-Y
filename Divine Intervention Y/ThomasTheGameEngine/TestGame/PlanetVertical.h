#pragma once

#include <GameObject.h>
#include <RenderableComponent.h>

class PlanetVertical : public GameObject
{
public:
	PlanetVertical(Level *, Vec3, std::string textName_);
	~PlanetVertical();

	RenderableComponent * renderer;
};