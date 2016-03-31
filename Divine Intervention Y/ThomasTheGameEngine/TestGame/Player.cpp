#include "Player.h"
#include <Level.h>
#include <Camera.h>
#include <Light.h>
#include <ParticleSystem.h>
#include <PhysicsWorld.h>

Player::Player(Level * _level, Camera * _camera) : GameObject(_level)
{
	fpsCamera = _camera;

	position = fpsCamera->position;
	rotation = fpsCamera->rotation;
	addChild(fpsCamera);

	t = 0;

	lightHolder = new GameObject(_level, position + Vec3(0, 1, -1.2));
	lightHolder->LookAt(Vec3(0, 1, 0));
	lightHolder->Rotate(Quat(20 * 3.14159f / 180.0f, Vec3::BasisX()));
	//flashLight = new Light(lightHolder, Colour(200, 200, 200), Light::Spot, 60 * 3.14159f / 180.0f);
	fpsCamera->addChild(lightHolder);

	feetMagic = new ParticleSystem(this, ParticleSystem::Emitter_Type_None, "plane", "footprint", 50, 0.7f, 3.f, Particle::Object3D);
	feetMagic->Stop();

	SDL_SetRelativeMouseMode(SDL_TRUE); //Traps Mouse in Window (centre)

	/*Rigidbody * rb = new Rigidbody(fpsCamera);
	rb->gravitas = false;
	rb->mass = 10;
	rb->CollisionRadius = 1;*/
}


Player::~Player()
{
}

void Player::Update(float _deltaTime)
{
	GameObject::Update(_deltaTime);

	if (previousPosition != position)
	{
		t += 6.0f * _deltaTime;
		fpsCamera->position.y = position.y + sin(t) / 25.0f;

		previousPosition = position;
	}

	GameObject * room = level->FindGameObjectWithTag("observatory");

	//if ((room->position - position).magnitude() < 10)
	if (Vec3(position.x - room->position.x, 0, position.z - room->position.z).magnitude() < 10)
	{
		if (!feetMagic->isPlaying())
			feetMagic->Play();

		feetMagic->SetPositionOffset(Vec3(0, -.49f, 0));
		Quat _feetRot = Quat(fpsCamera->rotation.w, 0, fpsCamera->rotation.vector.y, 0).NormalizeThis(); //Gets Y component of player
		feetMagic->SetInitialRotation(_feetRot * Quat(-M_PI / 2.0f, Vec3(1, 0, 0)));
		
		feetMagic->SetInitialScale(Vec3(0.15f, 0.2f, 0.2f));		
	}
	else
	{
		if (feetMagic->isPlaying())
			feetMagic->Stop();
	}
}
