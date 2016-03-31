#include "Camera.h"
#include "GameProperties.h"
#include "dependencies\glm\glm.hpp"
#include "dependencies\glm\gtc\matrix_transform.hpp"
#include "dependencies\glm\gtc\type_ptr.hpp"

#include <iostream>
#include "ModelManager.h"
#include "Shader.h"

Camera::Camera(Level * _level) : GameObject(_level, Vec3(0, 0, -2))
{
	CalculateCameraMatrix();	
}

void Camera::CalculateCameraMatrix()
{
	GameProperties * gp = GameProperties::getInstance();

	glm::mat4 m = glm::perspective(45.0f, gp->getVideoProperties()->aspectRatio(), 0.1f, gp->getVideoProperties()->drawDistance);

	Vec3 camForward = Quat::rotate(rotation, Vec3::BasisZ());
	Vec3 forawrd = position + camForward;

	Vec3 _camUp = Quat::rotate(rotation, Vec3(0, 1, 0)).Normalized();
	glm::mat4 m2 = glm::lookAt(glm::vec3(position.x, position.y, position.z), glm::vec3(forawrd.x, forawrd.y, forawrd.z), glm::vec3(_camUp.x, _camUp.y, _camUp.z));
	
	//Get array pointer to glm matrix
	const float *mSource = (const float*)glm::value_ptr(m);
	const float *m2Source = (const float*)glm::value_ptr(m2);

	for (int i = 0; i < 16; i++)
	{
		projectionMatrix.values[i] = mSource[i];
		viewMatrix.values[i] = m2Source[i];
	}
	
	////Note: To get the same results as GLM we must transpose both before AND after multiplication.	
	//projectionMatrix = projectionMatrix.transpose();
	//viewMatrix = viewMatrix.transpose();
	//
	//cameraMatrix = projectionMatrix * viewMatrix;
	//cameraMatrix = cameraMatrix.transpose();
}

Matrix4 Camera::getMatrix() const
{
	Matrix4 vp = projectionMatrix;
	Matrix4 mv = viewMatrix;

	return (vp.transpose() * mv.transpose()).transpose();
}

void Camera::Update(float _deltaTime)
{
	CalculateCameraMatrix();
	float pos[] = { position.x, position.y, position.z };

	//Update Shaders
	glProgramUniformMatrix4fv(Generic_Shader::_GetInstance()->GetProgram(), Generic_Shader::_GetInstance()->projection_Location, 1, GL_FALSE, projectionMatrix.values);
	glProgramUniformMatrix4fv(Generic_Shader::_GetInstance()->GetProgram(), Generic_Shader::_GetInstance()->view_Location, 1, GL_FALSE, viewMatrix.values);
	glProgramUniform3fv(Generic_Shader::_GetInstance()->GetProgram(), Generic_Shader::_GetInstance()->cameraPosition_Location, 1, pos);

	glProgramUniformMatrix4fv(Sun_Shader::_GetInstance()->GetProgram(), Sun_Shader::_GetInstance()->projection_Location, 1, GL_FALSE, projectionMatrix.values);
	glProgramUniformMatrix4fv(Sun_Shader::_GetInstance()->GetProgram(), Sun_Shader::_GetInstance()->view_Location, 1, GL_FALSE, viewMatrix.values);
	glProgramUniform3fv(Sun_Shader::_GetInstance()->GetProgram(), Sun_Shader::_GetInstance()->cameraPosition_Location, 1, pos);
	Sun_Shader::_GetInstance()->camPosition = position;

	glProgramUniformMatrix4fv(Atmosphere_Shader::_GetInstance()->GetProgram(), Atmosphere_Shader::_GetInstance()->projection_Location, 1, GL_FALSE, projectionMatrix.values);
	glProgramUniformMatrix4fv(Atmosphere_Shader::_GetInstance()->GetProgram(), Atmosphere_Shader::_GetInstance()->view_Location, 1, GL_FALSE, viewMatrix.values);

	glProgramUniformMatrix4fv(Glow_Shader::_GetInstance()->GetProgram(), Glow_Shader::_GetInstance()->projection_Location, 1, GL_FALSE, projectionMatrix.values);
	glProgramUniformMatrix4fv(Glow_Shader::_GetInstance()->GetProgram(), Glow_Shader::_GetInstance()->view_Location, 1, GL_FALSE, viewMatrix.values);
	glProgramUniform3fv(Glow_Shader::_GetInstance()->GetProgram(), Glow_Shader::_GetInstance()->cameraPosition_Location, 1, pos);
}

void Camera::Render()
{		
}

Camera::~Camera()
{
}
