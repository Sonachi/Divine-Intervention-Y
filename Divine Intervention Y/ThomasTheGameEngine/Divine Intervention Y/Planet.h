#pragma once

#include <GameObject.h>
#include <RenderableComponent.h>

class Rigidbody;
class PlayerBall;

class Planet : public GameObject
{
public:
	Planet(Level *, Vec3, float, std::string textName_, bool hasAtmosphere_ = true);
	~Planet();

	void Update(float) override;
	Generic_RenderableComponent * renderer;
	Atmosphere_RenderableComponent * at_renderer;

	virtual void SetRenderEnabled(bool isEnabled_) override;

private:
	bool hasAtmosphere;

	GameObject * atmosphere;

	Rigidbody * rigidbody;
	PlayerBall * player;
};