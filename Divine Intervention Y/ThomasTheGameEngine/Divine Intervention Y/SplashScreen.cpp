#include "SplashScreen.h"
#include <GuiImage.h>
#include <ModelManager.h>
#include <InputHandler.h>

#include <Game.h>
#include "LandingScreen.h"

SplashScreen::SplashScreen() : timeTilSwitchLevel(3.0f)
{
}

void SplashScreen::LoadContent()
{
	Models->loadTexture("SPLASH_SCREEN_IMAGE", "Images/splashscreen.png");

	new GuiImage(this, "SPLASH_SCREEN_IMAGE", Vec2(), ScreenAnchor::CENTER);
}

SplashScreen::~SplashScreen()
{

}

void SplashScreen::LevelUpdate(float timeStep_)
{
	Level::LevelUpdate(timeStep_);

	timeTilSwitchLevel -= timeStep_;

	if (timeTilSwitchLevel <= 0 || Input->isAnyKeyPressed())
	{
		GAME->LoadLevel(new LandingScreen());
	}
}