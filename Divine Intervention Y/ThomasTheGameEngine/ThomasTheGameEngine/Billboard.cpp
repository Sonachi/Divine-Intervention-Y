#include "Billboard.h"
#include "Game.h"
#include "Level.h"
#include "ModelManager.h"
#include <iostream>

Billboard::Billboard(Level * _level) : GameObject(_level)
{
	rc = new Generic_RenderableComponent(this,"plane", "white");
	position = Vec3(0, 0, 0);
	rotation = Quat();
	scale = Vec3(1, 1, 1);
}

Billboard::Billboard(Level * _level, Vec3 _position) : GameObject(_level)
{
	rc = new Generic_RenderableComponent(this, "plane", "white");
	position = _position;
	rotation = Quat();
	scale = Vec3(1, 1, 1);
}

void Billboard::Update(float _deltaTime)
{
	GameObject::Update(_deltaTime);

	LookAt(level->currentCamera->position);
}