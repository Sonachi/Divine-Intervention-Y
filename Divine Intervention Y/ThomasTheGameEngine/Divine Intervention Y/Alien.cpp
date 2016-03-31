//Author: Nathan Senter
//Date: 10/03/2016

#include "Obstacles.h"
#include "PlayerBall.h"

#include <Level.h>
#include <PhysicsWorld.h>
#include <Sound.h>

Alien::Alien(Level * level_, Vec3 position_, Swarm * swarm_) : GameObject(level_, position_), swarm(swarm_)
{
	stateMachine.ChangeState(new Attack(this));
	stateMachine.ChangeState(new Idle(this));

	renderer = new Generic_RenderableComponent(this, "ufo", "white");

	Scale(Vec3(0.01f, 0.01f, 0.01f));

	shouldShoot = false;
	velocity = forward() * 0.5f;
}


Alien::~Alien()
{
}

void Alien::Update(float timeStep_)
{
	if (!player)
	{
		player = static_cast<PlayerBall *>(level->FindGameObjectWithTag("player"));
	}

	GameObject::Update(timeStep_);

	stateMachine.UpdateState();
}

void Alien::ToggleState()
{
	stateMachine.ChangeState(stateMachine.GetPreviousState());
}

#define Parent static_cast<Alien *>(self)

Idle::Idle(void * whoAmI_) : State(whoAmI_){}
Idle::~Idle(){}
void Idle::onEnter(){}
void Idle::onExit(){}

void Idle::Execute()
{
	//Orbit around swarm.position
	if ((Parent->swarm->position - Parent->position).magnitude() <= Parent->IDLE_DISTANCE)
	{
		PhysicsWorld::Orbit(Parent->swarm->position, Vec3(0, 1, 0), Parent, 0.1f);
	}
	else
	{
		//Determine the point near the swarm to reach before orbiting
		Vec3 direction_ = Parent->swarm->position + ((Parent->swarm->position - Parent->position).Normalized() * Parent->IDLE_DISTANCE);
		
		//Calculate a steering force
		Parent->steeringForce = (direction_ - Parent->position).Normalized() * 2;

		Parent->velocity = Parent->velocity + Parent->steeringForce;

		//Add velocity and steering force
		Parent->position = Parent->position + Parent->velocity.Normalized() * 0.1f;
	}

	//Avoid other ships, or not
}

Attack::Attack(void * whoAmI_) : State(whoAmI_){}
Attack::~Attack(){}
void Attack::onEnter(){}
void Attack::onExit(){}

void Attack::Execute()
{
	//chase player ship and fire at it
	if ((Parent->player->position - Parent->position).magnitude() < 2.5f)
	{
		if (Parent->shouldShoot)
		{
			if (Parent->shootSound)
			{
				Parent->shootSound->Play();
				Parent->player->getComponent<Rigidbody>()->velocity = (Parent->player->getComponent<Rigidbody>()->velocity.magnitude() * 0.99999999f) * Parent->player->getComponent<Rigidbody>()->velocity.Normalized();
			}
			Parent->shouldShoot = false;
		}

		Vec3 distance_ = Parent->position - Parent->player->position;

		Vec3 avoidDirection_ = Quat::rotate(Quat(3.14159 / 2.0f, Vec3::cross(distance_, Parent->player->position)), distance_);
		
		//Calculate a steering force
		Parent->steeringForce = (Parent->position - avoidDirection_).Normalized();

		Parent->velocity = Parent->velocity + Parent->steeringForce;

		//Add velocity and steering force
		Parent->position = Parent->position + Parent->velocity.Normalized() * 0.1f;
	}
	else
	{
		if (!Parent->shootSound)
		{
			Parent->shouldShoot = true;
		}

		//Calculate a steering force
		Parent->steeringForce = (Parent->player->position - Parent->position).Normalized();

		Parent->velocity = Parent->velocity + Parent->steeringForce;

		//Add velocity and steering force
		Parent->position = Parent->position + Parent->velocity.Normalized() * 0.1f;
	}
}
