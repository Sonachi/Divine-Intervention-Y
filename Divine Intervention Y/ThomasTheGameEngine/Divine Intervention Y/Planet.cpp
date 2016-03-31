#include "Planet.h"
#include "PlayerBall.h"
#include "DIY_Level.h"
#include <iostream>

#include <Rigidbody.h>
Planet::Planet(Level * level_, Vec3 position_, float scaleFactor_, std::string textName, bool hasAtmosphere_) : GameObject(level_), hasAtmosphere(hasAtmosphere_)
{
	Scale(Vec3(scaleFactor_, scaleFactor_, scaleFactor_));

	position = position_ + Vec3(0, 0.5f, 0);

	renderer = new Generic_RenderableComponent(this, "sphere", textName);

	rigidbody = new Rigidbody(this, new SphereCollider(this));
	((SphereCollider *)rigidbody->col)->collisionRadius = scale.x / 2.0f;
	rigidbody->CollisionRadius = scale.x / 2.0f;
	rigidbody->isKinematic = false;
	rigidbody->mass = 50.0f;

	rigidbody->setTensorShape(Collider::Sphere, Vec3(scale.x / 2.0f, scale.x / 2.0f, scale.x / 2.0f));

	if (hasAtmosphere)
	{
		atmosphere = new GameObject(level_, position);
		at_renderer = new Atmosphere_RenderableComponent(atmosphere, "sphere");
		atmosphere->Scale(Vec3::One() * scaleFactor_ * 1.02f);
		addChild(atmosphere);
	}
}

void Planet::Update(float timeStep_)
{
	Rotate(Quat(0.3f * timeStep_, Vec3(0, 1, 0)));

	if (hasAtmosphere)
	{
		atmosphere->Rotate(Quat(0.15f * timeStep_, Vec3(0, 1, 0)));
		at_renderer->offset += Vec3::BasisX() * 0.05f * timeStep_;
	}

	//Instantiate Player
	if (!player)
	{
		player = (PlayerBall*)level->FindGameObjectWithTag("player");
	}

	//Check Victory conditions
	if (Collider::isColliding(rigidbody->col, player->getComponent<Rigidbody>()->col))
	{
		DIY_Level * level_ = ((DIY_Level *)level);

		if (level_->GetLayerPlane()->GetIndex() == 0)
		{
			level_->PlayerHasShotBallIntoSun = true;
		}
	}
}

void Planet::SetRenderEnabled(bool isEnabled_)
{
	GameObject::SetRenderEnabled(isEnabled_);

	if (hasAtmosphere)
		at_renderer->isEnabled = isEnabled_;
}