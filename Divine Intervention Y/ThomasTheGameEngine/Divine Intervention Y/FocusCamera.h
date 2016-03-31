#pragma once

#include <Camera.h>
#include <States.h>

class FocusCamera : public Camera{
	GameObject * focus;
	float followDistance;
	float MaxCameraDistance, MinCameraDistance;
	enum LookState{
		Stare,
		Peek,
		Orbit,
		Pan
	};

	Vec3 selfieStick;

	//these two should be either 1 or -1 for mouse movement inversion
	int xInverted, yInverted;

	//more camera control numbers.
	float lookSpeed;
	

	LookState behaviour;
	friend class Stare;
	friend class Peek;
	friend class Orbit;

public:
	FocusCamera(Level * level_, GameObject * focus_, Vec3 position_);
	~FocusCamera();

	StateMachine stateMachine;

	void Update(float deltatime_) override;

	GameObject * getFocus();
	void ChangeFocus(GameObject* newFocus_);
	void ChangeDistance(float newFollow_);
	void SetMaxDistance(float maxDistance_);
	void SetMinDistance(float minDistance_);
};

//States for the state machine.-------------------------------------------
class Stare : public State {
public:
	Stare(void*);
	~Stare();
	void Execute() override;
	void onEnter() override;
	void onExit() override;
};

class Peek : public State {
private:
	Quat beginningOrientation;
public:
	Peek(void*);
	~Peek();
	void Execute() override;
	void onEnter() override;
	void onExit() override;
};

class Orbit : public State {
public:
	Orbit(void*);
	~Orbit();
	void Execute() override;
	void onEnter() override;
	void onExit() override;
};