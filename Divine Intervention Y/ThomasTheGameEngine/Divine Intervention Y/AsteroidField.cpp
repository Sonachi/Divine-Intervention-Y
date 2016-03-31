#include "Obstacles.h"
#include "PlayerBall.h"
#include <stdio.h>
#include <AudioManager.h>
#include <iostream>

#include "DIY_Level.h"
AsteroidField::AsteroidField(Level * _level, Vec3 _position, float _radius, float _numAsteroids) : GameObject(_level, _position)
{
	position.y += 0.5f;

	srand(rand() % 101);
	for (int i = 0; i < _numAsteroids; i++)
	{
		//float theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2 * M_PI));
		//float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / _radius));
		float theta = (rand() % (int)(2 * M_PI * 10000)) / 10000.0f;
		float r = (rand() % (int)(_radius*10000)) / 10000.0f;

		int model = rand() % 4;

		asteroids.push_back(new GameObject(_level, _position));
		asteroids[asteroids.size() - 1]->position = Vec3(asteroids[asteroids.size() - 1]->position.x + r * cos(theta), _position.y + 0.5f, asteroids[asteroids.size() - 1]->position.z + r * sin(theta));
		asteroids[asteroids.size() - 1]->Scale(Vec3(0.002, 0.002, 0.002));
		addChild(asteroids[asteroids.size() - 1]);

		switch (model)
		{
		case 0:
			new Generic_RenderableComponent(asteroids[asteroids.size() - 1], "meteor1", "meteorTex1");
			break;
		case 1:
			new Generic_RenderableComponent(asteroids[asteroids.size() - 1], "meteor2", "meteorTex2");
			break;
		case 2:
			new Generic_RenderableComponent(asteroids[asteroids.size() - 1], "meteor3", "meteorTex3");
			break;
		default:
			break;
		}
	}

	Audio->loadSound("asteroidSound", "Sounds/asteroidCollide.wav");
	collideSound = Audio->getSound("asteroidSound");

	rigidbody = new Rigidbody(this, new SphereCollider(this));
	rigidbody->CollisionRadius = _radius;
	((SphereCollider *)rigidbody->col)->collisionRadius = _radius;
	rigidbody->isKinematic = false;
	rigidbody->mass = 50.0f;

	rigidbody->setTensorShape(Collider::Sphere, Vec3(scale.x / 2.0f, scale.x / 2.0f, scale.x / 2.0f));
}


AsteroidField::~AsteroidField()
{
}

void AsteroidField::Update(float timeStep_)
{
	if (!player)
		player = (PlayerBall*)level->FindGameObjectWithTag("player");

	GameObject::Update(timeStep_);

	if ((position - player->position).length() < 0.75f)
	{
		Rigidbody * rb_ = player->getComponent<Rigidbody>();

		if (rb_->isAwake())
		{
			collideSound->Play();
			std::cout << "Hi, I collided with the ball!" << std::endl;
			//rb_->velocity = rb_->velocity.Normalized() * (rb_->velocity.length() * 0.95f);
		}
	}
}

void AsteroidField::SetRenderEnabled(bool isEnabled_)
{
	for (int i = 0; i < asteroids.size(); i++)
	{
		RenderableComponent *r = asteroids[i]->getComponent<RenderableComponent>();
		if (r)
			r->isEnabled = isEnabled_;
	}
}

bool AsteroidField::GetRenderEnabled()
{
	RenderableComponent *r = asteroids[0]->getComponent<RenderableComponent>();

	if (r)
		return r->isEnabled;
	else
		return false;
}