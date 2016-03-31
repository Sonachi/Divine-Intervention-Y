#pragma once
#include "GameObject.h"
#include "RenderableComponent.h"

class Level;

class Billboard : public GameObject
{
public:
	Billboard(Level * _level);
	Billboard(Level * _level, Vec3 _position);
	//Billboard(Level * _level, Vec3 _position, Quat _rotation, Vec3 _scale){};

	void Update(float _deltaTime) override;

private:
	RenderableComponent * rc;
};