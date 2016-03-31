#pragma once

#include <GameObject.h>

class RenderableComponent;

class WarpGate : public GameObject
{
public:
	WarpGate(Level *, Vec3, Quat);
	~WarpGate(){}

private:
	RenderableComponent * rc;
};