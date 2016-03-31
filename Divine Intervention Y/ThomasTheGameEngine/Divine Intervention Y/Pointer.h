#pragma once
#include <GameObject.h>
#include <RenderableComponent.h>

class PlayerBall;

class Pointer :
	public GameObject
{
public:
	Pointer(Level *, Vec3, PlayerBall *);
	~Pointer();

	void Update(float timeStep_) override;
	void Render() override;

private:
	Generic_RenderableComponent * renderer;
	PlayerBall * ball;

	bool followCam;
	bool isEnabled;
};

