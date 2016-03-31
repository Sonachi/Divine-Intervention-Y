#include "Obstacles.h"
#include "PlayerBall.h"
#include "DIY_Level.h"

#include <AudioManager.h>


WarpGate::WarpGate(Level * level_, Vec3 position_, Quat rotation_) : GameObject(level_, position_)
{
	position = position + Vec3(0, 0.5f, 0);
	rotation = rotation_;

	rc = new Generic_RenderableComponent(this, "warpGate", "gateTexture");

	Scale(Vec3(0.2, 0.2, 0.2));

	Rotate(Quat((rand() % 366) * M_PI / 180.0f, Vec3::BasisY()));

	Audio->loadSound("warpSound", "Sounds/warp.wav");
	sound = Audio->getSound("warpSound");
}

void WarpGate::Update(float timeStep_)
{
	if (!player)
		player = (PlayerBall *)level->FindGameObjectWithTag("player");

	GameObject::Update(timeStep_);

	Rotate(Quat(180 * M_PI / 180.0f * timeStep_, Vec3::BasisY()));

	if ((position - player->position).length() < 1.f)
	{
		Rigidbody * rb = player->getComponent<Rigidbody>();

		if (Vec3::dot(forward(), rb->velocity) > 0.5f || Vec3::dot(forward() * -1, rb->velocity) > 0.5f)
		{
			rb->AddForce(rb->velocity.Normalized() * 10.0f);
			sound->Play();
		}
		else
		{
			rb->velocity = rb->velocity * -1.f;
		}
	}
}