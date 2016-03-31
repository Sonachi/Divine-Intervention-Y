#pragma once
#include "Camera.h"
#include <vector>
#include "Colour.h"

class GuiElement;

/*
	A level acts as a scene in the game. All game objects are specific to a level
	and the level will update, render and delete all its own gameObjects so the user
	need not worry about it.
*/
class Level
{
public:
	Level();
	~Level();

	//Used to initialize all the models / textures in a level.
	void init();

	//Virtual function child classes use to load models / textures.
	//init() will call this function. 
	virtual void LoadContent();

	GLuint cameraLocation;

	//A camera is a default gameObject in a level
	Camera * mainCamera;
	Camera * currentCamera;

	//Lists that call all updates/renders for gameObjects in the game
	//As well as a list that will collect all flagged gameObjects and delete them
	std::vector<GameObject *> gameObjects;

	std::vector<GuiElement *> guiElements;

	//Renders and updates all gameObjects in the game
	void LevelRender();
	virtual void LevelUpdate(float _timeStep);

	GameObject * FindGameObjectWithTag(Tag _tag);

	//Sets the current camera
	void SetCamera(Camera * _c);

	//Will delete all gameObjects that have been flagged for deletion
	void LevelCleanUp();

	void SetAmbientColor(Colour _color);
protected:
	virtual void DebugRender(){};
	Colour ambientLightColor;
private:
	bool hasCalledInit;
};

