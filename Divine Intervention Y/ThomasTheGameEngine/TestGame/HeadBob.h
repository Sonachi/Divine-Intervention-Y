#pragma once
#include <GameObject.h>
#include <Camera.h>
/*
	A Class that simulates headbobbing with a camera.
*/

class Level;

class HeadBob : public GameObject
{
public:
	HeadBob(Level *, Vec3, Camera*);
	~HeadBob();

	virtual void Update(float _deltaTime) override;

private:
	Camera* camera;
	
	float t;
	Vec3 previousPosition;
};
