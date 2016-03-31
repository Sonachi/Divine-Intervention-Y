#include "Sun.h"
#include <Colour.h>
#include <PhysicsWorld.h>
#include <Level.h>
#include <AudioManager.h>

#include "PlayerBall.h"
#include "DIY_Level.h"
#include <InputHandler.h>
#include <iostream>
#include <Random.h>
#include <Game.h>

Sun::Sun(Level * level_, Vec3 position_, float radius_) : GameObject(level_, position_), ballHitIntoSun(false)
{
	renderer = new Sun_RenderableComponent(this, "sun");
	renderer->intensity = (MINIMUM_INTENSITY + MAXIMUM_INTENSITY) / 2.0f;
	renderer->coreColour = Colour(1.5f, 1.5f, 0, 0);
	renderer->rippleColour = Colour(8.0f, 0.4f, 0, 0);
	Scale(Vec3::One() * radius_);

	glow = new GameObject(level_, position);
	glow->Scale(Vec3::One() * radius_ * 1.34f);
	addChild(glow);

	glow_renderer = new Glow_RenderableComponent(glow, "sun");
	glow_renderer->glowColour = Colour(1.0f, 0.5f, 0.0f, 0) * 2;

	light = new Light(this, Colour(1, 1, 1), Light::Point);

	rigidBody = new Rigidbody(this, new SphereCollider(this));
	rigidBody->isKinematic = false;
	static_cast<SphereCollider *>(rigidBody->col)->collisionRadius = 1.5f;
	
	Audio->loadSound("whoosh", "Sounds/firewhoosh.wav");
}


Sun::~Sun()
{
}

void Sun::Update(float timeStep_)
{
	if (!player)
	{
		player = static_cast<PlayerBall *>(level->FindGameObjectWithTag("player"));
		playerRigidBody = player->getComponent<Rigidbody>();
	}

	//if (InputController::getInstance()->isKeyReleased(SDLK_KP_PLUS))
	//{
	//	
	//}
	//else if (InputController::getInstance()->isKeyReleased(SDLK_KP_MINUS))
	//{
	//	
	//}

	renderer->intensity += Random::box_muller(0, INTENSITY_INCREMENT) * timeStep_;
	renderer->intensity = renderer->intensity > MAXIMUM_INTENSITY ? MAXIMUM_INTENSITY : renderer->intensity < MINIMUM_INTENSITY ? MINIMUM_INTENSITY: renderer->intensity;
	//std::cout << "Sun Intensity: " << renderer->intensity << "\n";
	
	renderer->offset -= Vec3(0.1f, 0.01f, 0.25f) * timeStep_;

	Vec3 toCam = (GAME->currentLevel->currentCamera->position - position).Normalized();
	renderer->offset += toCam * 0.25f * timeStep_;// (0.1f, 0.01f, 0.25f) * timeStep_;
	glow_renderer->offset += toCam * 0.25f * timeStep_;

	Rotate(Quat(0.05f * timeStep_, Vec3(0, 1, 0)));
}