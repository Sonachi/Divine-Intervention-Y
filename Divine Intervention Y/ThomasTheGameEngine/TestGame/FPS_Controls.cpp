#include "FPS_Controls.h"

float FPS_Controls::FPSMoveSpeed = 10.0f;
float FPS_Controls::FPSHorizontalTurnSpeed = 2.5f;
float FPS_Controls::FPSVerticalTurnSpeed = 1.5f;

void FPS_Controls::Update(GameObject* go_, float timeStep_)
{
	//FORWARD
	if (InputController::getInstance()->isKeyDown(SDLK_w))
	{
		go_->Translate(Quat::rotate(Quat(go_->rotation.w, 0, go_->rotation.vector.y, 0).NormalizeThis(), Vec3(0, 0, FPSMoveSpeed) * timeStep_));
	}

	//BACKWARDS
	if (InputController::getInstance()->isKeyDown(SDLK_s))
	{
		go_->Translate(Quat::rotate(Quat(go_->rotation.w, 0, go_->rotation.vector.y, 0).NormalizeThis(), Vec3(0, 0, -FPSMoveSpeed) * timeStep_));
	}

	//STRAFE LEFT
	if (InputController::getInstance()->isKeyDown(SDLK_a))
	{
		go_->Translate(Quat::rotate(Quat(go_->rotation.w, 0, go_->rotation.vector.y, 0).NormalizeThis(), Vec3(FPSMoveSpeed, 0, 0) * timeStep_));
	}

	//STRAFE RIGHT
	if (InputController::getInstance()->isKeyDown(SDLK_d))
	{
		go_->Translate(Quat::rotate(Quat(go_->rotation.w, 0, go_->rotation.vector.y, 0).NormalizeThis(), Vec3(-FPSMoveSpeed, 0, 0) * timeStep_));
	}

	//LOOK UP / DOWN
	{
		go_->Rotate(Quat(InputController::getInstance()->deltaMouse().y  * -FPSVerticalTurnSpeed * timeStep_ * Vec3::cross(go_->forward(), go_->up()).length(), Vec3::cross(go_->forward(), go_->up())));
	}

	//TURN LEFT / RIGHT
	{
		go_->Rotate(Quat(InputController::getInstance()->deltaMouse().x * -FPSHorizontalTurnSpeed * timeStep_, Vec3::BasisY()));
	}
}