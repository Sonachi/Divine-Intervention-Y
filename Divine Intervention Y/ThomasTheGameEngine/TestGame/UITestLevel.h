//Author:	Mathieu Violette
//Date:		1/20/2015

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
#include <Button.h>

#include "TestGame.h"

/*
A class that inherits from Level so the user can specify the behaviour of
their game and create their own gameObjects
*/

class UITestLevel : public Level
{
public:
	UITestLevel();
	~UITestLevel();

	GameObject *skybox, *ground, *Frosty, *frosty_face, *frosty_nose;
	GameObject *light;
	Button *button;
	Label *label;
	GuiImage *image;

	int buttonCounter;

	void LoadContent() override;
	void DebugRender() override;
	void LevelUpdate(float _timeStep) override;

};