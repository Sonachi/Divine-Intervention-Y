#pragma once
#include "Component.h"
#include "Component.h"
#include "../Math/four_dimensions.hpp"
#include "Colour.h"

class Light :
	public Component
{

public:
	enum Light_Type{
		Point,
		Spot,
		Directional
	};

	Light(GameObject * _g, Colour _colourIntensity, Light_Type _type, float _coneAngle = 0.5f);

	~Light();

	Light_Type lightType;
	Colour colourIntensity;
	float ConeAngle;

	void PushLight();
};

