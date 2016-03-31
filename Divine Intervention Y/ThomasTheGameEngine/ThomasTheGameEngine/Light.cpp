#include "Light.h"
#include "ModelManager.h"
#include "OpenGLUtilities.h"
#include "LightManager.h"
#include "GameObject.h"
#include "Colour.h"

Light::Light(GameObject * _g, Colour _colourIntensity, Light_Type _type, float _coneAngle) : Component(_g, Component::Light)
{
	colourIntensity = _colourIntensity;
	lightType = _type;
	ConeAngle = _coneAngle;
}

Light::~Light()
{
}

void Light::PushLight()
{
	Vec3 _forward;

	switch (lightType)
	{
		case Light_Type::Directional:
			_forward = parentObject->forward();
			LightManager::getInstance()->InputDirectionalLight(colourIntensity, Vec4(0, _forward.x, _forward.y, _forward.z));
			break;

		case Light_Type::Point:
			LightManager::getInstance()->InputPointLight(colourIntensity, Vec4(0, parentObject->position.x, parentObject->position.y, parentObject->position.z));
			break;

		case Light_Type::Spot:
			_forward = parentObject->forward();
			LightManager::getInstance()->InputSpotLight(colourIntensity, Vec4(0, parentObject->position.x, parentObject->position.y, parentObject->position.z), Vec4(0, _forward.x, _forward.y, _forward.z), ConeAngle);
			break;
	}	
}