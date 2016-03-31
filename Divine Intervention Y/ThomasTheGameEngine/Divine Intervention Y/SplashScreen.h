//Author:	Mathieu Violette
//Date:		2/12/2016

#include <Level.h>

class SplashScreen : public Level
{
private:
	float timeTilSwitchLevel;
public:
	SplashScreen();
	~SplashScreen();

	void LoadContent() override;

	void LevelUpdate(float timeStep_) override;
};