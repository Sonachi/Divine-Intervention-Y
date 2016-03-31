#pragma once

#include <GameObject.h>
#include <RenderableComponent.h>
#include <Rigidbody.h>

#include "Pointer.h"

class Sound;

class PlayerBall : public GameObject
{
public:
	PlayerBall(Level *, Vec3);
	~PlayerBall(){}

	void Update(float timeStep_) override;

	float GetChargePercent();
	bool GetIsChargingStrike();

	void FoulReset();

	Pointer * hand;
private:
	Generic_RenderableComponent * renderer;
	Rigidbody * rigidBody; 

	bool chargingStrike;

	const int MAX_FORCE = 250;
	int force_mult;
	const float CHARGE_PER_SECOND = 100.0f;

	Vec3 positionAtStrike;

	float chargePercent;
	float modifier;

	Sound * hitSound;
};