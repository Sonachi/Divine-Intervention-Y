#include "Layerr.h"

Layerr::Layerr(Level * _level, Vec3 _position, std::vector<GameObject *> _objects) : GameObject(_level, _position)
{
	objects = _objects;

	for (int i = 0; i < objects.size(); i++)
	{
		addChild(objects[i]);
	}

	r = new RenderableComponent("cuboid", "layerGrid", this);
	r->SetEffecctedByLight(false, false, false);
}


Layerr::~Layerr()
{
}

void Layerr::Update(float timeStep_)
{
}
