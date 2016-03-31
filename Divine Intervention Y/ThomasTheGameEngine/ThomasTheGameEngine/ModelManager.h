#pragma once
#include <string>
#include <vector>
#include <map>
#include "../Math/four_dimensions.hpp"
#include <glew.h>
#include "OpenGLUtilities.h"
#include <SDL_image.h>
#include "Colour.h"
#include "Shader.h"

#define RENDER_MODE_OPENGL ModelManager::Render_OpenGL
#define RENDER_MODE_DIRECTX ModelManager::Render_DirectX
#define RENDER_MODE_OGRE ModelManager::Render_Ogre

#define Models ModelManager::getInstance()

class Texture;

//So I dont have to type out std::string
typedef std::string string;
typedef unsigned int UINT32;

enum Shader_Program_Type
{
	GENERIC_SHADER, GUI_SHADER, SUN_SHADER, RAINBOW_GUI_SHADER, RAINBOW_GAMEOBJECT_SHADER
};

enum Draw_Mode
{
	CW, CCW
};

class ModelManager
{
	friend class RenderableComponent;
	friend class Renderable;
	friend class Camera;
	friend class TextSprite;
	friend class MultiLineTextSprite;

public:

	~ModelManager();

	//A static pointer to an instance of the manager
	static ModelManager * instance;

	//Returns a pointer to a ModelManager. Render mode defaults to openGL if none specified
	//Whatever you pass wont matter if instance is not null
	static ModelManager * getInstance(){
		if (!instance)instance = new ModelManager();
		return instance;
	}

	//Creates a renderable based on the given COLLADA file and inserts
	//it into the map of models with the _id as its string
	void loadModel(string _id, string _fileName, bool _useModelTextureMap = false, Draw_Mode _mode = Draw_Mode::CW);

	////called to delete all models in memory and clear the model map
	//void unloadModels();

	//Pushed all model information to the GPU
	void PushModels();

	//Clears all Models / Textures from ModelManager.
	void UnloadModels();

	//creates a texture using the pixel data and loads it into the texture map using the string _id
	void createTexture(string _id, void* _pixelData, UINT32 _textureWidth, UINT32 _textureHeight);

	//loads a texture with the fileName and loads it into the texture map using the string _id
	std::string loadTexture(string _id, string _fileName);

	/* PRIMITIVE FACTORY METHODS */
	void CreateCuboid(string _id, float _w, float _h, float _l, bool _useCubeMap = false, float _uvRepeatX = 1, float _uvRepeatY = 1);
	void CreateSkybox(string _id, float _size, bool _normalsOnBottom = true);
	void CreateSphere(string _id, float _r, float _uvRepeatX = 1, float _uvRepeatY = 1){}
	void CreatePlane(string _id, float _w, float _h, float _uvRepeatX = 1, float _uvRepeatY = 1);
	void CreatePyramid(string _id, float _w, float _h, float _l, float _uvRepeatX = 1, float _uvRepeatY = 1);
	void CreateCone(string _id, float _r, float _h, float _uvRepeatX = 1, float _uvRepeatY = 1){}
	void CreateCylinder(string _id, float _r, float _h, float _uvRepeatX = 1, float _uvRepeatY = 1){}

	UINT32 GetModelID(string _name) const;
	UINT32 GetTextureID(string _name) const;

	int GetTextureWidth(string _name) const;
	int GetTextureWidth(UINT32 _id) const;

	int GetTextureHeight(string _name) const;
	int GetTextureHeight(UINT32 _id) const;

	//Goes into the texture map and returns a pointer to the texture with the given tag
	Texture* getTexture(string _id) const;
	Texture* getTexture(UINT32 _id) const;

	//Returns the texture ID
	UINT32 InsertTexture(Texture* texture_);
private:
	ModelManager();

	enum Buffer_Type
	{
		VERTEX_BUFFER, TEXTURE_BUFFER, NORMAL_BUFFER
	};

	//Goes into the model map and returns a pointer to the renderable with the given tag
	Renderable * getModel(string _id);
	Renderable * getModel(UINT32 _id);

	//The map to store all models
	std::map<UINT32, Renderable *> models;
	std::map<string, UINT32> modelMap;
	static UINT32 nextModelID;

	//The master Vector list that will ultimately be pushed to the GPU
	std::vector<Vec3> masterVectorList, masterNormalList;

	std::vector<Colour> masterColourList;

	//The map to store all textures
	std::map<UINT32, Texture *> textures;
	std::map<string, UINT32> textureMap;
	static UINT32 nextTextureID;

	//The master Texture coordinates list that will ultimately be pushed to the GPU
	std::vector<Vec2> masterTextureCoords;

	//Shaders Location
	std::vector<Shader*> shaderPrograms;
	//GLuint VAOs[1];
	static const GLuint NUMBER_OF_BUFFERS = 3;
	GLuint Buffers[NUMBER_OF_BUFFERS];

	bool areBuffersInitialized;
	GLuint* textureArray;
	GLuint numberOfTextures;

	//Creates default models / textures
	void GenerateDefaultContent();

	//Sets vertexOffset and inserts into model map. Used to finish model creation.
	void InsertModel(Renderable* _renderable, string _id);

	//Generates the TextureMap for the model
	void GenerateTextureMap(Renderable* _renderable, float _uvRepeatX = 1, float _uvRepeatY = 1);

	//Generates a CubeMap for textures
	void GenerateCubeMap(Renderable* _renderable);

	//Generates the normals for the model
	void GenerateNormals(Renderable* _renderable, bool _reverse = false, bool _normalsOnBottom = false, bool _onlyFaceNormals = false);
};

