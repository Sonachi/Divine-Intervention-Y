#pragma once
#include "../Math/four_dimensions.hpp"
#include "../Math/Plane.h"
#include "../ThomasTheGameEngine/Camera.h"

class Ray
{
public:
	Ray();
	Ray(Vec3, Vec3);
	~Ray();
	Ray(Camera view_);
	Ray fromScreen();

	Vec3 point;
	Vec3 dir;
	bool castTo(Plane target_, Vec3 &output_);
};

