#include "Frustrum.h"
#include "Camera.h"
#include "GameProperties.h"
#include "Game.h"
#include "../Math/Plane.h"

Frustrum::Frustrum()
{
	float zFar = GameProperties::getVideoProperties()->drawDistance;
	float znear = 0.1f;
	float fov = 45.0f;
	float aspectRatio = GameProperties::getVideoProperties()->aspectRatio();

	nearBottomRight = Vec3(znear * tan(fov * aspectRatio / 2.0f), znear * -tan(fov / 2.0f), znear);
	nearTopLeft = Vec3(znear * -tan(fov * aspectRatio / 2.0f), znear * tan(fov / 2.0f), znear);

	farBottomRight = Vec3(zFar * tan(fov * aspectRatio / 2.0f), zFar * -tan(fov / 2.0f), zFar);
	farTopLeft = Vec3(zFar * -tan(fov * aspectRatio / 2.0f), zFar * tan(fov / 2.0f), zFar);
}

Frustrum::~Frustrum()
{

}

Frustrum Frustrum::getTransofmedFrustrum(Camera * _camera)
{
	Frustrum f;

	f.nearBottomRight = Vec4::DownCast(_camera->toMat4() * Vec4::HomoVec3(f.nearBottomRight));
	f.nearTopLeft = Vec4::DownCast(_camera->toMat4() * Vec4::HomoVec3(f.nearTopLeft));

	f.farBottomRight = Vec4::DownCast(_camera->toMat4() * Vec4::HomoVec3(f.farBottomRight));
	f.farTopLeft = Vec4::DownCast(_camera->toMat4() * Vec4::HomoVec3(f.farTopLeft));

	return f;
}

void Recalculate()
{
	Frustrum();
}

bool Frustrum::checkObjectInside(GameObject * _go, Camera * _c)
{
	Frustrum f = getTransofmedFrustrum(_c);

	//Near Plane
	Plane p = Plane(Vec3(f.nearTopLeft.x, f.nearBottomRight.y, f.nearBottomRight.z), f.nearTopLeft, f.nearBottomRight);
	if (p.DistanceToPoint(_go->position) < 0) 
		return false;

	//Far Plane
	p = Plane(Vec3(f.farTopLeft.x, f.farBottomRight.y, f.farBottomRight.z), f.farTopLeft, f.farBottomRight);
	if (p.DistanceToPoint(_go->position) > 0) 
		return false;

	//Left Plane
	p = Plane(Vec3(f.farTopLeft.x, f.farBottomRight.y, f.farBottomRight.z),f.farTopLeft, f.nearTopLeft); //Vec3(f.nearTopLeft.x, f.nearBottomRight.y, f.nearBottomRight.z)
	if (p.DistanceToPoint(_go->position) < 0) 
		return false;

	//Right Plane
	p = Plane(f.nearBottomRight, Vec3(f.nearBottomRight.x, f.nearTopLeft.y, f.nearBottomRight.z), f.farBottomRight);
	if (p.DistanceToPoint(_go->position) < 0) 
		return false;

	//Top Plane
	p = Plane(f.nearTopLeft, f.farTopLeft, Vec3(f.nearBottomRight.x, f.nearTopLeft.y, f.nearBottomRight.z));
	if (p.DistanceToPoint(_go->position) < 0) 
		return false;

	//Bottom Plane
	p = Plane(Vec3(f.farTopLeft.x, f.farBottomRight.y, f.farBottomRight.z), Vec3(f.nearTopLeft.x, f.nearBottomRight.y, f.nearBottomRight.z), f.farBottomRight);
	if (p.DistanceToPoint(_go->position) < 0) 
		return false;

	//If youve made it this far, you're inside
	//CONGRATULATIONS!
	return true;
}