#pragma once
#include "four_dimensions.hpp"
#include <vector>

class Plane
{
private:
	//Incomplete
	void sortBounds(Vec3 planeCentre_);
public:
	Vec3 normal;
	float D;
	std::vector<Vec3> bounds;

	Plane(){}
	Plane(Vec3 _normal, float _d);
	Plane(Vec3 normal_, Vec3 point_);
	Plane(Vec3 _Pa, Vec3 _Pb, Vec3 _Pc);
	~Plane();

	Vec4 getPlane();
	Vec3 getNormal();
	float getDistance();

	float DistanceToPoint(Vec3 _point);
	Vec3 projectPoint(Vec3 point_);
	void addBoundingPoints(Vec3 point_);

};

