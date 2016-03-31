//Author:	Mathieu Violette
//Date:		2/11/2016

#pragma once

#include <Label.h>
#include <GuiImage.h>
#include <TextButton.h>
#include "LevelSelectButton.h"

class DIY_Level_VictoryGUI
{
public:
	DIY_Level_VictoryGUI(Level *level_, int par_);
	~DIY_Level_VictoryGUI();

	GuiImage *Background;

	//Review
	Label *victoryLabel, *strokeCountLabel, *strokeLabel, *parCountLabel, *parLabel, *slashLabel;
	TextButton *retryButton, *levelSelectButton, *exitButton;

	//Level_Select
	Label *levelSelectLabel;
	TextButton *backButton, *loadLevelButton;

	//xml filepath | LevelSelectButton
	std::map<std::string, LevelSelectButton*> levelButtons;
	
	std::string selectedLevel;

	void HideAll();
	
	void HideReview();
	void ShowReview();

	void HideLevelSelect();
	void ShowLevelSelect();
};

class DIY_Level_PauseGUI
{
public:
	DIY_Level_PauseGUI(Level *level_);
	~DIY_Level_PauseGUI();

	GuiImage *Background;
	TextButton *ExitButton, *ResumeButton;

	void Hide();
	void Show();
};

class DIY_Level_GameGUI
{
public:
	DIY_Level_GameGUI(Level *level_, int par_, int objectives_);
	~DIY_Level_GameGUI();

	Label *ParLabel, *StrokeLabel;// , *ObjectivesLabel;
	Label *ParCountLabel, *StrokeCountLabel;// , *ObjectivesCountLabel;
	GuiImage *ShotPowerMeterFrame, *ShotPowerMeter, *ParBackground, *StrokeBackground;// , *ObjectivesBackground;

	void Hide();
	void Show();
};

class DIY_Level_GUI
{
private:
	int par, objectives, strokes;
	DIY_Level_GameGUI *gameGUI;
	DIY_Level_PauseGUI *pauseGUI;
	DIY_Level_VictoryGUI *victoryGUI;
public:
	DIY_Level_GUI(Level *level_, int par_, int objectives_);
	~DIY_Level_GUI();

	float shotPower;	

	void Update(float timeStep_);

	//Used to update the number of remaining objectives
	void SetObjectivesRemaining(int objectives_);

	//Used to set par
	void SetLevelPar(int par_);

	//Used to update the stroke count
	void PlayerTookAStroke(int strokeIncrement = 1);	
};