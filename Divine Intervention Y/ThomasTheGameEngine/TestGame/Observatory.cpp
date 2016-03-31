#include "Observatory.h"
#include <ModelManager.h>
#include <AudioManager.h>
#include <FontManager.h>
#include <Label.h>
#include "FPS_Controls.h"

Observatory::Observatory()
{
}

void Observatory::LoadContent()
{
	currentCamera->position = Vec3(0, 0, -5);

	SetAmbientColor(Colour(0.2, 0.2, 0.2));

	ModelManager::getInstance()->CreatePlane("plane", 1, 1);
	ModelManager::getInstance()->CreatePlane("surface", 1, 1, 3, 1);
	ModelManager::getInstance()->CreateSkybox("skybox", 10);
	ModelManager::getInstance()->CreateCuboid("doorModel", 0.25f, 0.5f, 0.05f);
	ModelManager::getInstance()->loadModel("meteor1", "Models/meteor_01.obj", true);
	ModelManager::getInstance()->loadModel("meteor2", "Models/meteor_02.obj", true);
	ModelManager::getInstance()->loadModel("meteor3", "Models/meteor_03.obj", true);
	ModelManager::getInstance()->loadModel("planet", "Models/planet.obj", true);
	ModelManager::getInstance()->loadModel("planet2", "Models/planet.obj", true);
	ModelManager::getInstance()->loadModel("torch", "Models/torch.obj", true);

	ModelManager::getInstance()->loadTexture("floor", "Images/dungeonFloor.tif");
	ModelManager::getInstance()->loadTexture("wall", "Images/dungeonWall.tif");
	ModelManager::getInstance()->loadTexture("space", "Images/spaceSkybox.tif");
	ModelManager::getInstance()->loadTexture("door", "Images/door.png");
	ModelManager::getInstance()->loadTexture("footprint", "Images/footprint.png");
	ModelManager::getInstance()->loadTexture("layerGrid", "Images/grid.png");
	ModelManager::getInstance()->loadTexture("meteorTex1", "Images/meteor_texture.tif");
	ModelManager::getInstance()->loadTexture("meteorTex2", "Images/meteor_texture_2.tif");
	ModelManager::getInstance()->loadTexture("meteorTex3", "Images/meteor_texture_3.tif");
	ModelManager::getInstance()->loadTexture("planetTex", "Images/hoth.tif");
	ModelManager::getInstance()->loadTexture("planetTex1", "Images/PlanetTex.png");
	ModelManager::getInstance()->loadTexture("sunTex", "Images/sun.tif");
	ModelManager::getInstance()->loadTexture("aruba", "Images/aruba.tif");
	ModelManager::getInstance()->loadTexture("Rim", "Images/planet_Rim.tif");
	ModelManager::getInstance()->loadTexture("minerMoon", "Images/minersMoon.tif");
	ModelManager::getInstance()->loadTexture("8ball", "Images/Ball8.tif");
	ModelManager::getInstance()->loadTexture("torchTexture", "Images/torch_DIF.tif");
	ModelManager::getInstance()->loadTexture("smoke", "Images/Smoke.png");

	AudioManager::getInstance()->loadMusic("observatory", "Sounds/observatory-loop.wav");
	AudioManager::getInstance()->setMusicVolume(0.0f);


	FontManager::getInstance()->GenerateFont("font", 72, "Font/DroidSans.ttf");

	Label *shadow = new Label(this, "Project: Thomas Engine Demo", FontManager::getInstance()->GetFont("font"), Vec2(-200, 37), ScreenAnchor::BOTTOM_RIGHT, Colour::Black());
	shadow->Scale(Vec3::One() * 0.4f);

	Label *label = new Label(this, "Project: Thomas Engine Demo", FontManager::getInstance()->GetFont("font"), Vec2(-202, 35), ScreenAnchor::BOTTOM_RIGHT, Colour::Yellow());
	label->Scale(Vec3::One() * 0.4f);

	player = new Player(this, currentCamera);
	hallway = new Hallway(this, Vec3::Zero());
	obr = new ObservatoryRoom(this, Vec3(0, 0, 15), player);
	door = new SlidingDoor(this, Vec3(0, 0, 4.9f), player);

	FPS_Controls::FPSMoveSpeed = 1.5f;
}

Observatory::~Observatory()
{
}

void Observatory::LevelUpdate(float timeStep_)
{
	Level::LevelUpdate(timeStep_);

	FPS_Controls::Update(player, timeStep_);
}