#include "HeadBob.h"

HeadBob::HeadBob(Level *_level, Vec3 _position, Camera* _camera) : GameObject(_level, _position), camera(_camera)
{
	t = 0;

	_camera->position = position;
	_camera->rotation = rotation;
	addChild(_camera);
}

HeadBob::~HeadBob()
{
}

void HeadBob::Update(float _deltaTime)
{
	GameObject::Update(_deltaTime);

	if (previousPosition != position)
	{
		t += 6.0f * _deltaTime;
		camera->position.y = position.y + sin(t) / 15.0f;

		previousPosition = position;
	}
}