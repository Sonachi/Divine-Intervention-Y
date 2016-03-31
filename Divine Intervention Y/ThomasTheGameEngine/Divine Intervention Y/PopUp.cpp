#include "Obstacles.h"
#include "PlayerBall.h"

#include <Level.h>

PopUp::PopUp(Level * level_, Vec3 position_) : GameObject(level_, position_)
{
	renderer = new Generic_RenderableComponent(this, "warpGate", "gateTexture");
	Rotate(Quat(3.141592654 / 2.0f, Vec3(1, 0, 0)));

	Scale(Vec3(0.2, 0.2, 0.2));
}

PopUp::~PopUp()
{

}

void PopUp::Update(float timeStep_)
{
	if (!player)
	{
		player = (PlayerBall*)level->FindGameObjectWithTag("player");
	}

	if (player)
	{
		if ((player->position - position).magnitude() < 1.0f)
		{
			Vec3 force = player->getComponent<Rigidbody>()->velocity.Normalized() * 10.0f;
			force.y = UPWARD_FORCE;

			player->getComponent<Rigidbody>()->AddForce(force);
		}
	}

}