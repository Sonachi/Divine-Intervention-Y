#pragma once

#include <Level.h>
#include "Player.h"
#include "Hallway.h"
#include "ObservatoryRoom.h"
#include "SlidingDoor.h"

class Observatory : public Level
{
public:
	Observatory();
	~Observatory();

	Player * player;
	Hallway * hallway;
	SlidingDoor * door;
	ObservatoryRoom * obr;

	void LoadContent() override;
	void LevelUpdate(float timeStep_) override;
};