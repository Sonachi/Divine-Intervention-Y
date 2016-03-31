// Author: Nathan Senter
// I forgot the original date. The new one is 05/20/2015

#include "GameObject.h"
#include "Component.h"
#include "RenderableComponent.h"
#include "Light.h"
#include <iostream>
#include "LightManager.h"
#include "Level.h"
#include "ParticleSystem.h"
#include "Flipbook.h"
#include "Rigidbody.h"
#include "PhysicsWorld.h"

void GameObject::Translate(Vec3 _translate)
{
	if (!PhysicsWorld::getInstance()->isPhysicsRunning)
		return;

	position += _translate;
	for (auto it = childObjects.begin(); it != childObjects.end(); it++){
		(*it)->Translate(_translate);
	}
}

void GameObject::Scale(Vec3 _scale)
{
	if (!PhysicsWorld::getInstance()->isPhysicsRunning)
		return;

	scale = Vec3(scale.x * _scale.x, scale.y * _scale.y, scale.z * _scale.z);
	for (auto it = childObjects.begin(); it != childObjects.end(); it++){
		(*it)->Scale(_scale);
	}
}


void GameObject::Rotate(Quat _rotation)
{
	if (!PhysicsWorld::getInstance()->isPhysicsRunning)
		return;

	_rotation = _rotation.NormalizeThis();

	//If problem exists, change order, internet says it should be fine
	rotation = _rotation * rotation;
	for (auto it = childObjects.begin(); it != childObjects.end(); it++){
		//Children don't quite rotate at the right pace.
		(*it)->Rotate(_rotation);
		Vec3 newPos = (*it)->position - position;
		if (newPos.length() > 0)
			(*it)->Translate((Quat::rotate(_rotation, newPos.Normalized()) * newPos.magnitude() + position) - (*it)->position);
	}
}

void GameObject::RotateAround(Vec3 centre_, Vec3 axis_, float angle_){
	PhysicsWorld::Orbit(centre_, axis_, this, angle_);
	Rotate(Quat(angle_, axis_));
}

//Rotates in the X-Y-Z plane (in that order) use Radians, if possible use the Rotate(Quat) method instead
void GameObject::Rotate(Vec3 _rotation)
{
	if (!PhysicsWorld::getInstance()->isPhysicsRunning)
		return;

	Quat _rot = (Quat(_rotation.x, Vec3::BasisX()) * Quat(_rotation.y, Vec3::BasisY()) * Quat(_rotation.z, Vec3::BasisZ())).NormalizeThis();

	rotation = rotation * _rot;

	for (auto it = childObjects.begin(); it != childObjects.end(); it++){
		//Children don't quite rotate at the right pace.
		(*it)->Rotate(_rot);
		Vec3 newPos = (*it)->position - position;
		(*it)->Translate((Quat::rotate(_rot, newPos.Normalized()) * newPos.magnitude() + position) - (*it)->position);
	}
}

Matrix4 GameObject::toMat4() {
	return Matrix4::Translate(position.x, position.y, position.z) * Matrix4::Rotate(rotation) * Matrix4::Scale(scale.x, scale.y, scale.z);
}

Quat GameObject::getQuat(){
	return rotation;
}

//Should not be using euler angles, included for completness
Vec3 GameObject::getEuler(){
	return Vec3(
		atan(2 * (rotation.w * rotation.vector.x + rotation.vector.y * rotation.vector.z) / (rotation.w * rotation.w - rotation.vector.x * rotation.vector.x - rotation.vector.y * rotation.vector.y + rotation.vector.z * rotation.vector.z)),
		-asin(2 * (rotation.vector.x * rotation.vector.z - rotation.w * rotation.vector.y)),
		atan(2 * (rotation.w * rotation.vector.z + rotation.vector.x * rotation.vector.y) / (rotation.w * rotation.w + rotation.vector.x * rotation.vector.x - rotation.vector.y * rotation.vector.y - rotation.vector.z * rotation.vector.z))
		);
}

Vec3 GameObject::forward(){
	return Quat::rotate(rotation, Vec3::BasisZ());
}

Vec3 GameObject::up() {
	return Quat::rotate(rotation, Vec3::BasisY());
}

Vec3 GameObject::right(){
	return Quat::rotate(rotation, Vec3::BasisX());
}

void GameObject::LookAt(Vec3 _target)
{
	if (!PhysicsWorld::getInstance()->isPhysicsRunning)
		return;

	Vec3 pointToTarget = (_target - position).Normalized();
	if (acos(Vec3::dot(forward(), pointToTarget)) < 0.00002f || acos(Vec3::dot(forward(), pointToTarget) > 0.99998f)){
		Rotate(Quat(acos(Vec3::dot(forward(), pointToTarget)), Vec3::cross(forward(), pointToTarget)));
	}
}


GameObject::GameObject(Level * _level) : position(Vec3::Zero()), scale(Vec3(1, 1, 1)), rotation(Quat(1, 0, 0, 0))
{
	isFlagged = false;
	level = _level;

	if (level)
		level->gameObjects.push_back(this);
}

GameObject::GameObject(Level * _level, Vec3 _position) : position(_position), scale(Vec3(1, 1, 1)), rotation(Quat(1, 0, 0, 0))
{
	isFlagged = false;
	level = _level;

	if (level)
		level->gameObjects.push_back(this);
}


GameObject::~GameObject()
{
	//std::cout << "DELETING GAME OBJECT! Number of Components: " << components.size() << " \n";

	//Clear all components
	if (components.size() > 1)
	{
		int t = 0;
	}

	for (auto it = components.begin(); it != components.end(); it++)
	{
		switch ((*it)->type)
		{
		case Component::ComponentType::Rigidbody:
			std::cout << "----DELETING Rigidbody...";
			delete (Rigidbody*)*it;
			std::cout << " done.\n";
			break;
		case Component::ComponentType::Collision:
			std::cout << "----DELETING Collider...";
			delete (Collider*)*it;
			std::cout << " done.\n";
			break;
		case Component::ComponentType::Renderable_Component:
			std::cout << "----DELETING RenderableComponent...";
			delete (RenderableComponent*)*it;
			std::cout << " done.\n";
			break;
		case Component::ComponentType::Flipbook:
			std::cout << "----DELETING Flipbook...";
			delete (Flipbook*)*it;
			std::cout << " done.\n";
			break;
		case Component::ComponentType::Light:
			std::cout << "----DELETING Light...";
			delete (Light*)*it;
			std::cout << " done.\n";
			break;
		case Component::ComponentType::ParticleSystem:
			std::cout << "----DELETING Particle System...";
			delete (ParticleSystem*)*it;
			std::cout << " done.\n";
			break;
		default:
			std::cout << "----DELETING Component?...";
			delete *it;
			std::cout << " done.\n";
			break;
		}
	}
	components.clear();
}

GameObject& GameObject::GetTransform()
{
	return *this;
}


/* Removes parameter component from list of components and deletes it from memory */
void GameObject::removeComponent(Component * _c)
{
	for (auto it = components.begin(); it != components.end(); it++)
	{
		if (*it._Ptr == _c)
		{
			components.erase(it);

			switch (_c->type)
			{
			case Component::ComponentType::Rigidbody:
				delete (Rigidbody*)_c;
				break;
			default:
				delete _c;
				break;
			}

			break;
		}
	}
}

void GameObject::addTag(Tag _tag)
{
	Tags.push_back(_tag);
}

/* Attaches passed game object to this game object. */
void GameObject::addChild(GameObject * _object)
{
	childObjects.push_back(_object);
}

void GameObject::removeChild(GameObject * _object)
{
	for (auto it = childObjects.begin(); it != childObjects.end(); it++)
	{
		if (*it._Ptr == _object)
		{
			childObjects.erase(it);
			break;
		}
	}
}

/* Returns true if the game objects list of tags contains the given tag. */
bool GameObject::hasTag(Tag _tag)
{
	for (int i = 0; i < Tags.size(); i++)
	{
		if (Tags[i] == _tag)
			return true;
	}
	return false;
}

void GameObject::Destroy()
{
	isFlagged = true;
}

/* Returns a pointer to the first component found of the given type. */
template<>
RenderableComponent* GameObject::getComponent()
{
	for (int i = 0; i < components.size(); i++)
	{
		if ((*components[i]).type == Component::ComponentType::Renderable_Component)
			return (RenderableComponent*)components[i];
	}
	return nullptr;
}

template<>
Flipbook* GameObject::getComponent()
{
	for (int i = 0; i < components.size(); i++)
	{
		if ((*components[i]).type == Component::ComponentType::Flipbook)
			return (Flipbook*)components[i];
	}
	return nullptr;
}

template<>
Rigidbody* GameObject::getComponent()
{
	for (int i = 0; i < components.size(); i++)
	{
		if ((*components[i]).type == Component::ComponentType::Rigidbody)
			return (Rigidbody*)components[i];
	}
	return nullptr;
}

template<>
Collider* GameObject::getComponent()
{
	for (int i = 0; i < components.size(); i++)
	{
		if ((*components[i]).type == Component::ComponentType::Collision)
			return (Collider*)components[i];
	}
	return nullptr;
}

template<>
ParticleSystem* GameObject::getComponent()
{
	for (int i = 0; i < components.size(); i++)
	{
		if ((*components[i]).type == Component::ComponentType::ParticleSystem)
			return (ParticleSystem*)components[i];
	}
	return nullptr;
}

template<>
Light* GameObject::getComponent()
{
	for (int i = 0; i < components.size(); i++)
	{
		if ((*components[i]).type == Component::ComponentType::Light)
			return (Light*)components[i];
	}
	return nullptr;
}

template<class TYPE>
TYPE* GameObject::getComponent()
{
	for (int i = 0; i < components.size(); i++)
	{
		if (typeid(*components[i]).name() == typeid(TYPE).name())
			return (TYPE*)components[i];
	}
	return nullptr;
}

void GameObject::Update(float _deltaTime)
{
	ParticleSystem* particles = getComponent<ParticleSystem>();

	if (particles)
		particles->UpdateParticles(_deltaTime);

	Flipbook * flipbook = getComponent<Flipbook>();

	if (flipbook)
		flipbook->UpdateFlipbook(_deltaTime);
}

void GameObject::Render()
{
	RenderableComponent* renderable = getComponent<RenderableComponent>();

	if (renderable)
	{
		renderable->Render();
	}

	ParticleSystem * particles = getComponent<ParticleSystem>();
	if (particles)
		particles->RenderParticles();
}

void GameObject::PreRender()
{
	Light * light = getComponent<Light>();

	if (light)
	{
		light->PushLight();
	}
}

void GameObject::SetRenderEnabled(bool isEnabled_)
{
	RenderableComponent* rend = getComponent<RenderableComponent>();
	if (rend)
		rend->isEnabled = isEnabled_;
}

bool GameObject::GetRenderEnabled()
{
	RenderableComponent* rend = getComponent<RenderableComponent>();
	if (rend)
		return rend->isEnabled;
	else
		return false;
}