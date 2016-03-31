#include "LightManager.h"
#include "Shader.h"
#include <algorithm>

LightManager * LightManager::instance;

LightManager::LightManager()
{
	ResetDirectional();
	ResetPoint();
	ResetSpot();
}

LightManager::~LightManager(){}

void LightManager::ResetDirectional()
{
	Directional.clear();
}

void LightManager::ResetPoint()
{
	Point.clear();
}

void LightManager::ResetSpot()
{
	Spot.clear();
}

void LightManager::InputDirectionalLight(Colour _color, Vec4 _direction)
{
	Directional.push_back(DirectionalLight(_color, _direction));
}

void LightManager::InputPointLight(Colour _color, Vec4 _position)
{
	Point.push_back(PointLight(_color, _position));
}

void LightManager::InputSpotLight(Colour _color, Vec4 _position, Vec4 _direction, float _coneAngle)
{
	Spot.push_back(SpotLight(_color, _position, _direction, _coneAngle));
}

void LightManager::PushLights(Vec3 cameraPosition_)
{
	int DirectionalLength = Directional.size() > MAX_DIRECTIONAL ? MAX_DIRECTIONAL : Directional.size();
	int PointLength = Point.size() > MAX_POINT ? MAX_POINT : Point.size();
	int SpotLength = Spot.size() > MAX_SPOT ? MAX_SPOT : Spot.size();

	//Direction
	{
		//Check if theres more directional lights than supported
		if (Directional.size() > MAX_DIRECTIONAL)
		{
			//Sort by distance to camera
			//TODO: make a sort function for each light type
			//std::sort(Directional.begin(), Directional.end(), []())
		}

		float* color = new float[DirectionalLength * 4];
		float* direction = new float[DirectionalLength * 4];

		for (int i = 0; i < DirectionalLength; i++)
		{
			color[i * 4 + 0] = Directional[i].colour.r;
			color[i * 4 + 1] = Directional[i].colour.g;
			color[i * 4 + 2] = Directional[i].colour.b;
			color[i * 4 + 3] = Directional[i].colour.a;

			direction[i * 4 + 0] = Directional[i].direction.x;
			direction[i * 4 + 1] = Directional[i].direction.y;
			direction[i * 4 + 2] = Directional[i].direction.z;
			direction[i * 4 + 3] = Directional[i].direction.w;
		}

		glProgramUniform4fv(Generic_Shader::_GetInstance()->GetProgram(), Generic_Shader::_GetInstance()->lightColor_Directional_Location, DirectionalLength, color);
		glProgramUniform4fv(Generic_Shader::_GetInstance()->GetProgram(), Generic_Shader::_GetInstance()->lightDirection_Directional_Location, DirectionalLength, direction);

		ResetDirectional();
		delete[] color;
		delete[] direction;
	}

	//Point
	{
		float* color = new float[PointLength * 4];
		float* position = new float[PointLength * 4];

		for (int i = 0; i < PointLength; i++)
		{
			color[i * 4 + 0] = Point[i].colour.r;
			color[i * 4 + 1] = Point[i].colour.g;
			color[i * 4 + 2] = Point[i].colour.b;
			color[i * 4 + 3] = Point[i].colour.a;

			position[i * 4 + 0] = Point[i].position.x;
			position[i * 4 + 1] = Point[i].position.y;
			position[i * 4 + 2] = Point[i].position.z;
			position[i * 4 + 3] = Point[i].position.w;
		}

		glProgramUniform4fv(Generic_Shader::_GetInstance()->GetProgram(), Generic_Shader::_GetInstance()->lightColor_Point_Location, PointLength, color);
		glProgramUniform4fv(Generic_Shader::_GetInstance()->GetProgram(), Generic_Shader::_GetInstance()->lightPosition_Point_Location, PointLength, position);

		ResetPoint();
		delete[] color;
		delete[] position;
	}

	//Spot
	{
		float* color = new float[SpotLength * 4];
		float* position = new float[SpotLength * 4];
		float* direction = new float[SpotLength * 4];
		float* angle = new float[SpotLength];

		for (int i = 0; i < SpotLength; i++)
		{
			color[i * 4 + 0] = Spot[i].colour.r;
			color[i * 4 + 1] = Spot[i].colour.g;
			color[i * 4 + 2] = Spot[i].colour.b;
			color[i * 4 + 3] = Spot[i].colour.a;

			position[i * 4 + 0] = Spot[i].position.x;
			position[i * 4 + 1] = Spot[i].position.y;
			position[i * 4 + 2] = Spot[i].position.z;
			position[i * 4 + 3] = Spot[i].position.w;

			direction[i * 4 + 0] = Spot[i].direction.x;
			direction[i * 4 + 1] = Spot[i].direction.y;
			direction[i * 4 + 2] = Spot[i].direction.z;
			direction[i * 4 + 3] = Spot[i].direction.w;

			angle[i] = Spot[i].coneAngle;
		}

		glProgramUniform4fv(Generic_Shader::_GetInstance()->GetProgram(), Generic_Shader::_GetInstance()->lightColor_Spot_Location, SpotLength, color);
		glProgramUniform4fv(Generic_Shader::_GetInstance()->GetProgram(), Generic_Shader::_GetInstance()->lightPosition_Spot_Location, SpotLength, position);
		glProgramUniform4fv(Generic_Shader::_GetInstance()->GetProgram(), Generic_Shader::_GetInstance()->lightDirection_Spot_Location, SpotLength, direction);
		glProgramUniform1fv(Generic_Shader::_GetInstance()->GetProgram(), Generic_Shader::_GetInstance()->lightAngle_Spot_Location, SpotLength, angle);

		ResetSpot();
		delete[] color;
		delete[] position;
		delete[] direction;
		delete[] angle;
	}

	float numLights[] = { DirectionalLength, PointLength, SpotLength };
	glProgramUniform3fv(Generic_Shader::_GetInstance()->GetProgram(), Generic_Shader::_GetInstance()->numberOfLights_Location, 1, numLights);
}