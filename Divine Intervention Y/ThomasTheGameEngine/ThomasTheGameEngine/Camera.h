#pragma once
#include "GameObject.h"
#include "Frustrum.h"
#include "../Math/four_dimensions.hpp"

class Camera :
	public GameObject
{
public:
	Camera(Level * _level);
	~Camera();

	Frustrum frustrum;

	//Returns a matrix of all aspects of the camera to be used by the GPU.
	// I.e. projection and model view matrix
	Matrix4 getMatrix() const;

	//Recalculates the above matrix
	void CalculateCameraMatrix();

	//Overridden methods inherited from the base class
	void Render() override;
	void Update(float _deltaTime) override;

private:
	//All matricies associated with the camera
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
};

