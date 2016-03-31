#include "Pointer.h"
#include "PlayerBall.h"

#include <InputHandler.h>
#include <PhysicsWorld.h>
#include <Game.h>
#include "DIY_Level.h"
#include <iostream>

Pointer::Pointer(Level * level_, Vec3 position_, PlayerBall * player_) : GameObject(level_, position_), followCam(true)
{
	renderer = new Generic_RenderableComponent(this, "pointer", "white");

	Scale(Vec3(0.075f, 0.075f, 0.075f));

	ball = player_;
	Rotate(Quat(M_PI / 2.0f, Vec3::BasisY()));
}


Pointer::~Pointer()
{
}

void Pointer::Update(float timeStep_)
{
	GameObject::Update(timeStep_);

	position.y = ball->position.y;

	if (Input->isKeyDown(SDLK_RIGHT) || Input->isKeyDown(SDLK_d))
	{
		RotateAround(ball->position, Vec3::BasisY(), 3 * timeStep_);
	}
	if (Input->isKeyDown(SDLK_LEFT) || Input->isKeyDown(SDLK_a))
	{
		RotateAround(ball->position, Vec3::BasisY(), -3 * timeStep_);

	}

	if (Input->isKeyReleased(SDLK_k))
		followCam = !followCam;

	if (Input->isMouseDown(SDL_BUTTON_RIGHT) && followCam)
	{
		//RotateAround(ball->position, Vec3::BasisY(), Input->deltaMouse().x * timeStep_);
		position = ball->position + Vec3(level->currentCamera->position.x - ball->position.x, 0, level->currentCamera->position.z - ball->position.z).Normalized() * 2;
		LookAt(ball->position);
		Rotate(Quat(M_PI / 2, Vec3::BasisY()));
	}

	if (((DIY_Level*)GAME->currentLevel)->levelState == DIY_Level_State::PLAYING &&
		((DIY_Level*)GAME->currentLevel)->playingState == DIY_Level_Playing_State::SHOOTING)
	{
		if (!isEnabled)
		{
			position = ball->position + Vec3(level->currentCamera->position.x - ball->position.x, 0, level->currentCamera->position.z - ball->position.z).Normalized() * 2;
			LookAt(ball->position);
			Rotate(Quat(M_PI / 2, Vec3::BasisY()));
			//Translate(Vec3(0, -1, 0));
		}
		isEnabled = true;
	}
	else
		isEnabled = false;

	renderer->isEnabled = isEnabled;

	//position += (level->currentCamera->right() * 0.5f);

}

void Pointer::Render()
{
	if (isEnabled)
		GameObject::Render();
}