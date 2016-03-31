//Author:	Mathieu Violette
//Date:		01/14/2016

#include "UITestLevel.h"

#include <SDL.h>
#include <sstream>
#include <Flipbook.h>
#include <Rigidbody.h>
#include <PhysicsWorld.h>

#include "HeadBob.h"
#include "CounterButton.h"

UITestLevel::UITestLevel()
{
}

void UITestLevel::LoadContent()
{
	buttonCounter = 0;

	currentCamera->position = Vec3(0, 3.5f, -8);
	ambientLightColor = Colour(0.1f, 0.1f, 0.1f);

	Frosty = new GameObject(this, Vec3(0, 1, 0));
	frosty_face = new GameObject(this, Frosty->position);
	frosty_nose = new GameObject(this, Frosty->position);

	light = new GameObject(this, Vec3(0, 60, -60));
	skybox = new GameObject(this, currentCamera->position);
	ground = new GameObject(this, Vec3(0, 0, 0));

	ground->LookAt(ground->position + Vec3(0, 1, 0));
	light->LookAt(Vec3(0, 0, 0));

	Frosty->addChild(frosty_face);
	Frosty->addChild(frosty_nose);
	Frosty->Rotate(Quat(180 * 3.14159f / 180.0f, Vec3(0, 1, 0)));

	/* CREATE MODELS */
	ModelManager::getInstance()->loadModel("frostBody", "Models/frosty_body.obj", false, ModelManager::Draw_Mode::CW);
	ModelManager::getInstance()->loadModel("frostFace", "Models/frosty_face_hat.obj", false, ModelManager::Draw_Mode::CW);
	ModelManager::getInstance()->loadModel("frostNose", "Models/frosty_nose.obj", false, ModelManager::Draw_Mode::CW);

	ModelManager::getInstance()->CreateSkybox("skybox", 200);
	ModelManager::getInstance()->CreatePlane("ground", 100, 100, 32, 32);

	/* CREATE TEXTURES */
	float pixelDataWhite[]
	{
		1.0f, 1.0f, 1.0f, 1.0f
	};
	ModelManager::getInstance()->createTexture("white", pixelDataWhite, 1, 1);

	float pixelDataBlack[]
	{
		0.1f, 0.1f, 0.1f, 1.0f
	};
	ModelManager::getInstance()->createTexture("black", pixelDataBlack, 1, 1);

	float pixelDataOrange[]
	{
		1, 0.5f, 0, 1
	};
	ModelManager::getInstance()->createTexture("orange", pixelDataOrange, 1, 1);

	ModelManager::getInstance()->loadTexture("grass", "Images/grass.png");
	ModelManager::getInstance()->loadTexture("skybox", "Images/Day_Skybox.png");

	ModelManager::getInstance()->loadTexture("unpressed", "Images/Buttons/Test_Unpressed.png");
	ModelManager::getInstance()->loadTexture("pressed", "Images/Buttons/Test_Pressed.png");
	ModelManager::getInstance()->loadTexture("hover", "Images/Buttons/Test_Hover.png");

	ModelManager::getInstance()->loadTexture("doge", "Images/doge.png");


	FontManager::getInstance()->GenerateFont("font", 36, "Font/DroidSans.ttf");
	label = new Label(this, "Label", FontManager::getInstance()->GetFont("font"), Vec2(0, 150), ScreenAnchor::CENTER);

	image = new GuiImage(this, "doge", Vec2(0, -150), ScreenAnchor::CENTER);
	image->Scale(Vec3(0.25f, 0.25f, 1));

	button = new CounterButton(&buttonCounter, this, Vec2(), "unpressed", "pressed", "hover", ScreenAnchor::CENTER);


	/* Add Components */
	new RenderableComponent("frostBody", "white", Frosty, new Material(1, 1, 1));
	new RenderableComponent("frostFace", "black", frosty_face, new Material(1, 1, 1));
	new RenderableComponent("frostNose", "orange", frosty_nose, new Material(1, 1, 1));

	new RenderableComponent("ground", "grass", ground, new Material(1, 1, 1));
	RenderableComponent *skyRC = new RenderableComponent("skybox", "skybox", skybox);
	skyRC->SetEffecctedByLight(1, 0, 0);

	new Light(light, Colour(1.0f, 1.0f, 1.0f), Light::Directional);
}

UITestLevel::~UITestLevel()
{
}

void UITestLevel::LevelUpdate(float _timeStep)
{
	Level::LevelUpdate(_timeStep);

	Frosty->Rotate(Quat(90 * _timeStep * 3.14159f / 180.0f, Vec3(0, 1, 0)));
	
	std::stringstream ss;
	ss << "Times Button Has been pressed: " << buttonCounter;
	label->SetText(ss.str());

	skybox->position = currentCamera->position;
}

void UITestLevel::DebugRender()
{

}

