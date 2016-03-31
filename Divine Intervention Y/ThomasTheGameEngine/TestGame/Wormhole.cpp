#include "Wormhole.h"

//#include "GameLevel.h"

Wormhole::Wormhole(Level * level_, Vec3 position_, int layerIndex) : GameObject(level_, position_)
{
	destinationLayer = layerIndex;

	rc = new RenderableComponent("cuboid", "planet1", this);
}

Layer * Wormhole::getDestinationLayer()
{
	//GameLevel * gl = static_cast<GameLevel *>(level);

	//return gl->layers[destinationLayer];
	return nullptr;
}

//void Wormhole::setDestinationLayer(Layer * layer_)
//{
//	destinationLayer = layer_;
//}