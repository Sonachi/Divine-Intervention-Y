#include "AmbientStar.h"
#include <stdlib.h>

AmbientStar::AmbientStar(Level * _level, Vec3 _position) : GameObject(_level, _position)
{
	b = new Billboard(_level, _position);
	b->Scale(Vec3(0.6, 0.6, 0.6));
	fb = new Flipbook(b, 12, "Images/Animation/Star/slice0.png", 0.5f, false, Flipbook::PNG);
	fb->SetEffecctedByLight(false, false, false);
	elapsedTime = rand() % 5;
}


AmbientStar::~AmbientStar()
{
}

void AmbientStar::Update(float _deltaTime)
{
	GameObject::Update(_deltaTime);

	if (elapsedTime > 5.5f)
	{
		fb->Play();
		elapsedTime = 0.0f;
	}
	else elapsedTime += _deltaTime;
}
