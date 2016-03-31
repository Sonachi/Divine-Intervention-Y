#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject * _go, ComponentType _type) : type(_type)
{
	if (_go)
	{
		parentObject = _go;
		_go->components.push_back(this);
	}	
	isEnabled = true;
}

Component::~Component()
{
}