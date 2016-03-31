#include "MockSolarSystem.h"
#include <Level.h>
#include <RenderableComponent.h>
#include <Rigidbody.h>
#include <PhysicsWorld.h>
//#include "EightBall.h"

MockSolarSystem::MockSolarSystem(Level * _level, Vec3 _position) : GameObject(_level, _position)
{
	layers.push_back(new GameObject(_level, _position + Vec3(0, -0.5f, 0)));
	new RenderableComponent("plane", "layerGrid", layers[layers.size() - 1]);
	layers[layers.size() - 1]->Scale(Vec3(15, 15, 15));
	layers[layers.size() - 1]->Rotate(Quat(-M_PI / 2.0f, Vec3(1, 0, 0)));

	layers.push_back(new GameObject(_level, _position + Vec3(0, 4.5f, 0)));
	new RenderableComponent("plane", "layerGrid", layers[layers.size() - 1]);
	layers[layers.size() - 1]->Scale(Vec3(15, 15, 15));
	layers[layers.size() - 1]->Rotate(Quat(M_PI / 2.0f, Vec3(1, 0, 0)));

	layers.push_back(new GameObject(_level, _position + Vec3(0, -4.5f, 0)));
	new RenderableComponent("plane", "layerGrid", layers[layers.size() - 1]);
	layers[layers.size() - 1]->Scale(Vec3(15, 15, 15));
	layers[layers.size() - 1]->Rotate(Quat(-M_PI / 2.0f, Vec3(1, 0, 0)));

	af = new AsteroidField(_level, _position + Vec3(-5, 4.5f, -2), 1.5f, 10);
	new AsteroidField(_level, _position + Vec3(-2, -4, -3), 3, 5);
	new AsteroidField(_level, _position + Vec3(-4, 0, 6), 3, 5);

	f = new GameObject(_level, _position + Vec3(3, 4.5, 2));
	new RenderableComponent("planet", "planetTex", f);
	f->Scale(Vec3(1.5, 1.5, 1.5));

	f2 = new GameObject(_level, _position + Vec3(5.5f, 4.5f, 2));
	f2->Scale(Vec3(0.25f, 0.25f, 0.25f));
	new RenderableComponent("planet", "planetTex1", f2);

	miner = new GameObject(_level, _position + Vec3(4, -4.5, -3));
	new RenderableComponent("planet", "minerMoon", miner);
	miner->Scale(Vec3(2, 2, 2));

	aruba = new GameObject(_level, _position + Vec3(6, -4.5, -5));
	new RenderableComponent("planet", "aruba", aruba);
	aruba->Scale(Vec3(0.4f, 0.4f, 0.4f));

	Rim = new GameObject(_level, aruba->position + Vec3(0, 0, 0.5));
	new RenderableComponent("planet", "Rim", Rim);
	Rim->Scale(Vec3(0.1f, 0.1f, 0.1f));

	aruba->addChild(Rim);
	miner->addChild(aruba);

}


MockSolarSystem::~MockSolarSystem()
{
}

void MockSolarSystem::Update(float _deltaTime)
{
	GameObject::Update(_deltaTime);

	f->Rotate(Quat(0.2f  * _deltaTime, Vec3(0, 1, 0)));
	f2->Rotate(Quat(0.8f * _deltaTime, Vec3(0, 1, 0)));
	PhysicsWorld::Orbit(f->position, Vec3(0, 1, 0), f2, 0.25 * _deltaTime);

	miner->Rotate(Quat(1.2f * _deltaTime, Vec3(0, 0.4f, 0.4f)));
	aruba->Rotate(Quat(0.2f * _deltaTime, Vec3(0.8, 0, 0.2)));
	Rim->Rotate(Quat(0.3f * _deltaTime, Vec3(0, 0.1, 0.6)));

	PhysicsWorld::Orbit(miner->position, Vec3(0.0f, 0.4f, 0), aruba, 0.22f * _deltaTime);
	PhysicsWorld::Orbit(aruba->position, Vec3(0, 0.8, 0), Rim, 0.85f * _deltaTime);
}
