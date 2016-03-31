#include "Obstacles.h"
#include "Planet.h"
#include "DIY_Level.h"

Satellite::Satellite(DIY_Level* level_, Planet* planet_, Vec3 startingPos_, Vec3 inPlaneVector_, float orbitSpeed_, std::string modelName_, std::string textureName_) : GameObject(level_, startingPos_), orbitSpeed(orbitSpeed_), planet(planet_)
{
	renderer = new Generic_RenderableComponent(this, modelName_, textureName_);

	orbitAxis = Vec3::cross(startingPos_ - planet_->position, inPlaneVector_);
	rigidBody = new Rigidbody(this, new SphereCollider(this));
	rigidBody->isKinematic = false;
}

void Satellite::Update(float deltaTime_)
{
	RotateAround(planet->position, orbitAxis, orbitSpeed);
}