#include "ObservatoryRoom.h"
#include <Level.h>
#include <RenderableComponent.h>
#include <AudioManager.h>
#include "Player.h"
#include <Light.h>
#include <cmath>
#include <PhysicsWorld.h>
#include <Flipbook.h>

ObservatoryRoom::ObservatoryRoom(Level * _level, Vec3 _position, Player * _player) : GameObject(_level, _position)
{
	Tags.push_back("observatory");

	skyBox = new RenderableComponent("skybox", "space", this);
	observatoryMusic = AudioManager::getInstance()->getMusic("observatory");
	observatoryMusic->Play();

	player = _player;

	abs = new AmbientStar(_level, position + Vec3(0, 9.5f, 4));
	new AmbientStar(_level, position + Vec3(-5, -9.5f, 2.5));
	new AmbientStar(_level, position + Vec3(6, 9.5, -5));
	new AmbientStar(_level, position + Vec3(9.5, 3, 7));
	new AmbientStar(_level, position + Vec3(9.5, -7, -2));
	new AmbientStar(_level, position + Vec3(-9.5, -4, -1));
	new AmbientStar(_level, position + Vec3(2, -1, 9.5));

	sun = new GameObject(_level, position);
	sun->Scale(Vec3(4, 4, 4));
	RenderableComponent * r = new RenderableComponent("planet2", "sunTex", sun);
	r->SetEffecctedByLight(false, false, false);
	new Light(sun, Colour(100, 100, 100), Light::Point);
	/*Flipbook * fb = new Flipbook(sun, 27, "Images/Animation/slice0.png", 0.5f, true, Flipbook::PNG);
	fb->SetEffecctedByLight(false, false, false);
	fb->Play();*/

	solarSystem = new MockSolarSystem(_level, position);
}


ObservatoryRoom::~ObservatoryRoom()
{
}

void ObservatoryRoom::Update(float _deltaTime)
{
	GameObject::Update(_deltaTime);

	Vec3 distance = Vec3(position.x - player->position.x, 0, position.z - player->position.z);

	float volumeScale;

	volumeScale = 1 / (distance.magnitude() * distance.magnitude() / 20.0f);

	AudioManager::getInstance()->setMusicVolume(volumeScale);

	sun->Rotate(Quat(0.08f * _deltaTime, Vec3(0, 1, 0)));

	PhysicsWorld::Orbit(sun->position, Vec3(1, 1, 1), solarSystem->miner, 0.0008f);
}