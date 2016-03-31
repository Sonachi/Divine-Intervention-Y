//Author:	Mathieu Violette
//Date:		2/15/2016
#pragma once

#include <GameObject.h>
#include <InputHandler.h>

class FPS_Controls
{
public:
	static float FPSMoveSpeed;
	static float FPSHorizontalTurnSpeed;
	static float FPSVerticalTurnSpeed;

	static void Update(GameObject* go_, float timeStep_);
};