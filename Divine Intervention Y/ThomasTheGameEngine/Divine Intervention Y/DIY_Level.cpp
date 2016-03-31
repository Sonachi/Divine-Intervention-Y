#include "DIY_Level.h"
#include <dependencies\TinyXML\tinyxml2.h>

#include <ModelManager.h>
#include <AudioManager.h>
#include <RenderableComponent.h>
#include <InputHandler.h>
#include <Game.h>

#include "FocusCamera.h"
#include "GameCamera.h"
#include "Planet.h"
#include "Obstacles.h"
#include "PlayerBall.h"

#include <PhysicsWorld.h>
#include "LandingScreen.h"

DIY_Level::DIY_Level(std::string fileName_) : fileName(fileName_), isPausedKeyStillPressed(false), hasFinishedLoading(false), PlayerHasShotBallIntoSun(false)
{
	levelState = DIY_Level_State::PLAYING;
	playingState = DIY_Level_Playing_State::SHOOTING;
	victoryState = DIY_Level_Victory_State::REVIEW;

	ambientLightColor = Colour(0.1f, 0.1f, 0.1f);
}

void DIY_Level::LoadContent()
{
	tinyxml2::XMLDocument doc;

	//Tries to load the file, quits if it fails
	if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		GAME->LoadLevel(new LandingScreen);
		std::cout << "ERROR LOADING: " << fileName << "\nLOADING LANDING SCREEN...\n";
		return;
	}

	//Grabs first element
	tinyxml2::XMLElement * element = doc.RootElement();

	//set par
	par = element->IntAttribute("par");

	//Delete old main camera and set currentCamera to it.
	mainCamera->isFlagged = true;
	mainCamera = new GameCamera(this, Vec3(0, 0.75f, -7), Vec3::Zero());
	currentCamera = mainCamera;

	//ADD GUI  **MUST BE BEFORE OTHER TEXTURES OR IT WON'T SHOW UP** <- for some reason...
	gui = new DIY_Level_GUI(this, par, 1);

	Models->CreateSkybox("skybox", 10000.0f);
	Models->loadTexture("skybox1", "Images/skyboxUP.png");
	Models->loadTexture("layerGrid", "Images/grid.png");

	Models->loadTexture("planet1", "Images/aruba.tif");
	Models->loadTexture("planet2", "Images/planet_Rim.tif");
	Models->loadTexture("planet3", "Images/minersMoon.tif");
	Models->loadTexture("planet4", "Images/planetTex.png");
	Models->loadTexture("planet5", "Images/hoth.tif");

	Models->loadTexture("ballSkin", "Images/8ball.png");
	Models->loadModel("sphere", "Models/planet.obj", true);
	Models->loadModel("sun", "Models/turboSphere.obj", true);

	Models->loadModel("warpGate", "Models/WarpGate.obj", true);
	Models->loadTexture("gateTexture", "Images/rosary.png");

	Models->loadTexture("meteorTex1", "Images/meteor_texture.tif");
	Models->loadTexture("meteorTex2", "Images/meteor_texture_2.tif");
	Models->loadTexture("meteorTex3", "Images/meteor_texture_3.tif");

	Models->loadTexture("sunTexture", "Images/sun.tif");

	Models->loadModel("meteor1", "Models/meteor_01.obj", true);
	Models->loadModel("meteor2", "Models/meteor_02.obj", true);
	Models->loadModel("meteor3", "Models/meteor_03.obj", true);

	Models->loadModel("satellite", "Models/Satellite.obj");

	//Load up and play the music for in-game
	Audio->loadMusic("gameTheme", "Sounds/Exotics.wav");
	Audio->getMusic("gameTheme")->Play();

	Models->loadModel("wormhole", "Models/wormhole.obj", true);
	Models->loadTexture("wormholeTexture", "Images/Galaxy.png");

	Models->loadModel("ufo", "Models/UFO.obj", true);

	//Gotta be big to show up..
	//don't know why 
	const int texSize = 14 * 14 * 4;
	float pixelDataWhite[texSize];
	for (int i = 0; i < texSize; i++)
	{
		pixelDataWhite[i] = 1.0f;
	}
	Models->createTexture("white", pixelDataWhite, 1, 1);
	//Models->loadModel("pointer", "Models/Hand_Pointer.obj", true);
	Models->loadModel("pointer", "Models/pointer.obj", true);


	//Setting level boundaries. CAN BE EDITTED
	levelBoundsX = 10;

	LoadLevel();

	//Change camera to new focus camera
	mainCamera->isFlagged = true;
	mainCamera = new FocusCamera(this, playerBall, playerBall->position + Vec3(0, 0, -7));
	currentCamera = mainCamera;

	//Can now go through Update loop.
	hasFinishedLoading = true;
}

DIY_Level::~DIY_Level()
{
	delete gui;
}

void DIY_Level::LoadLevel()
{
	tinyxml2::XMLDocument doc;

	//Tries to load the file, quits if it fails
	if (doc.LoadFile(fileName.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		GAME->LoadLevel(new LandingScreen);
		std::cout << "ERROR LOADING: " << fileName << "\nLOADING LANDING SCREEN...\n";

		return;
	}

	//Grabs first element
	tinyxml2::XMLElement * element = doc.RootElement();

	//set par
	par = element->IntAttribute("par");
	strokeCount = 0;

	int numLayers = 0;

	rotateLevel = false;
	isShooting = false;

	//Grab the number of layers from the XML
	numLayers = element->FloatAttribute("numLayers");

	//set up skybox
	{
		skybox = new GameObject(this, currentCamera->position);
		Generic_RenderableComponent * r = new Generic_RenderableComponent(skybox, "skybox", std::string(element->Attribute("skybox")));
		r->SetEffecctedByLight(false, false, false);
	}

	sunRadius = 100;
	sun = new Sun(this, Vec3(), sunRadius);

	element = element->FirstChildElement("Layer");
	for (int i = 0; i < numLayers; i++)
	{
		layers.push_back(new Layer(this, element, i));

		if (i + 1 == numLayers)
		{
			planeRigidBody = new Rigidbody(*(layers.end() - 1), new PlaneCollider(*(layers.end() - 1), Vec3(0, 1, 0)));
			planeRigidBody->isKinematic = false;

			SetLayerPlane(*(layers.end() - 1));

			float x = element->FloatAttribute("playerX");
			float z = element->FloatAttribute("playerZ");

			playerBall = new PlayerBall(this, Vec3(layers.back()->position.x + x, 2.5f, layers.back()->position.z + z));
		}

		element = element->NextSiblingElement();
	} 
	
}

void DIY_Level::LevelUpdate(float timeStep_)
{
	if (!hasFinishedLoading)
		return;

	Level::LevelUpdate(timeStep_);

	if (InputController::getInstance()->isKeyDown(SDLK_EQUALS))
	{
		sun->scale += Vec3::One() * 1.0f;
		std::cout << sun->scale.toString() << "\n";
	}
	else if (InputController::getInstance()->isKeyDown(SDLK_MINUS))
	{
		sun->scale -= Vec3::One() * 1.0f;
		std::cout << sun->scale.toString() << "\n";
	}	

	//Victory TEST
	if (InputController::getInstance()->isKeyDown(SDLK_v))
	{
		levelState = VICTORY;
	}

	//Game state switch
	switch (levelState)
	{
	case DIY_Level_State::PLAYING:
		gui->shotPower = ((PlayerBall*)playerBall)->GetChargePercent() / 100.0f;

		PauseLogic();

		//Check for Victory
		if (PlayerHasShotBallIntoSun)
		{
			levelState = DIY_Level_State::VICTORY;
		}
		else
		{
			switch (playingState)
			{
			case DIY_Level_Playing_State::SHOOTING:
				if (!isShooting && ((PlayerBall*)playerBall)->GetIsChargingStrike())
					isShooting = true;
				else if (isShooting && !((PlayerBall*)playerBall)->GetIsChargingStrike())
				{
					isShooting = false;
					strokeCount++;
					gui->PlayerTookAStroke();

					playingState = DIY_Level_Playing_State::WATCHING;
				}
				break;
			case DIY_Level_Playing_State::WATCHING:
				if (!playerBall->getComponent<Rigidbody>()->isAwake())
					playingState = DIY_Level_Playing_State::SHOOTING;
				break;
			}

			//if (Input->isMouseDown(SDL_BUTTON_RIGHT))
			//{
			//	//layerContainer->Rotate(Quat(Input->deltaMouse().x * timeStep_, Vec3(0, 1.0f, 0)));
			//	PhysicsWorld::Orbit(Vec3::Zero(), Vec3::BasisY(), mainCamera, Input->deltaMouse().x * timeStep_);
			//	mainCamera->Rotate(Quat(Input->deltaMouse().x * timeStep_, Vec3::BasisY()));
			//}
			//break;
		}
	case DIY_Level_State::PAUSED:
		PauseLogic();
		break;
	case DIY_Level_State::VICTORY:
		switch (victoryState)
		{
		case DIY_Level_Victory_State::REVIEW:
			break;
		case DIY_Level_Victory_State::LEVEL_SELECT:
			break;
		}
		break;
	}

	gui->Update(timeStep_);
}

void DIY_Level::SetLayerPlane(Layer * layer_)
{
	//Render the current layer invisible
	if (planeRigidBody->parentObject)
		((Layer *)planeRigidBody->parentObject)->SetEnabled(false);

	//Assign the rigid body to the new layer
	planeRigidBody->parentObject = layer_;
	planeRigidBody->col->parentObject = layer_;
	static_cast<PlaneCollider *>(planeRigidBody->col)->plane = Plane(Vec3(0, 1, 0), planeRigidBody->parentObject->position);

	//Render the new layer visible
	((Layer *)planeRigidBody->parentObject)->SetEnabled(true);
}

Layer * DIY_Level::GetLayerPlane()
{
	return (Layer *)planeRigidBody->parentObject;
}

void DIY_Level::PauseLogic()
{
	if (Input->isKeyDown(SDLK_ESCAPE) && !isPausedKeyStillPressed)
	{
		levelState = levelState == DIY_Level_State::PAUSED ? DIY_Level_State::PLAYING : DIY_Level_State::PAUSED;
		isPausedKeyStillPressed = true;
	}
	else if (!Input->isKeyDown(SDLK_ESCAPE) && isPausedKeyStillPressed)
	{
		isPausedKeyStillPressed = false;
	}

	PhysicsWorld::getInstance()->isPhysicsRunning = levelState == DIY_Level_State::PAUSED ? false : levelState == DIY_Level_State::VICTORY ? false : true;
}

void DIY_Level::AdjustObjectiveCount(int countChange_)
{
	objectiveCount += countChange_;
	gui->SetObjectivesRemaining(objectiveCount);
}

std::string  DIY_Level::GetLevelFileName()
{
	return fileName;
}

Vec2 DIY_Level::GetLevelBounds()
{
	return Vec2(levelBoundsX, levelBoundsY);
}