//Author: Nathan Senter
//Date: 28/01/2016

#pragma once

#include <Camera.h>

class GameCamera : public Camera
{

public:

	//Will store the forward vector when you begin rotating. Will be the reference to return to its
	//original look at position when the mouse is released
	Vec3 originalLookat;
	Quat originalRotation;

	GameCamera(Level * level_, Vec3 position_, Vec3 lookAt_);
	~GameCamera();

	void Update(float _deltaTime) override;

private:
	bool isOriginalSet;
};