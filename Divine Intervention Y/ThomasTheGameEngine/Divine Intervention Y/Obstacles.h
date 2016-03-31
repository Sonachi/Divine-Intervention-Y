#pragma once

#include <GameObject.h>
#include <PhysicsWorld.h>
#include <RenderableComponent.h>
#include <States.h>

class PlayerBall;
class Sound;
class Layer;
class Planet;
class DIY_Level;

//Planet orbitting Satellite
#pragma region Satellite
class Satellite : public GameObject{
private:
	float orbitSpeed;
	Vec3 orbitAxis;
	Planet* planet;
	Rigidbody * rigidBody;
	Generic_RenderableComponent * renderer;

public:
	Satellite(DIY_Level* level_,Planet* planet_, Vec3 starticPos_, Vec3 inPlaneVector_, float orbitSpeed_, std::string modelName_, std::string textureName_);
	~Satellite();

	void Update(float _deltaTime) override;
};

#pragma endregion

//PopUp Object. Likely to be renamed
#pragma region PopUp
class PopUp : public GameObject
{
public:
	PopUp(Level *, Vec3);
	~PopUp();

	void Update(float) override;
private:
	Generic_RenderableComponent * renderer;
	PlayerBall * player;

	const float UPWARD_FORCE = 150.0f;
};

#pragma endregion

//Collidable asteroids. Act as immovable wall
#pragma region Asteroids
class AsteroidField :
	public GameObject
{
public:
	AsteroidField(Level *, Vec3, float _radius, float _numAsteroids);
	~AsteroidField();

	void Update(float timeStep_) override;

	virtual void SetRenderEnabled(bool isEnabled_) override;
	virtual bool GetRenderEnabled() override;
private:
	std::vector<GameObject *> asteroids;
	PlayerBall * player;
	Sound * collideSound;
	Rigidbody * rigidbody;
};

#pragma endregion

//Everything concerning the alien ships. Inside is the swarm host object. The alien class, as well as it's states
#pragma region Aliens
class Swarm;

class Alien : public GameObject
{
	friend class Idle;
	friend class Attack;
public:
	Alien(Level *, Vec3, Swarm *);
	~Alien();

	void Update(float) override;
	void ToggleState();
private:
	Swarm * swarm;
	PlayerBall * player;

	RenderableComponent * renderer;

	StateMachine stateMachine;

	Vec3 velocity;
	Vec3 steeringForce;

	Sound * shootSound;

	bool shouldShoot;

	const float IDLE_DISTANCE = 1.0f;
};

class Swarm : public GameObject
{
public:
	Swarm(Level *, Vec3);
	~Swarm();

	void Update(float) override;
	std::vector<Alien *> aliens;

private:
	PlayerBall * player;

	const int NUM_ALIENS = 20;
	bool inAttackMode;
};

class Idle : public State
{
public:

	Idle(void*);
	~Idle();
	void Execute() override;
	void onEnter() override;
	void onExit() override;

};

class Attack : public State
{
public:
	Attack(void*);
	~Attack();
	void Execute() override;
	void onEnter() override;
	void onExit() override;

};

#pragma endregion

//Warpgate that boosts player
#pragma region WarpGate
class WarpGate : public GameObject
{
public:
	WarpGate(Level *, Vec3, Quat);
	~WarpGate(){}

	void Update(float) override;
private:
	Generic_RenderableComponent * rc;
	PlayerBall * player;
	Sound * sound;
};

#pragma endregion

//Acts as the seguay between different layers
#pragma region Wormhole

class Wormhole : public GameObject
{
public:
	Wormhole(Level *, Vec3, int, Vec3);
	~Wormhole(){}

	Layer * getDestinationLayer();
	void Update(float) override;
private:
	Generic_RenderableComponent * rc;
	PlayerBall * player;
	DIY_Level * diyLevel;

	int initialRotation;
	float yRotation;
	int destinationLayer;
	Vec3 destinationLocation;

	bool ballCaught, isScalingDown;
	Sound * sound;
};

#pragma endregion