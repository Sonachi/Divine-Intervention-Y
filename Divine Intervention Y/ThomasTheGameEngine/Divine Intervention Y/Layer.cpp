#include "Layer.h"
#include "DIY_Level.h"

#include <Random.h>
#include <string>

Layer::Layer(Level * _level, tinyxml2::XMLElement * element_, int index_) : GameObject(_level)
{
	//Save reference to the level as proper pointer
	dl = static_cast<DIY_Level *>(_level);

	index = index_;
	distanceFromSun = element_->FloatAttribute("r");

	position = Vec3(Random::box_muller(0.f, 1.f), 0, Random::box_muller(0.f, 1.f)).Normalized() * (distanceFromSun + dl->sunRadius);

	//Adjust size of layer
	float scale = element_->FloatAttribute("s");
	
	//Enforce minimum layer size
	if (scale < 10) scale = 10.0f;

	Scale(Vec3(scale, 0.01f, scale));

	//Not the first layer
	if (index != 0)
	{
		float x = element_->FloatAttribute("wormHoleX");
		float z = element_->FloatAttribute("wormHoleZ");

		float destX = element_->FloatAttribute("wormHoleDestX");
		float destZ = element_->FloatAttribute("wormHoleDestY");

		wormHole = new Wormhole(dl, position + Vec3(x, 0, z), index - 1, Vec3(destX, 0, destZ));

		//Create wormhole here
		objects.push_back(wormHole);
	}

	float x = element_->FloatAttribute("planetX");
	float z = element_->FloatAttribute("planetZ");
	float s = element_->FloatAttribute("planetScale");
	const char * sunTex = element_->Attribute("planetTexture");

	//Planet creation not including scale
	planet = new Planet(dl, position + Vec3(x, 0, z), s, sunTex);
	objects.push_back(planet);

	//Grab the first element and loop through all the elements
	//Adding GameObjects as you go
	tinyxml2::XMLElement * object = element_->FirstChildElement("Object");
	while (object)
	{
		Vec2 position = Vec2(object->FloatAttribute("x"), object->FloatAttribute("z"));
		CreateObjects(object->Attribute("type"), position);
		object = object->NextSiblingElement();
	}


	for (int i = 0; i < objects.size(); i++)
	{
		addChild(objects[i]);

		objects[i]->SetRenderEnabled(false);
	}

	r = new Generic_RenderableComponent(this, "cuboid", "layerGrid");
	r->SetEffecctedByLight(false, false, false);
	r->isEnabled = false;
}


Layer::~Layer()
{
}

//Where all the objects are created
void Layer::CreateObjects(const char * attribute_, const Vec2 position_)
{
	std::string string(attribute_);

	if (string == "WarpGate")
	{
		objects.push_back(new WarpGate(dl, position + Vec3(position_.x, 0, position_.y), Quat::Identity()));
	}
	if (string == "Asteroids")
	{
		objects.push_back(new AsteroidField(dl, position + Vec3(position_.x, 0, position_.y), 1, 4));
	}
	if (string == "Aliens")
	{
		objects.push_back(new Swarm(dl, position + Vec3(position_.x, 1, position_.y)));
	}
	if (string == "Satellite")
	{
		objects.push_back(new Satellite(dl, planet, position + Vec3(position_.x, 1, position_.y), Vec3(0, 1, 0), 0.001, "satellite", "white"));
	}
	if (string == "PopUp")
	{
		objects.push_back(new PopUp(dl, position + Vec3(position_.x, 0, position_.y)));
	}
}

void Layer::SetEnabled(bool isEnabled_)
{
	r->isEnabled = isEnabled_;

	for (int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->SetRenderEnabled(isEnabled_);
	}
}

int Layer::GetIndex()
{
	return index;
}