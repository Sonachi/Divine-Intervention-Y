//Author:	Mathieu Violette, Nathan Senter
//Date:		10/21/2015, 11/24,2015(NS)
//Example Level

#pragma once

#include <Level.h>
#include <ModelManager.h>
#include <RenderableComponent.h>
#include <Texture.h>
#include <Light.h>
#include <Billboard.h>
#include <ParticleSystem.h>
#include <Label.h>
#include <FontManager.h>
#include <Flipbook.h>
#include <GuiImage.h>

#include "TestGame.h"
#include "Torch.h"

/*
A class that inherits from Level so the user can specify the behaviour of
their game and create their own gameObjects
*/

class TestLevel : public Level
{
public:
	TestLevel();
	~TestLevel();

	GameObject *cube, *lilCube, *light, *lightAnchor, *soBright, *ground, *flashLight, *skybox;
	GameObject *teddy, *bear, *lilbear, *InsideCube, *OutsideCube, *dragon, *dragonLight;
	GameObject *headbob;
	Label *label, *fpsLabel;

	//Billboard * animationGuy;
	Torch * torch;

	void LoadContent() override;
	void DebugRender() override;
	void LevelUpdate(float _timeStep) override;

};