#pragma once

#include "Component.h"
#include "../Math/four_dimensions.hpp"
#include <vector>
#include "Particle.h"

class ParticleSystem : public Component
{

public:

	enum Emitter_Type
	{
		Emitter_Type_Sphere,
		Emitter_Type_Cone,
		Emitter_Type_Cube,
		Emitter_Type_None
	};

	ParticleSystem(GameObject * _owner, Emitter_Type _emitterType, std::string _modelName, std::string _textureName,
		float _maxParticles, float _rateOfEmission, float _lifeTime, Particle::Object_Type);

	~ParticleSystem();
	void UpdateParticles(float _deltaTime);
	void RenderParticles();

	void SetEmitterType(Emitter_Type);
	void SetInitialVelocity(Vec3);
	void SetInitialRotation(Quat);
	void SetPositionOffset(Vec3);
	void SetInitialScale(Vec3);

	bool isPlaying();

	void Play();
	void Stop();
private:

	std::vector<Particle *> particles;
	
	Vec3 iVeloctiy;
	Vec3 iScale;
	Quat iRotation;

	Vec3 positionOffset;

	Emitter_Type emitter_type;
	
	bool isActive;

	float elapsedTime;
	float emissionRate;
};