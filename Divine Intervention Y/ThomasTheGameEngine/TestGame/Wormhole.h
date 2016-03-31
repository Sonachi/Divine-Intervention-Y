//Author : Nathan Senter
//Date : 2/3/2016 (NS)

#pragma once

#include <GameObject.h>
#include <RenderableComponent.h>

class Layer;
class GameLevel;

class Wormhole : public GameObject
{
public:
	Wormhole(Level *, Vec3, int);
	~Wormhole(){}

	Layer * getDestinationLayer();
	//void setDestinationLayer(Layer *);

private:

	RenderableComponent * rc;
	int destinationLayer;
};