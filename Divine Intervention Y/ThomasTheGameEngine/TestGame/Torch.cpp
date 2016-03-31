#include "Torch.h"
#include <Level.h>
#include <ModelManager.h>
#include <Flipbook.h>
#include <Light.h>

Torch::Torch(Level * _level, Vec3 _position) : GameObject(_level, _position)
{
	ModelManager * mm = ModelManager::getInstance();

	mm->loadModel("torch", "Models/torch.obj", true, ModelManager::CCW);
	mm->loadTexture("torchTexture", "Images/torch_DIF.tif");

	scale = Vec3(0.05f, 0.05f, 0.05f);

	torch = new RenderableComponent("torch", "torchTexture", this);

	flame = new Billboard(_level, _position + Vec3(0, 8.8f, 2.7f));
	flame->Scale(Vec3(1.8, 1.8, 1.5));

	//new Light(flame, Colour(100.0f, 100.0f, 100.0f), Light::Point);

	Flipbook * fb = new Flipbook(flame, 16, "Images/Animation/Fire/slice0.png", 1.5f, true, Flipbook::PNG);
	fb->Play();
	fb->SetEffecctedByLight(false, false, false);

	addChild(flame);

	smokeEmitter = new ParticleSystem(flame, ParticleSystem::Emitter_Type_Cone, "plane", "smoke", 10, 0.5f, 2.0f, Particle::Billboard);
}


Torch::~Torch()
{
	/*delete torch;
	delete smokeEmitter;
	delete flame;*/
}
