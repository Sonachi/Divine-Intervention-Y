#pragma once
#include <GameObject.h>

class Player;
class Sound;

class SlidingDoor :
	public GameObject
{
public:
	SlidingDoor(Level *, Vec3, Player *);
	~SlidingDoor();

	void Update(float _deltaTime) override;

	Player * player;

private:

	GameObject * door1;
	GameObject * door2;

	bool doorsInMotion;
	Sound * doorOpening;

	Vec3 d1OriginalPosition, d2OriginalPosition;
};

