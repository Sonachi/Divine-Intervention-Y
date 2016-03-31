//Author:	Mathieu Violette
//Date:		01/14/2016

#include "DrScottLevel.h"

#include <SDL.h>
#include <sstream>
#include <Flipbook.h>
#include <Rigidbody.h>
#include <PhysicsWorld.h>

#include "HeadBob.h"
#include "FPS_Controls.h"

DrScottLevel::DrScottLevel()
{
	updates = 0;
}

void DrScottLevel::LoadContent()
{
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

	/* TEXTURES */
	
	const int texSize = 13 * 13 * 4;
	float pixelDataWhite[texSize];
	for (int i = 0; i < texSize; i++)
	{
		pixelDataWhite[i] = 1.0f;
	}
	Models->createTexture("white", pixelDataWhite, 1, 1);

	float GC[]
	{
		1, 1, 1, 1,		0, 1, 0, 1,
		0, 1, 0, 1,		1, 1, 1, 1
	};
	Models->createTexture("greenCheckerz", GC, 2, 2);

	float pixelDataBlack[]
	{
		0.1f, 0.1f, 0.1f, 1.0f
	};
	Models->createTexture("black", pixelDataBlack, 1, 1);

	float pixelDataOrange[]
	{
		1, 0.5f, 0, 1
	};
	Models->createTexture("orange", pixelDataOrange, 1, 1);

	Models->loadTexture("grass", "Images/grass.png");
	Models->loadTexture("skybox", "Images/Day_Skybox.png");

		/* Add Components */
	new RenderableComponent("frostBody", "greenCheckerz", Frosty, new Material(1, 1, 1));
	new RenderableComponent("frostFace", "black", frosty_face, new Material(1, 1, 1));
	new RenderableComponent("frostNose", "orange", frosty_nose, new Material(1, 1, 1));

	new RenderableComponent("ground", "grass", ground, new Material(1, 1, 1));
	RenderableComponent *skyRC = new RenderableComponent("skybox", "skybox", skybox);
	skyRC->SetEffecctedByLight(1, 0, 0);

	new Light(light, Colour(1.0f, 1.0f, 1.0f), Light::Directional);

	{
		/* Space Controls */
		/*BindKey(SDLK_b, BindKey(SDLK_v, new CameraUp(currentCamera, SDLK_SPACE)));
		new CameraDown(currentCamera, SDLK_x);

		new CameraForward(currentCamera, SDLK_w);
		new CameraBackward(currentCamera, SDLK_s);

		new CameraTurnLeft(currentCamera, SDLK_a);
		new CameraTurnRight(currentCamera, SDLK_d);

		new CameraTurnDown(currentCamera, SDLK_c);
		new CameraTurnUp(currentCamera, SDLK_z);

		new CameraRollLeft(currentCamera, SDLK_q);
		new CameraRollRight(currentCamera, SDLK_e);

		new CameraTurnRight(currentCamera, SDLK_RIGHT);
		new CameraTurnLeft(currentCamera, SDLK_LEFT);*/
		/*new FPS_MOVE_UP(ground, SDLK_y);
		new FPS_MOVE_DOWN(ground, SDLK_h);*/

		/* FPS CONTROLS */
		SDL_SetRelativeMouseMode(SDL_TRUE); //Traps Mouse in Window (centre)

	}
}

DrScottLevel::~DrScottLevel()
{
}

void DrScottLevel::LevelUpdate(float _timeStep)
{
	Level::LevelUpdate(_timeStep);

	if (updates > 10)
		FPS_Controls::Update(currentCamera, _timeStep);
	else
		updates++;

	Frosty->Rotate(Quat(90 * _timeStep * 3.14159f / 180.0f, Vec3(0, 1, 0)));

	//std::stringstream ss;
	//ss << "FPS: " << Game::GetInstance()->GetFPS();
	//fpsLabel->SetText(ss.str());

	skybox->position = currentCamera->position;

}

void DrScottLevel::DebugRender()
{

}

