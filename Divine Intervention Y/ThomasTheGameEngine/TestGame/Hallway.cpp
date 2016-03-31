#include "Hallway.h"
#include <Level.h>
#include <RenderableComponent.h>

Hallway::Hallway(Level * _level, Vec3 _position) : GameObject(_level, _position)
{
	Tags.push_back("hallway");

	floor = new GameObject(_level);
	wall1 = new GameObject(_level);
	wall2 = new GameObject(_level);
	cieling = new GameObject(_level);

	RenderableComponent * r = new RenderableComponent("surface", "floor", floor);
	r->SetEffecctedByLight(true, false, true);

	floor->Scale(Vec3(10, 1, 1));

	floor->Rotate(Quat(M_PI, Vec3(0, 1, 0)));
	floor->Rotate(Quat(M_PI / 2.0f, Vec3(0, 0, 1)));
	floor->Rotate(Quat(M_PI / 2.0f, Vec3(1, 0, 0)));

	RenderableComponent * r1 = new RenderableComponent("surface", "wall", wall1);
	r1->SetEffecctedByLight(true, false, true);

	wall1->Scale(Vec3(10, 1, 1));
	wall1->Rotate(Quat(-M_PI / 2.0f, Vec3(0, 1, 0)));

	RenderableComponent * r2 = new RenderableComponent("surface", "wall", wall2);
	r2->SetEffecctedByLight(true, false, true);

	wall2->Scale(Vec3(10, 1, 1));
	wall2->Rotate(Quat(M_PI / 2.0f, Vec3(0, 1, 0)));

	RenderableComponent * r3 = new RenderableComponent("surface", "wall", cieling);
	r3->SetEffecctedByLight(true, false, true);

	cieling->Scale(Vec3(10, 1, 1));
	cieling->Rotate(Quat(M_PI / 2.0f, Vec3(1, 0, 0)));
	cieling->Rotate(Quat(M_PI / 2.0f, Vec3(0, 1, 0)));

	floor->position = _position + Vec3(0, -0.5f, 0);
	wall1->position = _position + Vec3(0.5f, 0, 0);
	wall2->position = _position + Vec3(-0.5f, 0, 0);
	cieling->position = _position + Vec3(0, 0.5f, 0);
}


Hallway::~Hallway()
{
	delete wall1;
	delete wall2;
	delete cieling;
	delete floor;
}

void Hallway::letThereBeLight()
{
	floor->getComponent<RenderableComponent>()->SetEffecctedByLight(true, true, true);
	wall1->getComponent<RenderableComponent>()->SetEffecctedByLight(true, true, true);
	wall2->getComponent<RenderableComponent>()->SetEffecctedByLight(true, true, true);
	cieling->getComponent<RenderableComponent>()->SetEffecctedByLight(true, true, true);
}
