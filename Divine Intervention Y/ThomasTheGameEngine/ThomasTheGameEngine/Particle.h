#pragma once

#include "GameObject.h"
#include "RenderableComponent.h"



class Particle : public GameObject
{

public:
	enum Object_Type
	{
		Billboard, Object3D
	};

	Particle(Level *, Vec3 _startPosition, float _lifeTime, std::string _modelName, std::string _textureName, Object_Type _type);
	~Particle();

	void Update(float _deltaTime) override;
	void Render() override;

	void SetVelocity(Vec3);

	bool isAlive;
private:
	Object_Type objectType;

	float scaleAmount, rotationAmount;

	float maxLifeTime, currentLifeTime;

	Vec3 velocity;

	RenderableComponent * renderer;
};