#include "PlanetHorizontal.h"

PlanetHorizontal::PlanetHorizontal(Level * level_, Vec3 position_, std::string textName) : GameObject(level_)
{
	position = position_;

	renderer = new RenderableComponent("sphere", textName, this);
}