#include "WarpGate.h"

#include <RenderableComponent.h>

WarpGate::WarpGate(Level * level_, Vec3 position_, Quat rotation_) : GameObject(level_, position_)
{
	rotation = rotation_;

	rc = new RenderableComponent("warpGate", "gateTexture", this);

	Scale(Vec3(0.1, 0.1, 0.1));
}