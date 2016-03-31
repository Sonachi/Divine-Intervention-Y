#pragma once
#include <GameObject.h>
#include <RenderableComponent.h>
#include <Light.h>

class Rigidbody;
class PlayerBall;

class Sun : public GameObject
{
public:
	Sun(Level * level_, Vec3 position_, float radius_);
	~Sun();

	void Update(float) override;

private:
	GameObject* glow;
	Sun_RenderableComponent * renderer;
	Glow_RenderableComponent * glow_renderer;
	Light * light;
	Rigidbody * rigidBody;
	PlayerBall * player;
	Rigidbody * playerRigidBody;
	bool ballHitIntoSun;
	const float INTENSITY_INCREMENT = 0.25f;
	const float	MINIMUM_INTENSITY = 0.8f;
	const float MAXIMUM_INTENSITY = 1.0f;
};

