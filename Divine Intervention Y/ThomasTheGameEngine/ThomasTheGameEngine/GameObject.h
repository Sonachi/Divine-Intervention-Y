// Author: Nathan Senter
// I forgot the original date. The new one is 05/20/2015

#pragma once
#include <vector>
#include <string>
#include <typeinfo>
#include <glew.h>
#include <freeglut.h>
#include "../Math/four_dimensions.hpp"
//#include "Transform.h"

class Component;
class Level;

typedef std::string Tag;

class GameObject
{
	friend class Particle;
public:
	
	Vec3 position;
	Vec3 scale;
	Quat rotation;

	//Direction vectors that can aid in object movement
	Vec3 forward();
	Vec3 up();
	Vec3 right();

	//Returns the objects orientation in different formats
	Quat getQuat();
	Vec3 getEuler();

	//Transform methods
	//These will apply the given transforms and move the gameObject
	void LookAt(Vec3);
	void Rotate(Quat);
	void RotateAround(Vec3 centre_, Vec3 axis_, float angle_);

	//Rotates in the X-Y-Z plane (in that order) use Radians, if possible use the Rotate(Quat) method instead
	void Rotate(Vec3);
	void Translate(Vec3);
	virtual void Scale(Vec3);

	//Returns all components of the transform as a single matrix4
	//More or less only used for the GPU
	Matrix4 toMat4();

	
	//Various constructors to set the starting position of a gameObject
	GameObject(Level *);
	GameObject(Level *, Vec3 _position);
	//GameObject(Transform _t);
	~GameObject();

	GameObject& GetTransform();
	//Transform transform;

	//Render and update functionalities specific to the gameObject
	virtual void Render();
	virtual void PreRender();
	virtual void Update(float _deltaTime);

	Level * level;

	//Flag that sets object for removal from game
	bool isFlagged;

	//A list of ALL the gameObjects components
	std::vector<Component *> components;

	//List of all tags applied to this game object
	std::vector<Tag> Tags;

	//List of all objects that are children of this object
	std::vector<GameObject *> childObjects;

	//Removes the specified component from the gameObject
	void removeComponent(Component *);

	//Adds a string tag to the gameObject. Tags can be used
	//as an easy way to identify gameObjects from outside itself
	void addTag(Tag);

	//Adds or removes gameObjects as children
	void addChild(GameObject *);
	void removeChild(GameObject *);

	//Checks whether the game object has given tag
	bool hasTag(Tag);

	//Triggers the isFlagged boolean
	void Destroy();

	//Set whether the gameObject will render.
	virtual void SetRenderEnabled(bool isEnabled_);
	
	//Gets whether the gameObject will render.
	virtual bool GetRenderEnabled();

	/**********************************************/
	//NOTE: This method is still under development// 
	/**********************************************/

	//Returns the address to a given component type
	template<class TYPE> TYPE* getComponent();
	/*{
		for (int i = 0; i < components.size(); i++)
		{
			if (typeid(*components[i]) == typeid(TYPE))
			return (TYPE*)components[i];

		}
		return nullptr;
	}*/


private:
	GameObject(){};

};

