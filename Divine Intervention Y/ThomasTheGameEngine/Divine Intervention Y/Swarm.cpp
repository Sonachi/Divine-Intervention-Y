#include "Obstacles.h"
#include "PlayerBall.h"

#include <Level.h>

Swarm::Swarm(Level * level_, Vec3 position_) : GameObject(level_, position_)
{
	inAttackMode = false;

	aliens.reserve(sizeof(Alien *)* NUM_ALIENS);

	float theta = 0;
	float startY = -0.2f;

	for (int i = 0; i < NUM_ALIENS; i++)
	{
		Vec3 position_ = position + Vec3(cos(theta), startY, sin(theta)).Normalized() * 0.95f;

		aliens.push_back(new Alien(level_, position_, this));

		theta += 2 * 3.141592654 / (NUM_ALIENS / 2.0f);
		
		if (theta > 2 * 3.141592654)
		{
			theta -= 1.5 * 3.141592654;
			startY += 0.2f;
		}
	}
}


Swarm::~Swarm()
{
}

void Swarm::Update(float timeStep_)
{
	if (!player)
	{
		player = static_cast<PlayerBall *>(level->FindGameObjectWithTag("player"));
	}

	if ((player->position - position).magnitude() < 2.5f)
	{
		if (!inAttackMode)
		{
			inAttackMode = true;

			for (int i = 0; i < NUM_ALIENS; i++)
			{
				aliens[i]->ToggleState();
			}
		}

		if (player->getComponent<Rigidbody>()->isAwake())
		{
			printf("Awake");
		}
		else
		{
			player->FoulReset();
		}
	}
	else
	{
		if (inAttackMode)
		{
			inAttackMode = false;

			for (int i = 0; i < NUM_ALIENS; i++)
			{
				aliens[i]->ToggleState();
			}
		}
	}
}
