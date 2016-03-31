#pragma once
#include <Level.h>
#include "Layerr.h"

class GameLevel :
	public Level
{
public:
	GameLevel(std::string fileName_);
	~GameLevel();

	int par;
	int strokeCount;
	int levelBounds;

	std::vector<Layer *> layers;

	bool HasObjectives();

	GameObject * layerContainer;

	bool rotateLevel;

	void LoadContent() override;
	void LevelUpdate(float timeStep_) override;
private:
	GameObject * skybox;
	std::string fileName;
};

