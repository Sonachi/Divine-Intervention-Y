//Author:	Mathieu Violette
//Date:		1/14/2015

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

/*
A class that inherits from Level so the user can specify the behaviour of
their game and create their own gameObjects
*/

class DrScottLevel : public Level
{
private:
	int updates;
public:
	DrScottLevel();
	~DrScottLevel();

	GameObject *skybox, *ground, *Frosty, *frosty_face, *frosty_nose;
	GameObject *light;

	void LoadContent() override;
	void DebugRender() override;
	void LevelUpdate(float _timeStep) override;

};