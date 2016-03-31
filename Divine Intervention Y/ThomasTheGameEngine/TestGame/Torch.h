#pragma once
#include <GameObject.h>

#include <RenderableComponent.h>
#include <Billboard.h>
#include <ParticleSystem.h>

class Level;

class Torch :
	public GameObject
{
public:
	Torch(Level *, Vec3);
	~Torch();

private:
	RenderableComponent * torch;
	ParticleSystem * smokeEmitter;
	Billboard * flame;
};

