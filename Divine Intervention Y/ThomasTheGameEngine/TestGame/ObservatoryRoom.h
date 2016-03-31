#pragma once
#include <GameObject.h>
#include <Billboard.h>
#include "AmbientStar.h"
#include "MockSolarSystem.h"

class Level;
class RenderableComponent;
class Music;
class Player;

class ObservatoryRoom :
	public GameObject
{
public:
	ObservatoryRoom(Level *, Vec3, Player *);
	~ObservatoryRoom();

	void Update(float _deltaTime) override;

private:

	Player * player;
	RenderableComponent * skyBox;
	Music * observatoryMusic;

	AmbientStar * abs;
	
	GameObject * sun;
	MockSolarSystem * solarSystem;
};

