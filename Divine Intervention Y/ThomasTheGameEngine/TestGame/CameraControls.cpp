#include "CameraControls.h"
#include "GameCamera.h"
#include "GameLevel.h"

CameraControls::CameraControls()
{
}


CameraControls::~CameraControls()
{
}

Camera_Forward::Camera_Forward(GameObject * owner_, SDL_Keycode key_) : InputComponent(owner_, key_)
{
	owner = static_cast<GameCamera *>(owner_);
}

void Camera_Forward::whenPressed(float timeStep_)
{
	if (!owner->mouseDown)
	{
		owner->Translate(Vec3(0, 0, 1) * timeStep_);
	}
}

Camera_Back::Camera_Back(GameObject * owner_, SDL_Keycode key_) : InputComponent(owner_, key_)
{
	owner = static_cast<GameCamera *>(owner_);
}

void Camera_Back::whenPressed(float timeStep_)
{
	if (!owner->mouseDown)
	{
		owner->Translate(Vec3(0, 0, -1) * timeStep_);
	}
}

Camera_Right::Camera_Right(GameObject * owner_, SDL_Keycode key_) : InputComponent(owner_, key_)
{
	owner = static_cast<GameCamera *>(owner_);
}

void Camera_Right::whenPressed(float timeStep_)
{
	if (!owner->mouseDown)
	{
		owner->Translate(Vec3(-1, 0, 0) * timeStep_);
	}
}

Camera_Left::Camera_Left(GameObject * owner_, SDL_Keycode key_) : InputComponent(owner_, key_)
{
	owner = static_cast<GameCamera *>(owner_);
}

void Camera_Left::whenPressed(float timeStep_)
{
	if (!owner->mouseDown)
	{
		owner->Translate(Vec3(1, 0, 0) * timeStep_);
	}
}

LeftClick::LeftClick(GameObject * owner_, SDL_Keycode key_) : InputComponent(owner_, key_)
{
	owner = static_cast<GameCamera *>(owner_);
	isSet = false;
}

void LeftClick::whenPressed(float timeStep_)
{
	if (!isSet)
	{
		owner->originalLookat = owner->forward() + owner->position;
		owner->originalRotation = owner->rotation;
		isSet = true;
	}
	owner->mouseDown = true;
}

void LeftClick::whenReleased()
{
	isSet = false;
	owner->mouseDown = false;
}

RightClick::RightClick(GameObject * owner_, SDL_Keycode key_) : InputComponent(owner_, key_)
{
	gl = static_cast<GameLevel *>(owner_->level);
}

void RightClick::whenPressed(float timeStep_)
{
	gl->rotateLevel = true;
}

void RightClick::whenReleased()
{
	gl->rotateLevel = false;
}

Look_Up::Look_Up(GameObject * owner_, MouseMovement e_) : InputComponent(owner_, e_)
{
	owner = static_cast<GameCamera *>(owner_);
}

void Look_Up::whenPressed(float timeStep_)
{
	if (owner->mouseDown)
		owner->Rotate(Quat(2 * timeStep_ * Vec3::cross(owner->forward(), owner->up()).length(), Vec3::cross(owner->forward(), owner->up())));
}

Look_Down::Look_Down(GameObject * owner_, MouseMovement e_) : InputComponent(owner_, e_)
{
	owner = static_cast<GameCamera *>(owner_);
}

void Look_Down::whenPressed(float timeStep_)
{
	if (owner->mouseDown)
		owner->Rotate(Quat(-2 * timeStep_ * Vec3::cross(owner->forward(), owner->up()).length(), Vec3::cross(owner->forward(), owner->up())));
}

Look_Right::Look_Right(GameObject * owner_, MouseMovement e_) : InputComponent(owner_, e_)
{
	owner = static_cast<GameCamera *>(owner_);
	gl = static_cast<GameLevel *>(owner_->level);
}

void Look_Right::whenPressed(float timeStep_)
{
	if (owner->mouseDown)
		owner->Rotate(Quat(2 * timeStep_, Vec3(0, 1, 0)));
	else if (gl->rotateLevel)
		gl->layerContainer->Rotate(Quat(1 * timeStep_, Vec3(0, 1, 0)));
}

Look_Left::Look_Left(GameObject * owner_, MouseMovement e_) : InputComponent(owner_, e_)
{
	owner = static_cast<GameCamera *>(owner_);
	gl = static_cast<GameLevel *>(owner_->level);
}

void Look_Left::whenPressed(float timeStep_)
{
	if (owner->mouseDown)
		owner->Rotate(Quat(-2 * timeStep_, Vec3(0, 1, 0)));
	else if (gl->rotateLevel)
		gl->layerContainer->Rotate(Quat(-1 * timeStep_, Vec3(0, 1, 0)));
}