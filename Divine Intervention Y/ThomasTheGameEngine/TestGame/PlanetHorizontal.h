#pragma once

#include <GameObject.h>
#include <RenderableComponent.h>

class PlanetHorizontal : public GameObject
{
public:
	PlanetHorizontal(Level *, Vec3, std::string textName_);
	~PlanetHorizontal();

	RenderableComponent * renderer;
};