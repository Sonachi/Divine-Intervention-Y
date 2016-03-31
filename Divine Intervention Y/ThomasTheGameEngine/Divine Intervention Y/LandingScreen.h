//Author:	Mathieu Violette, Nathan Senter
//Date:		2/17/2016, 2/16/2016(NS)

#include <Level.h>
#include "Landing_GUI.h"
#include "GalaxyMap.h"

class LandingScreen : public Level
{
private:
	Landing_GUI *gui;

	std::string targetFileName;
public:
	LandingScreen();
	~LandingScreen();

	void LoadContent() override;

	void LevelUpdate(float timeStep_) override;

	std::string GetTargetFileName();

	GameObject * skybox;
	GalaxyMap * galaxyMap;
};