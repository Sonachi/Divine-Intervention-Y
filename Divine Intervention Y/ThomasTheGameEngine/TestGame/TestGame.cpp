//Author:	Mathieu Violette
//Date:		10/22/2015

#include "TestGame.h"
#include "TestLevel.h"
#include <GameObject.h>
#include <GameProperties.h>
#include <AudioManager.h>
#include <Camera.h>
#include <sstream>
#include "Observatory.h"
#include "DrScottLevel.h"
#include "TestNetLevel.h"
#include "UITestLevel.h"
//#include "GameLevel.h"
#include <InputHandler.h>

TestGame::TestGame()
{
	instance = this;


	//LoadLevel(new GameLevel("testLevel.xml"));

	LoadLevel(new Observatory());
	//LoadLevel(new TestLevel());
	//LoadLevel(new DrScottLevel());
	//LoadLevel(new TestNetLevel());
	//LoadLevel(new UITestLevel());

	GLU::OutputOpenGLVersion();
}

TestGame::~TestGame()
{

}

void TestGame::Update(float _timestep)
{
	if (InputController::getInstance()->isKeyDown(SDLK_ESCAPE))
	{
		Game::GetInstance()->setRunning(false);
		printf("\nExit\n");
	}

	//FPS
	{
		std::stringstream ss;
		ss << "Thomas Demo | FPS: " << FPS;
		string _fps = ss.str();

		SDL_SetWindowTitle(gameWindow, _fps.c_str());
	}
}

void TestGame::Render()
{

}