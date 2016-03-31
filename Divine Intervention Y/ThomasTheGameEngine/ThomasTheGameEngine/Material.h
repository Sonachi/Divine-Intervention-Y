#pragma once

#include "../Math/four_dimensions.hpp"

#define MATERIAL_TYPE_MATTE Material_Prefab.Matte
#define MATERIAL_TYPE_METALIC Material_Prefab.Metalic



class Material
{
public:

	enum Material_Prefab{
		MATTE,
		METALIC
	};

	Material(float _ambient, float _diffuse, float _specular);
	Material(Material_Prefab _preDefinedMaterial){};

	~Material();

	float ambient, diffuse, specular;

	Vec3 getMaterial();
};

