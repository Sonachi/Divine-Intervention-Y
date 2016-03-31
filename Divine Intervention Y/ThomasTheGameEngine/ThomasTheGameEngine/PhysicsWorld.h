#pragma once
#include "GameObject.h"
#include "Rigidbody.h"

#define Physics PhysicsWorld::getInstance()

class PhysicsWorld
{
	PhysicsWorld();
	float lastTimeStep;
public:
	static PhysicsWorld * instance;
	static PhysicsWorld * getInstance(){
		if (!instance)instance = new PhysicsWorld();
		return instance;
	}
	~PhysicsWorld();
	
	std::vector<Rigidbody*> PhysicalObjects;

	//Easiest to think of in regular gravity units (Newtons or KG * M / S^2) whatever works though earth standard is Vec3(0,-9.8,0)
	Vec3 worldGravity;

	//Pause physics world
	static bool isPhysicsRunning;

	//You can think of this as rotateAround if you'd like
	void static Orbit(Vec3 centre, Vec3 axis, GameObject* object, float angle);
	void static Orbit(Rigidbody* mover_, GameObject* centre);
	//void static Impulse(GameObject*, GameObject*);
	void static Impulse(Rigidbody*, Rigidbody*);
	float getTimeStep();
	void Update(float);
};

