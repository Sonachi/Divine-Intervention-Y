#pragma once
#include "../Math/four_dimensions.hpp"

class GameObject;
class Camera;

class Frustrum
{
public:
	Frustrum();
	~Frustrum();

	//Pass in a game object and this method will return true if the object is within the frustrum
	bool checkObjectInside(GameObject * _go, Camera * _c);

	//Returns a copy of the frustrum but matched to the current cameras transform
	static Frustrum getTransofmedFrustrum(Camera * camera);

	//Called when video properties change, will recalculate the size of the frustrum
	void Recalculate();

	Vec3 nearBottomRight, nearTopLeft;
	Vec3 farBottomRight, farTopLeft;
private:
	//For memory saving purposes we are only storing enough points to create the near and far plane
	//All other planes can be constructed using just these points
};