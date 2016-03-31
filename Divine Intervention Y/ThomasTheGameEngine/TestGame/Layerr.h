#pragma once
#include <GameObject.h>
#include <RenderableComponent.h>
#include <Rigidbody.h>


class Layerr :
	public GameObject
{
public:
	Layerr(Level *, Vec3 position_, std::vector<GameObject *>);
	~Layerr();

	std::vector<GameObject *> objects;

	void Update(float) override;
private:
	RenderableComponent * r;
};


