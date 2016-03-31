#include "Ray.h"
#include "Game.h"
#include "Level.h"
#include "InputHandler.h"
#include "GameProperties.h"
#include <iostream>


Ray::Ray()
{
	point = Vec3::Zero();
	dir = Vec3(0, 0, 1);
}

Ray::Ray(Vec3 _point, Vec3 _dir){
	point = _point;
	dir = _dir;
}

Ray Ray::fromScreen(){
	Camera* view_ = Game::GetInstance()->currentLevel->currentCamera;
	point = view_->position;

	//%age of screen away from top left in each direction
	Vec2 mouseInput = Vec2(
		Input->mousePos().x / GameProperties::getInstance()->getVideoProperties()->screenWidth,
		Input->mousePos().y / GameProperties::getInstance()->getVideoProperties()->screenHeight);

	//Place point directly on transformed frustum
	Frustrum transformed = view_->frustrum;

	Vec3 worldPos = transformed.nearTopLeft + Vec3(
		mouseInput.x * (transformed.nearBottomRight.x - transformed.nearTopLeft.x),
		mouseInput.y * (transformed.nearBottomRight.y - transformed.nearTopLeft.y),
		0.0f);

	dir = Quat::rotate(view_->rotation, worldPos);

	return Ray(point, dir);
}


Ray::~Ray()
{
}

bool Ray::castTo(Plane target_, Vec3 &output_) {
	//Ray is parrallel to plane
	if (Vec3::dot(target_.normal, dir) == 0) {
		return false;
	}

	//Calculate the negative of T in point + T * dir = plane point
	float t = (Vec3::dot(target_.normal, point) - target_.D) / Vec3::dot(target_.normal, dir);

	//If t > 0 that means -T < 0 which means dir is pointing away from plane.
	if (t > 0) { return false; }
	else {
		output_ = point + -t * dir;
	}
}