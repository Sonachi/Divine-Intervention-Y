#include "Level.h"
#include "OpenGLUtilities.h"
#include "ModelManager.h"
#include "LightManager.h"
#include "GuiElement.h"

#include "Button.h"
#include "GuiImage.h"
#include "Label.h"
#include "RenderableComponent.h"
#include "Shader.h"

Level::Level() : ambientLightColor(Colour(0.5f, 0.5f, 0.5f, 1))
{
	mainCamera = new Camera(this);
	currentCamera = mainCamera;

	hasCalledInit = false;
}

Level::~Level()
{
	for (auto it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		delete *it;
	}
	gameObjects.clear();

	for (auto it = guiElements.begin(); it != guiElements.end(); it++)
	{
		switch ((*it)->type)
		{
		case GuiType::BUTTON:
			delete (Button*) *it;
			break;
		case GuiType::IMAGE:
			delete ((GuiImage*)*it);
			break;
		case GuiType::LABEL:
			delete ((Label*)*it);
			break;
		}		
	}
	guiElements.clear();
}

void Level::init()
{
	//Will only load models/textures of a level once
	if (!hasCalledInit)
	{
		LoadContent();
		hasCalledInit = true;

		Models->PushModels();
	}
}

void Level::LoadContent(){}

void Level::LevelRender()
{
	glProgramUniform4f(Generic_Shader::_GetInstance()->GetProgram(), Generic_Shader::_GetInstance()->ambient_Location, ambientLightColor.r, ambientLightColor.g, ambientLightColor.b, ambientLightColor.a);

	//Pre-Render
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (!gameObjects[i]->isFlagged)
			gameObjects[i]->PreRender();
	}
	LightManager::getInstance()->PushLights(mainCamera->position);

	//New Rendering method

	RenderableComponent::DrawRenderables();

	//UI
	for (int i = 0; i < guiElements.size(); i++)
	{
		guiElements[i]->Draw();
	}

	LevelCleanUp();
}

void Level::LevelUpdate(float _timeStep)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (!gameObjects[i]->isFlagged)
			gameObjects[i]->Update(_timeStep);
	}

	for (int i = 0; i < guiElements.size(); i++)
	{
		guiElements[i]->Update(_timeStep);
	}
}

void Level::LevelCleanUp()
{
	for (int i = gameObjects.size() - 1; i >= 0; i--)
	{
		if (gameObjects[i]->isFlagged)
		{
			auto it = (gameObjects.begin() + i);
			gameObjects.erase(it);
		}
	}

	gameObjects.shrink_to_fit();
}

GameObject * Level::FindGameObjectWithTag(Tag _tag)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->hasTag(_tag))return gameObjects[i];
	}
}

void Level::SetAmbientColor(Colour _c)
{
	ambientLightColor = _c;
}

void Level::SetCamera(Camera * _c)
{
	currentCamera = _c;
}