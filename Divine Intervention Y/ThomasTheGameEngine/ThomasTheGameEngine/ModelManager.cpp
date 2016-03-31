#include "ModelManager.h"
#include "Renderable.h"
#include "Texture.h"
#include "Shader.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <windows.h>

void gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
		);
}

int wherex()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.dwCursorPosition.X;
}

int wherey()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.dwCursorPosition.Y;
}

#define BUFFER_OFFSET(i) ((void*)(i))

ModelManager * ModelManager::instance;

UINT32 ModelManager::nextTextureID = 0;
UINT32 ModelManager::nextModelID = 0;

ModelManager::ModelManager()
{
	//Create Buffers
	glGenBuffers(NUMBER_OF_BUFFERS, Buffers);
	areBuffersInitialized = true;

	//Create Shader Programs
	shaderPrograms.push_back(Generic_Shader::_GetInstance());
	shaderPrograms.push_back(GUI_Shader::_GetInstance());
	shaderPrograms.push_back(Sun_Shader::_GetInstance());
	shaderPrograms.push_back(Rainbow_GUI_Shader::_GetInstance());
	shaderPrograms.push_back(Atmosphere_Shader::_GetInstance());
	shaderPrograms.push_back(Glow_Shader::_GetInstance());

	//Bind Attributes
	{
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[VERTEX_BUFFER]);
		glVertexAttribPointer(VERTEX_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(VERTEX_ATTRIBUTE);

		glBindBuffer(GL_ARRAY_BUFFER, Buffers[TEXTURE_BUFFER]);
		glVertexAttribPointer(TEXTURE_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(TEXTURE_ATTRIBUTE);

		glBindBuffer(GL_ARRAY_BUFFER, Buffers[NORMAL_BUFFER]);
		glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
		glEnableVertexAttribArray(NORMAL_ATTRIBUTE);
	}

	textureArray = nullptr;
	numberOfTextures = 0;	

	GenerateDefaultContent();
}

ModelManager::~ModelManager()
{
	//deletes all models still in memory
	for (auto it = models.begin(); it != models.end(); it++)
	{
		if (it->second)
			delete it->second;
	}

	models.clear();

	/*for (auto it = textures.begin(); it != textures.end(); it++)
	{
	if (it->second)
	delete it->second;
	}*/

	textures.clear();
}

void ModelManager::GenerateDefaultContent()
{
	CreatePlane("plane", 1.0f, 1.0f);
	CreateCuboid("cuboid", 1.0f, 1.0f, 1.0f);

	//Gotta be big to show up..
	//don't know why 
	const int texSize = 140 * 140 * 4;
	float pixelDataWhite[texSize];
	for (int i = 0; i < texSize; i++)
	{
		pixelDataWhite[i] = 1.0f;
	}
	createTexture("white", pixelDataWhite, 1, 1);
}

void ModelManager::loadModel(string _id, string _fileName, bool _useModelTextureMap, Draw_Mode _mode)
{
	//Don't load models with duplicate IDs
	for (auto it = modelMap.begin(); it != modelMap.end(); it++)
	{
		if (it->first == _id)
			return;
	}

	std::cout << "Loading Model: \"" << _id << "\", \"" << _fileName << "\", ";
	unsigned int totalVertices = 0, currentVertex = 0;

	Assimp::Importer import;
	const aiScene * scene;

	Renderable * model;

	bool hasNormals;

	model = new Renderable();

	scene = import.ReadFile(_fileName, aiProcess_Triangulate | aiProcess_SortByPType);

	hasNormals = false;

	for (int i = 0; i < scene->mNumMeshes; i++)
		totalVertices += scene->mMeshes[i]->mNumVertices;

	std::cout << totalVertices << " Vertices ...";
	Vec2 cursorPos = Vec2(wherex(), wherey());

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		Renderable::Mesh mesh;
				
		for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
		{
			Vec3 vertex = Vec3(scene->mMeshes[i]->mVertices[j].x, scene->mMeshes[i]->mVertices[j].y, scene->mMeshes[i]->mVertices[j].z);
			mesh.vertex.push_back(vertex);

			if (scene->mMeshes[i]->HasNormals())
			{
				Vec3 normal = Vec3(scene->mMeshes[i]->mNormals[j].x, scene->mMeshes[i]->mNormals[j].y, scene->mMeshes[i]->mNormals[j].z);
				mesh.normal.push_back(normal);
				hasNormals = true;
			}

			if (_useModelTextureMap && scene->mMeshes[i]->HasTextureCoords(0))
			{
				Vec2 texture = Vec2(scene->mMeshes[i]->mTextureCoords[0][j].x, scene->mMeshes[i]->mTextureCoords[0][j].y);
				mesh.textureMap.push_back(texture);
			}

			currentVertex++;

			if (currentVertex % 1000 == 0)
			{
				gotoxy(cursorPos.x, cursorPos.y);
				std::cout << (currentVertex / (float)totalVertices) * 100 << "%    ";
			}
		}

		model->meshes.push_back(mesh);
	}

	gotoxy(cursorPos.x, cursorPos.y);
	std::cout << "100%     \n";

	for (int m = 0; m < model->meshes.size(); m++)
	for (unsigned int i = 0; i < model->meshes[m].vertex.size(); i++)
		model->meshes[m].edge.push_back(i);


	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		for (int j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
		{
			model->meshes[i].face.push_back(scene->mMeshes[i]->mFaces[j].mNumIndices);
		}
	}

	if (!_useModelTextureMap)
		GenerateTextureMap(model);
	else
	{
		for (int m = 0; m < model->meshes.size(); m++)
		for (auto it = model->meshes[m].textureMap.begin(); it != model->meshes[m].textureMap.end(); it++)
		{
			masterTextureCoords.push_back(*it);
		}
	}

	/*if (!hasNormals)
		GenerateNormals(model);
	else
		GenerateNormals(model, false, false, true);*/

	model->drawMode = _mode;

	InsertModel(model, _id);
}

Renderable * ModelManager::getModel(string _id)
{
	return models.find(GetModelID(_id))->second;
}

Renderable * ModelManager::getModel(UINT32 _id)
{
	return models.find(_id)->second;
}

//void ModelManager::unloadModels()
//{
//	//deletes all models still in memory
//	for (auto it = models.begin(); it != models.end(); it++)
//	{
//		delete it->second;
//	}
//
//	models.clear();
//}

#define Cuboid_Vertex_0 Vec3(-_w, _h, _l)
#define Cuboid_Vertex_1 Vec3(_w, _h, _l)
#define Cuboid_Vertex_2 Vec3(_w, -_h, _l)
#define Cuboid_Vertex_3 Vec3(-_w, -_h, _l)
#define Cuboid_Vertex_4 Vec3(-_w, -_h, -_l)
#define Cuboid_Vertex_5 Vec3(_w, -_h, -_l)
#define Cuboid_Vertex_6 Vec3(_w, _h, -_l)
#define Cuboid_Vertex_7 Vec3(-_w, _h, -_l)
void ModelManager::CreateCuboid(string _id, float _w, float _h, float _l, bool _useCubeMap, float _uvRepeatX, float _uvRepeatY)
{
	//If id is already on model list then do nothing.
	auto search = modelMap.find(_id);
	if (search != modelMap.end())
		return;

	Renderable * cube;
	Renderable::Mesh mesh;	

	cube = new Renderable();

	cube->meshes.push_back(mesh);

	//ABD  DBC

	/* Face Back */
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_0);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_1);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_3);

	cube->meshes[0].vertex.push_back(Cuboid_Vertex_3);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_1);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_2);

	/* Face Front */
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_4);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_5);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_7);

	cube->meshes[0].vertex.push_back(Cuboid_Vertex_7);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_5);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_6);

	/* Face Right */
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_6);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_5);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_1);

	cube->meshes[0].vertex.push_back(Cuboid_Vertex_1);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_5);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_2);

	/* Face Left */
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_0);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_3);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_7);

	cube->meshes[0].vertex.push_back(Cuboid_Vertex_7);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_3);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_4);		
		
	/* Face Top */
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_7);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_6);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_0);
		
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_0);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_6);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_1);

	/* Face Bottom */
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_3);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_2);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_4);

	cube->meshes[0].vertex.push_back(Cuboid_Vertex_4);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_2);
	cube->meshes[0].vertex.push_back(Cuboid_Vertex_5);


	///* Face Back */		cube->edge.push_back(0);	cube->edge.push_back(1);	cube->edge.push_back(2);	cube->edge.push_back(3);
	///* Face Front */		cube->edge.push_back(4);	cube->edge.push_back(5);	cube->edge.push_back(6);	cube->edge.push_back(7);
	///* Face Left */		cube->edge.push_back(0);	cube->edge.push_back(3);	cube->edge.push_back(4);	cube->edge.push_back(7);
	///* Face Right */		cube->edge.push_back(6);	cube->edge.push_back(5);	cube->edge.push_back(2);	cube->edge.push_back(1);
	///* Face Top */		cube->edge.push_back(7);	cube->edge.push_back(6);	cube->edge.push_back(1);	cube->edge.push_back(0);
	///* Face Bottom */		cube->edge.push_back(3);	cube->edge.push_back(2);	cube->edge.push_back(5);	cube->edge.push_back(4);

	for (unsigned int i = 0; i < cube->meshes[0].vertex.size(); i++)
		cube->meshes[0].edge.push_back(i);

	for (int i = 0; i < 12; i++)
		cube->meshes[0].face.push_back(3);

	GenerateNormals(cube);

	if (!_useCubeMap)
		GenerateTextureMap(cube, _uvRepeatX, _uvRepeatY);
	else
		GenerateCubeMap(cube);

	InsertModel(cube, _id);

}

#define Skybox_Vertex_0 Vec3(-_size, -_size, _size)
#define Skybox_Vertex_1 Vec3(_size, -_size, _size)
#define Skybox_Vertex_2 Vec3(_size, _size, _size)
#define Skybox_Vertex_3 Vec3(-_size, _size, _size)
#define Skybox_Vertex_4 Vec3(-_size, _size, -_size)
#define Skybox_Vertex_5 Vec3(_size, _size, -_size)
#define Skybox_Vertex_6 Vec3(_size, -_size, -_size)
#define Skybox_Vertex_7 Vec3(-_size, -_size, -_size)
void ModelManager::CreateSkybox(string _id, float _size, bool _normalsOnBottom)
{
	//If id is already on model list then do nothing.
	auto search = modelMap.find(_id);
	if (search != modelMap.end())
		return;

	Renderable * skybox;
	Renderable::Mesh mesh;

	skybox = new Renderable();
	skybox->meshes.push_back(mesh);

	/* Face Back  */
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_0);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_1);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_3);

	skybox->meshes[0].vertex.push_back(Skybox_Vertex_3);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_1);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_2);

	/* Face Front */
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_4);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_5);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_7);

	skybox->meshes[0].vertex.push_back(Skybox_Vertex_7);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_5);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_6);

	/* Face Right */
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_6);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_5);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_1);

	skybox->meshes[0].vertex.push_back(Skybox_Vertex_1);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_5);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_2);
		
	/* Face Left */
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_0);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_3);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_7);

	skybox->meshes[0].vertex.push_back(Skybox_Vertex_7);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_3);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_4);
		
	/* Face Top */
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_7);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_6);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_0);
		
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_0);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_6);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_1);
		
	/* Face Bottom */
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_3);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_2);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_4);

	skybox->meshes[0].vertex.push_back(Skybox_Vertex_4);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_2);
	skybox->meshes[0].vertex.push_back(Skybox_Vertex_5);

	for (unsigned int i = 0; i < skybox->meshes[0].vertex.size(); i++)
		skybox->meshes[0].edge.push_back(i);

	for (int i = 0; i < 12; i++)
		skybox->meshes[0].face.push_back(3);

	GenerateNormals(skybox, true, _normalsOnBottom);
	GenerateCubeMap(skybox);

	InsertModel(skybox, _id); 
}

void ModelManager::CreatePyramid(string _id, float _w, float _h, float _l, float _uvRepeatX, float _uvRepeatY)
{
	//If id is already on model list then do nothing.
	auto search = modelMap.find(_id);
	if (search != modelMap.end())
		return;

	Renderable * pyramid;
	Renderable::Mesh mesh;

	pyramid = new Renderable();
	pyramid->meshes.push_back(mesh);

	pyramid->meshes[0].vertex.push_back(Vec3(_w, 0.0f, -_l));
	pyramid->meshes[0].vertex.push_back(Vec3(-_w, 0.0f, -_l));
	pyramid->meshes[0].vertex.push_back(Vec3(-_w, 0.0f, _l));
	pyramid->meshes[0].vertex.push_back(Vec3(_w, 0.0f, _l));

	pyramid->meshes[0].vertex.push_back(Vec3(0.0f, _h, 0.0f));
	pyramid->meshes[0].vertex.push_back(pyramid->meshes[0].vertex[3]);
	pyramid->meshes[0].vertex.push_back(pyramid->meshes[0].vertex[2]);

	pyramid->meshes[0].vertex.push_back(pyramid->meshes[0].vertex[4]);
	pyramid->meshes[0].vertex.push_back(pyramid->meshes[0].vertex[2]);
	pyramid->meshes[0].vertex.push_back(pyramid->meshes[0].vertex[1]);

	pyramid->meshes[0].vertex.push_back(pyramid->meshes[0].vertex[4]);
	pyramid->meshes[0].vertex.push_back(pyramid->meshes[0].vertex[1]);
	pyramid->meshes[0].vertex.push_back(pyramid->meshes[0].vertex[0]);

	pyramid->meshes[0].vertex.push_back(pyramid->meshes[0].vertex[4]);
	pyramid->meshes[0].vertex.push_back(pyramid->meshes[0].vertex[0]);
	pyramid->meshes[0].vertex.push_back(pyramid->meshes[0].vertex[3]);

	////	/* Bottom */pyramid->edge.push_back(0);	 pyramid->edge.push_back(1);	pyramid->edge.push_back(2);		pyramid->edge.push_back(3);
	////	/* Face 1 */pyramid->edge.push_back(0);	 pyramid->edge.push_back(1);	pyramid->edge.push_back(4);
	////	/* Face 2 */pyramid->edge.push_back(1);	 pyramid->edge.push_back(2);	pyramid->edge.push_back(4);
	////	/* Face 3 */pyramid->edge.push_back(2);	 pyramid->edge.push_back(3);	pyramid->edge.push_back(4);
	////	/* Face 4 */pyramid->edge.push_back(3);	 pyramid->edge.push_back(0);	pyramid->edge.push_back(4);

	for (unsigned int i = 0; i < pyramid->meshes[0].vertex.size(); i++)
		pyramid->meshes[0].edge.push_back(i);

	pyramid->meshes[0].face.push_back(4);
	for (int i = 1; i < 5; i++)pyramid->meshes[0].face.push_back(3);

	GenerateNormals(pyramid);
	GenerateTextureMap(pyramid, _uvRepeatX, _uvRepeatY);

	InsertModel(pyramid, _id);
}

void ModelManager::CreatePlane(string _id, float _h, float _w, float _uvRepeatX, float _uvRepeatY)
{
	//If id is already on model list then do nothing.
	auto search = modelMap.find(_id);
	if (search != modelMap.end())
		return;

	Renderable * plane;
	Renderable::Mesh mesh;

	plane = new Renderable();
	plane->meshes.push_back(mesh);

	_h /= 2.0f;
	_w /= 2.0f;

	plane->meshes[0].vertex.push_back(Vec3(-_w, _h, 0.0f));	//A
	plane->meshes[0].vertex.push_back(Vec3(_w, _h, 0.0f));	//B
	plane->meshes[0].vertex.push_back(Vec3(-_w, -_h, 0.0f)); //D
		
	plane->meshes[0].vertex.push_back(Vec3(-_w, -_h, 0.0f)); //D
	plane->meshes[0].vertex.push_back(Vec3(_w, _h, 0.0f));	//B
	plane->meshes[0].vertex.push_back(Vec3(_w, -_h, 0.0f)); //C

	for (unsigned int i = 0; i < plane->meshes[0].vertex.size(); i++)
		plane->meshes[0].edge.push_back(i);

	for (int i = 0; i < 2; i++)
		plane->meshes[0].face.push_back(3);

	GenerateNormals(plane);
	GenerateTextureMap(plane, _uvRepeatX, _uvRepeatY);

	InsertModel(plane, _id);
}

void ModelManager::InsertModel(Renderable* _renderable, string _id)
{
	models.insert(std::pair<UINT32, Renderable *>(nextModelID, _renderable));
	modelMap.insert(std::pair<string, int>(_id, nextModelID));
	nextModelID++;

	for (int m = 0; m < _renderable->meshes.size(); m++)
	{
		_renderable->meshes[m].offsetVertex = masterVectorList.size();

		//for (auto it = _renderable->meshes[m].vertex.begin(); it != _renderable->meshes[m].vertex.end(); it++)
		for (int i = 0; i < _renderable->meshes[m].vertex.size(); i++)
		{
			masterVectorList.push_back(_renderable->meshes[m].vertex[i]);
			masterNormalList.push_back(_renderable->meshes[m].normal[i]);
		}

		for (int i = 0; i < _renderable->meshes[m].edge.size(); i++)
			_renderable->meshes[m].edge[i] += _renderable->meshes[m].offsetVertex;
	}
}

void ModelManager::GenerateTextureMap(Renderable* _renderable, float _uvRepeatX, float _uvRepeatY)
{
	for (int m = 0; m < _renderable->meshes.size(); m++)
	{
		for (int i = 0; i < _renderable->meshes[m].face.size(); i++)
		{
			if (_renderable->meshes[m].face[i] == 4)
			{
				_renderable->meshes[m].textureMap.push_back(Vec2(0, 0));
				_renderable->meshes[m].textureMap.push_back(Vec2(_uvRepeatX, 0));
				_renderable->meshes[m].textureMap.push_back(Vec2(_uvRepeatX, _uvRepeatY));
				_renderable->meshes[m].textureMap.push_back(Vec2(0, _uvRepeatY));
			}
			else if (_renderable->meshes[m].face[i] == 3)
			{
				if (i % 2 == 0)
				{
					_renderable->meshes[m].textureMap.push_back(Vec2(0, _uvRepeatY));
					_renderable->meshes[m].textureMap.push_back(Vec2(_uvRepeatX, _uvRepeatY));
					_renderable->meshes[m].textureMap.push_back(Vec2(0, 0));
				}
				else
				{
					_renderable->meshes[m].textureMap.push_back(Vec2(0, 0));
					_renderable->meshes[m].textureMap.push_back(Vec2(_uvRepeatX, _uvRepeatY));
					_renderable->meshes[m].textureMap.push_back(Vec2(_uvRepeatX, 0));
				}
			}
		}

		for (auto it = _renderable->meshes[m].textureMap.begin(); it != _renderable->meshes[m].textureMap.end(); it++)
		{
			masterTextureCoords.push_back(*it);
		}
	}
}

void  ModelManager::GenerateCubeMap(Renderable* _renderable)
{
	//Generate Cubemap 
	{
		//Back
		_renderable->meshes[0].textureMap.push_back(Vec2(0.5f, 1 / 3.0f));
		_renderable->meshes[0].textureMap.push_back(Vec2(0.25f, 1 / 3.0f));
		_renderable->meshes[0].textureMap.push_back(Vec2(0.5f, 2 / 3.0f));

		_renderable->meshes[0].textureMap.push_back(Vec2(0.5f, 2 / 3.0f));
		_renderable->meshes[0].textureMap.push_back(Vec2(0.25f, 1 / 3.0f));
		_renderable->meshes[0].textureMap.push_back(Vec2(0.25f, 2 / 3.0f));

		//Front
		_renderable->meshes[0].textureMap.push_back(Vec2(0.75f, 2 / 3.0f));
		_renderable->meshes[0].textureMap.push_back(Vec2(1, 2 / 3.0f));
		_renderable->meshes[0].textureMap.push_back(Vec2(0.75f, 1 / 3.0f));

		_renderable->meshes[0].textureMap.push_back(Vec2(0.75f, 1 / 3.0f));
		_renderable->meshes[0].textureMap.push_back(Vec2(1, 2 / 3.0f));
		_renderable->meshes[0].textureMap.push_back(Vec2(1, 1 / 3.0f));

		//Left
		_renderable->meshes[0].textureMap.push_back(Vec2(0, 1 / 3.0f));//3
		_renderable->meshes[0].textureMap.push_back(Vec2(0, 2 / 3.0f));//4
		_renderable->meshes[0].textureMap.push_back(Vec2(0.25f, 1 / 3.0f));//2

		_renderable->meshes[0].textureMap.push_back(Vec2(0.25f, 1 / 3.0f));//2
		_renderable->meshes[0].textureMap.push_back(Vec2(0, 2 / 3.0f));//4
		_renderable->meshes[0].textureMap.push_back(Vec2(0.25f, 2 / 3.0f));//1

		//Right
		_renderable->meshes[0].textureMap.push_back(Vec2(0.5f, 1 / 3.0f));//3
		_renderable->meshes[0].textureMap.push_back(Vec2(0.5f, 2 / 3.0f));//4
		_renderable->meshes[0].textureMap.push_back(Vec2(0.75f, 1 / 3.0f));//2

		_renderable->meshes[0].textureMap.push_back(Vec2(0.75f, 1 / 3.0f));//2
		_renderable->meshes[0].textureMap.push_back(Vec2(0.5f, 2 / 3.0f));//4
		_renderable->meshes[0].textureMap.push_back(Vec2(0.75f, 2 / 3.0f));//1

		//Bottom 
		_renderable->meshes[0].textureMap.push_back(Vec2(0.5f, 0));
		_renderable->meshes[0].textureMap.push_back(Vec2(0.25f, 0));
		_renderable->meshes[0].textureMap.push_back(Vec2(0.5f, 1 / 3.0f));
																 
		_renderable->meshes[0].textureMap.push_back(Vec2(0.5f, 1 / 3.0f));
		_renderable->meshes[0].textureMap.push_back(Vec2(0.25f, 0));
		_renderable->meshes[0].textureMap.push_back(Vec2(0.25f, 1 / 3.0f));
														 
		//Top										 
		_renderable->meshes[0].textureMap.push_back(Vec2(0.50f, 2 / 3.0f));
		_renderable->meshes[0].textureMap.push_back(Vec2(0.25f, 2 / 3.0f));
		_renderable->meshes[0].textureMap.push_back(Vec2(0.5f, 1));
														 
		_renderable->meshes[0].textureMap.push_back(Vec2(0.5f, 1));
		_renderable->meshes[0].textureMap.push_back(Vec2(0.25f, 2 / 3.0f));
		_renderable->meshes[0].textureMap.push_back(Vec2(0.25f, 1));
	}

	for (auto it = _renderable->meshes[0].textureMap.begin(); it != _renderable->meshes[0].textureMap.end(); it++)
	{
		masterTextureCoords.push_back(*it);
	}
}

void ModelManager::GenerateNormals(Renderable* _renderable, bool _reverse, bool _normalsOnBottom, bool _onlyFaceNormals)
{
	for (int m = 0; m < _renderable->meshes.size(); m++)
	{
		int offset = 0;

		std::vector<Vec3> normals;

		for (int i = 0; i < _renderable->meshes[m].face.size(); i++)
		{
			Vec3 normal;

			if (_normalsOnBottom)
				normal = Vec3(0, 1, 0);
			else
			{
				Vec3 p1 = _renderable->meshes[m].vertex[offset];
				Vec3 p2 = _renderable->meshes[m].vertex[offset + 1];
				Vec3 p3 = _renderable->meshes[m].vertex[offset + 2];

				Vec3 A = p1 - p2;
				Vec3 B = p2 - p3;

				normal = Vec3::cross(A, B).Normalized();

				float dotP = Vec3::dot(p1, normal);

				if (Vec3::dot(p1, normal) > 0)
					normal *= -1;
			}			

			if (!_reverse)
				normal *= -1;

			for (int iii = 0; iii < _renderable->meshes[m].face[i]; iii++)
			{
				normals.push_back(normal);
				_renderable->meshes[m].faceNormal.push_back(normal);

				if (!_onlyFaceNormals)
					_renderable->meshes[m].normal.push_back(normal);
			}

			offset += _renderable->meshes[m].face[i];
		}

		//Change from face normals to vertex normals
		if (!_onlyFaceNormals)
		{
			struct VertexIndex
			{
				Vec3 vertex;
				std::vector<int> indices;

				VertexIndex(Vec3 _vertex, int _indices)
					: vertex(_vertex){
					indices.push_back(_indices);
				}
			};

			std::vector<VertexIndex> _vertexList;

			//Populate with all unique vertex and their index
			for (int i = 0; i < _renderable->meshes[m].vertex.size(); i++)
			{
				bool isInVector = false;

				for (int j = 0; j < _vertexList.size(); j++)
				{
					if (_renderable->meshes[m].vertex[i] == _vertexList[j].vertex)
					{
						isInVector = true;
						_vertexList[j].indices.push_back(i);
					}
				}

				if (!isInVector)
					_vertexList.push_back(VertexIndex(_renderable->meshes[m].vertex[i], i));
			}

			//Calculate Vertex Normals
			for (int i = 0; i < _vertexList.size(); i++)
			{
				Vec3 _normal = Vec3();

				for (int j = 0; j < _vertexList[i].indices.size(); j++)
				{
					_normal += normals[_vertexList[i].indices[j]];
				}

				_normal.NormalizeThis();

				for (int j = 0; j < _vertexList[i].indices.size(); j++)
					_renderable->meshes[m].normal[_vertexList[i].indices[j]] = _normal;
			}
		}
	}
}

void ModelManager::PushModels()
{
	//Clear Buffers
	if (areBuffersInitialized)
	{
		GLuint zero = 0;

		//OpenGL 4.5.0
		//glClearNamedBufferData(Buffers[0], GL_RGB32F, GL_RGB, GL_UNSIGNED_INT, &zero);
		//glClearNamedBufferData(Buffers[1], GL_RG32F, GL_RG, GL_UNSIGNED_INT, &zero);
		//glClearNamedBufferData(Buffers[2], GL_RGB32F, GL_RGB, GL_UNSIGNED_INT, &zero);

		//OpenGL 4.3.0
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[VERTEX_BUFFER]); glClearBufferData(GL_ARRAY_BUFFER, GL_RGB32F, GL_RGB, GL_UNSIGNED_INT, &zero);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[TEXTURE_BUFFER]); glClearBufferData(GL_ARRAY_BUFFER, GL_RG32F, GL_RG, GL_UNSIGNED_INT, &zero);
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[NORMAL_BUFFER]); glClearBufferData(GL_ARRAY_BUFFER, GL_RGB32F, GL_RGB, GL_UNSIGNED_INT, &zero);

		glDeleteTextures(numberOfTextures, textureArray);
	}

	/*  VERTICES  */
	if (masterVectorList.size() > 0)
	{
		int arraySize = masterVectorList.size() * 3 * 4; //Each vector contains 3 floats (which are 4 bytes)
		float * newMasterList = new float[masterVectorList.size() * 3];

		//populate vector list
		{
			int i = 0;
			std::cout << "Loading Vertices: " << (i / (float)masterVectorList.size()) * 100 << "%";

			for (i = 0; i < masterVectorList.size(); i++)
			{
				newMasterList[i * 3] = masterVectorList[i].x;
				newMasterList[3 * i + 1] = masterVectorList[i].y;
				newMasterList[3 * i + 2] = masterVectorList[i].z;

				if (i % 1000 == 0)
				{
					gotoxy(18, wherey());
					std::cout << "                                                     ";
					gotoxy(18, wherey());
					std::cout << (i / (float)masterVectorList.size()) * 100 << "%";
				}
			}

			gotoxy(0, wherey());
			std::cout << "                                                                        ";
			gotoxy(0, wherey());
			std::cout << "Loading Vertices: 100%\n";

			glBindBuffer(GL_ARRAY_BUFFER, Buffers[VERTEX_BUFFER]);
			glBufferData(GL_ARRAY_BUFFER, arraySize, newMasterList, GL_STATIC_DRAW);

			std::cout << "Loaded " << i << " Vertices.\n";
		}
		delete[] newMasterList;
	}

	/*  TEXTURES  */
	if (textureArray)
	{
		delete[] textureArray;
		textureArray = nullptr;
	}

	numberOfTextures = textures.size();
	textureArray = new GLuint[numberOfTextures];
	glGenTextures(numberOfTextures, textureArray);

	//Pass texture id to textures
	int i = 0;
	for (auto it = textures.begin(); it != textures.end(); it++)
	{
		std::cout << "Loading Texture #" << i << " \"" << textures[i]->ID << "\"...";

		it->second->address = textureArray[i];

		switch (it->second->dataType)
		{
		case Texture::TextureDataType::Float:
			glBindTexture(GL_TEXTURE_2D, it->second->address);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, it->second->width, it->second->height, 0, GL_RGBA, GL_FLOAT, it->second->pixelData);
			break;
		case Texture::TextureDataType::UnsignedByte:
			glBindTexture(GL_TEXTURE_2D, it->second->address);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, it->second->width, it->second->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, it->second->pixelData);
			break;
		case Texture::TextureDataType::UnsignedByte_3D:
			glBindTexture(GL_TEXTURE_3D, it->second->address);
			glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, it->second->width, it->second->height, it->second->depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, it->second->pixelData);
			break;
		}

		std::cout << " done!\n";

		i++;
	}

	if (masterTextureCoords.size() > 0)
	{
		float * newMasterTextureList = new float[masterTextureCoords.size() * 2];
		for (int i = 0; i < masterTextureCoords.size(); i++)
		{
			newMasterTextureList[2 * i + 0] = masterTextureCoords[i].x;
			newMasterTextureList[2 * i + 1] = masterTextureCoords[i].y * -1;
		}

		glBindBuffer(GL_ARRAY_BUFFER, Buffers[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, masterTextureCoords.size() * 2 * 4, newMasterTextureList, GL_STATIC_DRAW);

		delete[] newMasterTextureList;
	}

	/* NORMALS */
	if (masterNormalList.size() > 0)
	{
		int arraySize = masterNormalList.size() * 3 * 4; //Each vector contains 3 floats (which are 4 bytes)
		float * newMasterList = new float[masterNormalList.size() * 3];

		//populate vector list
		for (int i = 0; i < masterNormalList.size(); i++)
		{
			newMasterList[i * 3] = masterNormalList[i].x;
			newMasterList[3 * i + 1] = masterNormalList[i].y;
			newMasterList[3 * i + 2] = masterNormalList[i].z;
		}

		glBindBuffer(GL_ARRAY_BUFFER, Buffers[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, arraySize, newMasterList, GL_STATIC_DRAW);

		delete[] newMasterList;
	}
}

void ModelManager::UnloadModels()
{
	//Models
	for (auto it = models.begin(); it != models.end(); it++)
	{
		if (it->second)
			delete it->second;
	}

	models.clear();
	modelMap.clear();
	nextModelID = 0;

	masterVectorList.clear();
	masterNormalList.clear();
	masterColourList.clear();
	masterTextureCoords.clear();

	////Textures
	//for (auto it = textures.begin(); it != textures.end(); it++)
	//{
	//	if (it->second)
	//		delete it->second;
	//}

	textures.clear();
	textureMap.clear();
	nextTextureID = 0;

	//Repopulate default models / textures
	GenerateDefaultContent();
}

void ModelManager::createTexture(string _id, void* _pixelData, UINT32 _textureWidth, UINT32 _textureHeight)
{
	textures.insert(std::pair<int, Texture*>(nextTextureID, new Texture(_pixelData, _textureWidth, _textureHeight, _id)));
	textureMap.insert(std::pair<string, int>(_id, nextTextureID));

	nextTextureID++;
}

std::string ModelManager::loadTexture(string _id, string _fileName)
{
	//Don't load textures with duplicate IDs
	for (auto it = textureMap.begin(); it != textureMap.end(); it++)
	{
		if (it->first == _id)
			return _id;
	}

	SDL_Surface *texture = IMG_Load(_fileName.c_str());

	if (texture == NULL)		{
		printf("Image failed to load! SDL_image Error: %s\n", IMG_GetError());
		return "error";
	}

	Texture * t = new Texture(texture, _id);
	textures.insert(std::pair<UINT32, Texture*>(nextTextureID, t));
	textureMap.insert(std::pair<string, UINT32>(_id, nextTextureID));

	nextTextureID++;

	return _id;
}

UINT32 ModelManager::InsertTexture(Texture* texture_)
{
	UINT32 _id = nextTextureID;
	textures.insert(std::pair<UINT32, Texture*>(nextTextureID, texture_));
	
	nextTextureID++;

	return _id;
}

Texture* ModelManager::getTexture(string _id) const
{
	return textures.find(GetTextureID(_id))->second;
}

Texture* ModelManager::getTexture(UINT32 _id) const
{
	return textures.find(_id)->second;
}

UINT32 ModelManager::GetModelID(string _name) const
{
	return modelMap.find(_name)->second;
}

UINT32 ModelManager::GetTextureID(string _name) const
{
	return textureMap.find(_name)->second;
}

int ModelManager::GetTextureWidth(string _name) const
{
	Texture* tex = getTexture(_name);

	if (tex != nullptr)
		return tex->width;
	else
		return -1;
}

int ModelManager::GetTextureWidth(UINT32 _id) const
{
	Texture* tex = getTexture(_id);

	if (tex != nullptr)
		return tex->width;
	else
		return -1;
}


int ModelManager::GetTextureHeight(string _name) const
{
	Texture* tex = getTexture(_name);
	if (tex != nullptr)
		return tex->height;
	else
		return -1;
}

int ModelManager::GetTextureHeight(UINT32 _id) const
{
	Texture* tex = getTexture(_id);
	if (tex != nullptr)
		return tex->height;
	else
		return -1;
}