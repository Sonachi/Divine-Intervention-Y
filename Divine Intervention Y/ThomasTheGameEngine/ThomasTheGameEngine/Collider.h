#pragma once

#include "Component.h"
#include "../Math/Plane.h"
#include <vector>

class SphereCollider;
class PlaneCollider;
class CubeCollider;

class Collider :
	public Component{
protected:
public:
	enum ColliderType{
		Sphere, BoundedPlane, Cube
	};
	std::vector<Collider> nestedColliders;
	Vec3 colliderCentre;
	ColliderType type;

	static bool isColliding(Collider*, Collider*);

	//for N types of colliders need N^2 functions. This should clean up code and make future implementation better

	static bool isColliding(SphereCollider*, SphereCollider*);
	static bool isColliding(SphereCollider*, PlaneCollider*);
	static bool isColliding(SphereCollider*, CubeCollider*);
	static bool isColliding(PlaneCollider*, SphereCollider*);
	static bool isColliding(PlaneCollider*, PlaneCollider*);
	static bool isColliding(PlaneCollider*, CubeCollider*);
	static bool isColliding(CubeCollider*, SphereCollider*);
	static bool isColliding(CubeCollider*, PlaneCollider*);
	static bool isColliding(CubeCollider*, CubeCollider*);

	virtual void OnColliding(Collider* other){}

	Collider(GameObject*);
	~Collider(){}
};

class SphereCollider :
	public Collider{
public:
	SphereCollider(GameObject* _parent);

	float collisionRadius;
};

class PlaneCollider :
	public Collider{
public:
	PlaneCollider(GameObject * parent_, Vec3 normal_);
	Plane plane;
};

class CubeCollider :
	public Collider{
public:
	//Just a thick plane?
	float depth;
};