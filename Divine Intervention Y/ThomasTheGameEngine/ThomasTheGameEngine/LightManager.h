#include "../Math/four_dimensions.hpp"
#include "Colour.h"
#include <vector>

class LightManager
{
public:
	struct DirectionalLight
	{
		Colour colour;
		Vec4 direction;

		DirectionalLight(Colour colour_, Vec4 direction_) : colour(colour_), direction(direction_) {}
	};

	struct PointLight
	{
		Colour colour;
		Vec4 position;

		PointLight(Colour colour_, Vec4 position_) : colour(colour_), position(position_) {}
	};

	struct SpotLight
	{
		Colour colour;
		Vec4 position, direction;
		float coneAngle;

		SpotLight(Colour colour_, Vec4 position_, Vec4 direction_, float coneAngle_) : colour(colour_), position(position_), direction(direction_), coneAngle(coneAngle_) {}
	};
	
	~LightManager();

	//A static pointer to an instance of the manager
	static LightManager * instance;

	//Returns a pointer to a LightManager.
	static LightManager * getInstance(){
		if (!instance)instance = new LightManager();
		return instance;
	}

	//Input a Directional Light into Light array
	void InputDirectionalLight(Colour _color, Vec4 _direction);

	//Input a Point Light into Light array
	void InputPointLight(Colour _color, Vec4 _position);

	//Input a Spot Light into Light array
	void InputSpotLight(Colour _color, Vec4 _position, Vec4 _direction, float _coneAngle);

	//Pushes Lights to Shaders. Should be called After lighting arrays have been built and before draw calls.
	void PushLights(Vec3 cameraPosition_);

private:
	LightManager();

	void ResetDirectional();
	void ResetPoint();
	void ResetSpot();

	static const unsigned int MAX_DIRECTIONAL = 4, MAX_POINT = 10, MAX_SPOT = 10;	
	std::vector<DirectionalLight> Directional;
	std::vector<PointLight> Point;
	std::vector<SpotLight> Spot;

};