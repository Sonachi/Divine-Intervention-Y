#include "Plane.h"


Plane::Plane(Vec3 _normal, float _d)
{
	normal = _normal;
	D = _d;
}

Plane::Plane(Vec3 _Pa, Vec3 _Pb, Vec3 _Pc)
{
	normal = Vec3::cross((_Pb - _Pa), (_Pc - _Pa));
	D = Vec3::dot(normal, _Pa);
}

Plane::Plane(Vec3 normal_, Vec3 point_){
	normal = normal_;
	D = Vec3::dot(normal_, point_);
}

Plane::~Plane()
{
}

float Plane::DistanceToPoint(Vec3 _point)
{
	return (Vec3::dot(normal, _point) - D);
}

Vec4 Plane::getPlane()
{
	return Vec4(D, normal.x, normal.y, normal.z);
}

Vec3 Plane::getNormal()
{
	return normal;
}

float Plane::getDistance()
{
	return D;
}

//Vec3 Plane::projectPoint(Vec3 point_){
//	return (point_ + normal * DistanceToPoint(point_));
//}
//
//void Plane::addBoundingPoints(Vec3 point_){
//	//project point into plane, then add to list
//	bounds.push_back(projectPoint(point_));
//}
//
//void Plane::sortBounds(Vec3 planeCentre_){
//	//start with [0], compare to [1]
//	//insertion sort compare each to the next for positive/negative
//
//	for (int i = 1; i < bounds.size(); i++){
//		int r = i;
//		Vec3 lineA = bounds[i - 1] - planeCentre_;
//		Vec3 lineB = bounds[i] - planeCentre_;
//		while (r >= 1 && Vec3::dot(lineA, lineB) < 0){
//			Vec3 tempBound = bounds[r];
//			bounds[r] = bounds[r - 1];
//			bounds[r - 1] = bounds[r];
//
//			r--;
//			lineA = bounds[r - 1] - planeCentre_;
//			lineB = bounds[r] - planeCentre_;
//		}
//	}
//
//}