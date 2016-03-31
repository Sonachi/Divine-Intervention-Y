#pragma once
#include "../Math/four_dimensions.hpp"
#include "Collider.h"

class Rigidbody :
	public Component
{
private:
	void Init();
public:
	Rigidbody(GameObject*);
	Rigidbody(GameObject* _parent, Collider* _collider);
	~Rigidbody();

	float mass;
	Vec3 centreOfMass;
	bool isKinematic;
	bool gravitas;
	Matrix3 inertiaTensor;

	//Note--Move this, should be at the collider level and not the rigidbody level.
	float CollisionRadius;

	float drag;
	float angularDrag;
	float sleepThreshold;
	float angularSleep;
	Quat AngularVelocity;
	Quat AngularAccel;
	Vec3 velocity;
	Vec3 accel;
	
	Collider* col;

	//Functions
	void AddForce(Vec3);
	void setTensorShape(Collider::ColliderType, Vec3);

	//Not implemented!
	void AddTorque(Quat); //torque in local space

	bool isAwake();
};

