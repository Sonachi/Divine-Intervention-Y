//Author:	Mathieu Violette
//Date:		10/21/2015
//Example Level

#include "TestLevel.h"

#include <SDL.h>
#include <sstream>
#include <Flipbook.h>
#include <Rigidbody.h>
#include <PhysicsWorld.h>
#include <InputHandler.h>

#include "HeadBob.h"
#include "FPS_Controls.h"

//#define BUFFER_OFFSET(i) ((void*)(i))
//
//enum VAO_IDs { Triangles, NumVAOs };
//enum Buffer_IDs { ArrayBuffer, NumBuffers };
//enum Attrib_IDs { vPosition = 0 };
//
//GLuint VAOs[NumVAOs];
//GLuint Buffers[NumBuffers];

TestLevel::TestLevel()
{
}

void TestLevel::LoadContent()
{
	currentCamera->position = Vec3(0, 3.5f, -8);
	headbob = new HeadBob(this, currentCamera->position, currentCamera);

	ambientLightColor = Colour(0.1f, 0.1f, 0.1f);


	lilCube = new GameObject(this, Vec3(2, 0.5f, 0));
	light = new GameObject(this, Vec3(0, 60, 0));
	lightAnchor = new GameObject(this, Vec3());

	ground = new GameObject(this, Vec3(0, 0, 0));
	flashLight = new GameObject(this, currentCamera->position + Vec3(0, 0, -1));
	skybox = new GameObject(this, currentCamera->position);
	//teddy = new GameObject(this, Vec3(2, -10, 2));
	bear = new GameObject(this, Vec3(-3, 15, 2));
	lilbear = new GameObject(this, Vec3(-8, 5, 2));
	InsideCube = new GameObject(this, Vec3(12, 5.1f, 0));
	OutsideCube = new GameObject(this, InsideCube->position);
	//animationGuy = new Billboard(this, Vec3(4, 4, 4));

	dragon = new GameObject(this, Vec3(-8, 3, 4));
	dragonLight = new GameObject(this, dragon->position + Vec3(-10, 3, 0));
	soBright = new GameObject(this, dragon->position + Vec3(0, 3, 7.25f));
	cube = new GameObject(this, soBright->position + Vec3(0, 0, 0.25f));

	new Rigidbody(bear);
	bear->getComponent<Rigidbody>()->mass = 88;
	new Rigidbody(lilbear);

	FontManager::getInstance()->GenerateFont("font", 72, "Font/DroidSans.ttf");
	FontManager::getInstance()->GenerateFont("lazyfont", 50, "Font/ostrich-black.ttf");

	ModelManager::getInstance()->loadTexture("doge", "Images/doge.png");
	GuiImage *gi = new GuiImage(this, "doge", Vec2(-150, -150), ScreenAnchor::TOP_RIGHT);
	gi->Scale(Vec3::One() * 0.6f);

	Label *label2 = new Label(this, "So Font", FontManager::getInstance()->GetFont("font"), Vec2(0, -50.0f), ScreenAnchor::TOP_CENTER, Colour::Orange());
	label = new Label(this, "WOW!", FontManager::getInstance()->GetFont("lazyfont"), Vec2(-100, 60), ScreenAnchor::BOTTOM_RIGHT, Colour::Pink());
	fpsLabel = new Label(this, "MUCH LABELS!", FontManager::getInstance()->GetFont("lazyfont"), Vec2(160, 120), ScreenAnchor::BOTTOM_LEFT, Colour::Lime());
	fpsLabel->Rotate(Quat(45 * M_PI / 180.0f, Vec3::BasisZ()));

	cube->Scale(Vec3::One() * 0.25f);
	soBright->addChild(cube);

	light->LookAt(Vec3(0, 0, 0));

	soBright->Rotate(Quat(3.141592654f, Vec3::BasisY()));

	ground->LookAt(ground->position + Vec3(0, 1, 0));
	//flashLight->LookAt(Vec3(0, 1 , 0));

	dragon->addChild(soBright);
	lightAnchor->addChild(light);
	currentCamera->addChild(flashLight);
	InsideCube->addChild(OutsideCube);

	flashLight->Rotate(Quat(20 * 3.14159f / 180.0f, Vec3::BasisX()));
	bear->Scale(Vec3::One() * 0.2f);
	bear->Rotate(Quat(3.14159f, Vec3::BasisY()));
	lilbear->Scale(Vec3::One() * 0.1f);
	lilbear->Rotate(Quat(150 * 3.14159f / 180.0f, Vec3::BasisY()));

	dragonLight->Scale(Vec3::One() * 0.1f);

	/* CREATE MODELS */
	ModelManager::getInstance()->loadModel("teddy", "Models/teddy.obj", false, ModelManager::Draw_Mode::CW);
	ModelManager::getInstance()->loadModel("bear", "Models/bear-obj.obj", true, ModelManager::Draw_Mode::CW);
	ModelManager::getInstance()->loadModel("torch", "Models/torch.obj", true);
	ModelManager::getInstance()->loadModel("sphere", "Models/sphere.obj", false, ModelManager::Draw_Mode::CW);
	ModelManager::getInstance()->loadModel("turboSphere", "Models/turboSphere.obj", false, ModelManager::Draw_Mode::CW);

	ModelManager::getInstance()->CreateCuboid("cube", 0.5f, 0.5f, 0.5f, true);
	ModelManager::getInstance()->CreateCuboid("InCubeO", 5, 5, 5, true);
	ModelManager::getInstance()->CreateSkybox("InCubeI", 5, false);
	ModelManager::getInstance()->CreatePlane("plane", .5f, .5f);
	ModelManager::getInstance()->CreateCuboid("light", 0.25f, 0.25f, 0.25f);
	ModelManager::getInstance()->CreatePlane("ground", 100, 100, 32, 32);
	ModelManager::getInstance()->CreatePyramid("directional", 2.5f, 2.5f, 2.5f);
	ModelManager::getInstance()->CreateSkybox("skybox", 200);


	/* CREATE TEXTURES */
	const int texSize = 13 * 13 * 4;
	float pixelDataWhite[texSize];
	for (int i = 0; i < texSize; i++)
	{
		pixelDataWhite[i] = 1.0f;
	}
	Models->createTexture("white", pixelDataWhite, 1, 1);

	float pixelData[]
	{
		0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
	};
	ModelManager::getInstance()->createTexture("greenCheckers", pixelData, 2, 2);

	float pixelDataRed[]
	{
		1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
	ModelManager::getInstance()->createTexture("redCheckers", pixelDataRed, 2, 2);

	//float pixelDataWhite[]
	//{
	//	1.0f, 1.0f, 0.0f, 1.0f
	//};
	ModelManager::getInstance()->createTexture("white", pixelDataWhite, 1, 1);

	ModelManager::getInstance()->loadTexture("star", "Images/star.png");
	ModelManager::getInstance()->loadTexture("smoke", "Images/Smoke.png");
	ModelManager::getInstance()->loadTexture("grass", "Images/grass.png");
	ModelManager::getInstance()->loadTexture("skybox", "Images/Day_Skybox.png");
	ModelManager::getInstance()->loadTexture("bear", "Images/bear.tif");
	ModelManager::getInstance()->loadTexture("bearTeeth", "Images/bear_teeth_transparent.tif");
	ModelManager::getInstance()->loadTexture("dice", "Images/dice_texture.png");
	ModelManager::getInstance()->loadTexture("torchTexture", "Images/torch_DIF.tif");

	/* Add Components */
	new RenderableComponent("cube", "dice", cube);
	new RenderableComponent("light", "star", soBright);
	new RenderableComponent("ground", "grass", ground, new Material(1, 1, 1));
	new RenderableComponent("sphere", "greenCheckers", light);

	RenderableComponent *skyRC = new RenderableComponent("skybox", "skybox", skybox);
	skyRC->SetEffecctedByLight(1, 0, 0);

	new RenderableComponent("turboSphere", "white", dragon, new Material(1, 1, 1));
	new RenderableComponent("sphere", "greenCheckers", dragonLight, new Material(1, 1, 1));

	new RenderableComponent("InCubeO", "dice", OutsideCube);
	RenderableComponent *inRC = new RenderableComponent("InCubeI", "dice", InsideCube);
	inRC->SetEffecctedByLight(false, true, true);

	RenderableComponent *bearRC = new RenderableComponent("bear", "bear", bear, new Material(1, 1, 0.3f));
	bearRC->textureName.push_back(ModelManager::getInstance()->GetTextureID("bearTeeth"));
	bearRC->textureName.push_back(ModelManager::getInstance()->GetTextureID("bearTeeth"));

	RenderableComponent *lilbearRC = new RenderableComponent("bear", "bear", lilbear, new Material(1, 1, 0.3f));
	lilbearRC->textureName.push_back(ModelManager::getInstance()->GetTextureID("bearTeeth"));
	lilbearRC->textureName.push_back(ModelManager::getInstance()->GetTextureID("bearTeeth"));

	new ParticleSystem(lilCube, ParticleSystem::Emitter_Type_Sphere, "plane", "smoke", 10, 1, 5, Particle::Billboard);


	//new Light(light, Colour(1.0f, 1.0f, 1.0f), Light::Directional);
	new Light(soBright, Colour(80, 80, 80), Light::Spot, 180 * 3.14159f / 180.0f);
	//new Light(dragonLight, Colour(20, 10, 10), Light::Point);

	//flashlight
	//new Light(flashLight, Colour(200, 200, 200), Light::Spot, 60 * 3.14159f / 180.0f);

	/*Flipbook * fb = new Flipbook(animationGuy, 16, "Images/Animation/Fire/slice0.png", 1.5f, true, Flipbook::PNG);
	fb->Play();*/

	torch = new Torch(this, InsideCube->position + Vec3(0, -10, 2.5));  //Vec3(10, -5, -8));
	torch->Rotate(Quat(180 * 3.14159f / 180.0f, Vec3::BasisY()));


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

	//new AmbientBrightnessUp(&ambientLightColor, Game::GetInstance()->inputManager->mouseButtonDict[SDL_BUTTON_LEFT]);
	//new AmbientBrightnessDown(&ambientLightColor, Game::GetInstance()->inputManager->mouseButtonDict[SDL_BUTTON_RIGHT]);

	/*new FPS_MOVE_UP(ground, SDLK_y);
	new FPS_MOVE_DOWN(ground, SDLK_h);*/

	/* FPS CONTROLS */
	SDL_SetRelativeMouseMode(SDL_TRUE); //Traps Mouse in Window (centre)
	/*new FPS_FORWARD(headbob, SDLK_w);
	new FPS_BACKWARD(headbob, SDLK_s);
	new FPS_STRAFE_LEFT(headbob, SDLK_a);
	new FPS_STRAFE_RIGHT(headbob, SDLK_d);

	new FPS_MOVE_UP(headbob, SDLK_SPACE);
	new FPS_MOVE_DOWN(headbob, SDLK_x);

	new FPS_TURN_LEFT(headbob, MouseMovement::Negative_X);
	new FPS_TURN_RIGHT(headbob, MouseMovement::Positive_X);
	new FPS_TURN_UP(headbob, MouseMovement::Positive_Y);
	new FPS_TURN_DOWN(headbob, MouseMovement::Negative_Y);*/
}

TestLevel::~TestLevel()
{
}

void TestLevel::LevelUpdate(float _timeStep)
{
	Level::LevelUpdate(_timeStep);

	FPS_Controls::Update(headbob, _timeStep);

	dragon->GetTransform().Rotate(Quat(0.6f * _timeStep, Vec3(1, 0, 0)));
	lightAnchor->GetTransform().Rotate(Quat(-1.2f * _timeStep, Vec3(1, 0, 0)));
	light->LookAt(Vec3());

	//LOL
	label->Rotate(Quat(90 * M_PI / 180.0f * _timeStep, Vec3::BasisZ()));
	
	std::stringstream ss;
	ss << "FPS: " << Game::GetInstance()->GetFPS();
	fpsLabel->SetText(ss.str());
	
	skybox->position = currentCamera->position;

	//Orbit small bear around large bear
	PhysicsWorld::getInstance()->Orbit(bear->position, Vec3::BasisY(), lilbear, M_PI / 100);
	
}

void TestLevel::DebugRender()
{

}

