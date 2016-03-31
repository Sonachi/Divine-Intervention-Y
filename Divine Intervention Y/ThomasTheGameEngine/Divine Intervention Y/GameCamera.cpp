#include "GameCamera.h"
#include <Game.h>
#include <InputHandler.h>
#include "DIY_Level.h"

GameCamera::GameCamera(Level * level_, Vec3 position_, Vec3 lookAt_) : Camera(level_)
{
	position = position_;
	LookAt(lookAt_);

	originalLookat = forward();

	isOriginalSet = false;
}

GameCamera::~GameCamera()
{
	GameObject::~GameObject();
}

void GameCamera::Update(float deltaTime_)
{
	Vec3 right_ = Vec3::cross(forward(), up());

	if (((DIY_Level*)GAME->currentLevel)->levelState == DIY_Level_State::PLAYING)
	{
		//Left mouse camera move
		if (Input->isMouseDown(SDL_BUTTON_LEFT))
		{
			if (!isOriginalSet)
			{
				isOriginalSet = true;
				originalLookat = forward() + position;

				originalRotation = rotation;
			}

			Vec2 mouseDir_ = InputController::getInstance()->deltaMouse();

			Rotate(Quat(-mouseDir_.x * deltaTime_, Vec3(0, 1.0f, 0)));
			Rotate(Quat(-mouseDir_.y * deltaTime_, right_));
		}

		//If youre no longer holding the mouse and have rotated, this section will rotate back
		if (!Input->isMouseDown(SDL_BUTTON_LEFT) && !Input->isMouseDown(SDL_BUTTON_RIGHT)){
			if (forward() != originalLookat)
			{
				LookAt(originalLookat);

				if (((forward() + position) - originalLookat).magnitude() < 0.25f)
				{
					rotation = originalRotation;
					originalLookat = forward();

					isOriginalSet = false;
				}
			}
		}

		if (Input->isKeyDown(SDLK_a) && !Input->isMouseDown(SDL_BUTTON_LEFT))
		{
			Translate(-1 * right_ * deltaTime_);
		}
		else if (Input->isKeyDown(SDLK_d) && !Input->isMouseDown(SDL_BUTTON_LEFT))
		{
			Translate(right_ * deltaTime_);
		}

		/*if (Input->isKeyDown(SDLK_w) && !Input->isMouseDown(SDL_BUTTON_LEFT))
		{
			Translate(forward() * deltaTime_);
		}
		else if (Input->isKeyDown(SDLK_s) && !Input->isMouseDown(SDL_BUTTON_LEFT))
		{
			Translate(forward() * -1 * deltaTime_);
		}*/

		if (Input->mouseWheel > 0)
		{
			if (position.y < 3){
				Translate(up());
				originalLookat = originalLookat + up();
			}
		}
		else if (Input->mouseWheel < 0)
		{
			if (position.y > -0.5f){
				Translate(up() * -1);
				originalLookat = originalLookat - up();
			}
		}
	}

	Camera::Update(deltaTime_);
}