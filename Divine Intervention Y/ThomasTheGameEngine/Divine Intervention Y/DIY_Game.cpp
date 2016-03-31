//Author:	Mathieu Violette
//Date:		10/22/2015

#include "DIY_Game.h"
#include <sstream>
#include <OpenGLUtilities.h>
#include <Camera.h>
#include <GameObject.h>
#include <InputHandler.h>
#include <GameProperties.h>
#include <AudioManager.h>

#include <Component.h>
#include <map>
#include <vector>
#include <../Math/four_dimensions.hpp>
#include <SDL.h>

#include "SplashScreen.h"

DIY_Game::DIY_Game()
{
	GLU::OutputOpenGLVersion();

	instance = this;
	inputManager = InputController::getInstance();

	LoadLevel(new SplashScreen());
}

DIY_Game::~DIY_Game()
{

}

void DIY_Game::Update(float _timestep)
{
	if (Input->isKeyDown(SDLK_LALT) && Input->isKeyDown(SDLK_F4))
		GAME->setRunning(false);

	//FPS
	{
		std::stringstream ss;
		ss << "Divine Intervention Y | FPS: " << FPS;
		std::string _fps = ss.str();

		SDL_SetWindowTitle(gameWindow, _fps.c_str());
	}
}

void DIY_Game::Render()
{

}