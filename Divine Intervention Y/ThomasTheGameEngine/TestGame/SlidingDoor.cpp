#include "SlidingDoor.h"
#include <RenderableComponent.h>
#include "Player.h"
#include "Hallway.h"

#include <AudioManager.h>
#include <Level.h>

SlidingDoor::SlidingDoor(Level * _level, Vec3 _position, Player * _player) : GameObject(_level, _position)
{
	door1 = new GameObject(_level, _position + Vec3(-.25f, 0, 0));
	d1OriginalPosition = door1->position;

	door2 = new GameObject(_level, _position + Vec3(0.25f, 0, 0));
	door2->Rotate(Quat(3.141592654, Vec3(1, 0, 0)));
	d2OriginalPosition = door2->position;

	RenderableComponent * r = new RenderableComponent("doorModel", "door", door1);
	RenderableComponent * r1 = new RenderableComponent("doorModel", "door", door2);
	r->SetEffecctedByLight(true, false, true);
	r1->SetEffecctedByLight(true, false, true);

	player = _player;

	doorsInMotion = false;

	AudioManager::getInstance()->loadSound("doorSound", "Sounds/door.wav");
	doorOpening = AudioManager::getInstance()->getSound("doorSound");
}


SlidingDoor::~SlidingDoor()
{
}

void SlidingDoor::Update(float _deltaTime)
{
	GameObject::Update(_deltaTime);

	if ((position - player->position).magnitude() < 2.5)
	{
		Hallway * hallway = static_cast<Hallway *>(level->FindGameObjectWithTag("hallway"));
		hallway->letThereBeLight();

		if (!doorsInMotion)
		{
			doorsInMotion = true;
			doorOpening->Play();
		}

		if (door2->position.x < d2OriginalPosition.x + 0.5)
			door2->Translate(Vec3(0.25f * _deltaTime, 0, 0));

		if (door1->position.x > d1OriginalPosition.x - 0.5)
			door1->Translate(Vec3(-0.25f * _deltaTime, 0, 0));
	}
}
