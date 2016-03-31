#pragma once
#include <Billboard.h>
#include <Flipbook.h>

class AmbientStar :
	public GameObject
{
public:
	AmbientStar(Level *, Vec3);
	~AmbientStar();

	void Update(float _deltaTime) override;

private:
	float elapsedTime;

	Billboard * b;
	Flipbook * fb;
};

