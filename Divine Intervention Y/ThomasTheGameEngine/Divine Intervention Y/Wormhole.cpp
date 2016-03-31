#include <AudioManager.h>

#include "Obstacles.h"

#include "DIY_Level.h"
#include "PlayerBall.h"

Wormhole::Wormhole(Level * level_, Vec3 position_, int layerIndex, Vec3 destinationLocation_) : GameObject(level_, position_), yRotation(0)
{
	destinationLayer = layerIndex;
	diyLevel = static_cast<DIY_Level *>(level);

	Scale(Vec3(2, 2, 2));

	rc = new Generic_RenderableComponent(this, "wormhole", "wormholeTexture");

	ballCaught = false;
	isScalingDown = true;
	//drawStyle = DRAW_STYLE::RAINBOW_STYLE;

	initialRotation = (rand() % 366);
	Rotate(Quat(initialRotation * M_PI / 180.0f, Vec3::BasisY()));

	destinationLocation = destinationLocation_;

	Audio->loadSound("wormhole", "Sounds/wormhole.wav");
	sound = Audio->getSound("wormhole");

}

Layer * Wormhole::getDestinationLayer()
{
	return diyLevel->layers[destinationLayer];
}

void Wormhole::Update(float timeStep_)
{

	scale.z = isScalingDown ? scale.z - 0.1f * timeStep_ : scale.z + 0.1f * timeStep_;
	isScalingDown = isScalingDown ? (scale.z < 0 ? false : true) : (scale.z > 0.7f ? true : false);

	Rotate(Quat(-(initialRotation + yRotation) * M_PI / 180.0f, Vec3::BasisY()));
	Rotate(Quat(90 * M_PI / 180.0f * timeStep_, Vec3::BasisZ()));
	yRotation += 5 * timeStep_;
	Rotate(Quat((initialRotation + yRotation) * M_PI / 180.0f, Vec3::BasisY()));

	if (!player)
		player = static_cast<PlayerBall *>(level->FindGameObjectWithTag("player"));

	GameObject::Update(timeStep_);

	if ((position - player->position).length() < 1.f)
	{
		if (!ballCaught)
		{
			player->getComponent<Rigidbody>()->velocity = Vec3(0, -0.1f, 0);

			sound->Play();

			ballCaught = true;
			diyLevel->SetLayerPlane(getDestinationLayer());

			//Teleport to layer
			player->position = getDestinationLayer()->position + destinationLocation + Vec3(0, 1.5f, 0);
		}
	}
	else
	{
		if (ballCaught)
			ballCaught = false;
	}
}