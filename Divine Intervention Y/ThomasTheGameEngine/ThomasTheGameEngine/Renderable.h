#pragma once

#include "GameObject.h"
#include <vector>
#include "../Math/four_dimensions.hpp"
#include "ModelManager.h"

/*
	An abstract class that contains all information a renderable object
	would have. The draw method is abstract so that a concrete class can
	be made to specify how its draw.
	*/

class Renderable
{
	friend class ModelManager;
public:
	~Renderable(){};

	struct Mesh
	{
		//Lists of all vertecies, edges, faces and normals
		std::vector<Vec3> vertex;
		std::vector<unsigned int> edge;
		std::vector<unsigned int> face;
		std::vector<Vec3> normal;
		std::vector<Vec3> faceNormal;
		std::vector<Vec2> textureMap;
		
		unsigned int offsetVertex;
	};

	std::vector<Mesh> meshes;

	Draw_Mode GetDrawMode() const;
protected:
	
	Draw_Mode drawMode;

	Renderable()
	{
		drawMode = Draw_Mode::CCW;
	};
};