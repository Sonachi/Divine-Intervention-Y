#pragma once
#include <Level.h>
#include "Layer.h"
#include <Rigidbody.h>
#include "Pointer.h"
#include "DIY_Level_GUI.h"
#include "Sun.h"


#define Global_Scale 2.0f

enum DIY_Level_State
{
	PLAYING, PAUSED, VICTORY
};

enum DIY_Level_Playing_State
{
	SHOOTING, WATCHING
};

enum DIY_Level_Victory_State
{
	REVIEW, LEVEL_SELECT
};

class DIY_Level :
	public Level
{
	friend class Layer;
public:
	DIY_Level(std::string fileName_);
	~DIY_Level();

	void LoadContent() override;

	int par;
	int strokeCount;
	////bool isPaused;

	DIY_Level_State levelState;
	DIY_Level_Playing_State playingState;
	DIY_Level_Victory_State victoryState;

	std::vector<Layer *> layers;

	DIY_Level_GUI * gui;

	bool PlayerHasShotBallIntoSun;
	bool rotateLevel;

	void LevelUpdate(float timeStep_) override;

	void SetLayerPlane(Layer *);
	Layer * GetLayerPlane();

	void AdjustObjectiveCount(int);

	std::string GetLevelFileName();

	Vec2 GetLevelBounds();
private:
	GameObject * skybox, *playerBall, *layerRB;
	Sun * sun;
	float sunRadius;
	Rigidbody * planeRigidBody;

	std::string fileName;

	int objectiveCount;

	bool isShooting;
	bool isPausedKeyStillPressed;

	void PauseLogic();

	bool hasFinishedLoading;

	void LoadLevel();

	float levelBoundsX;
	const float levelBoundsY = 5.f;
	const float DISTANCE_BETWEEN_LAYERS = 20.0f;
};

