//Author:	Mathieu Violette, Nathan Senter
//Date:		2/17/2016, 2/26/2016 (NS)

#pragma once

#include <Label.h>
#include <GuiImage.h>
#include <TextButton.h>
#include "LevelSelectButton.h" //SelectButton

class LandingScreen;

enum Menu_State{
	Title,
	Tranisiton,
	Select
};

class Landing_GUI
{
private:
	bool isInstructionsShown, isInputIsClear;

	Menu_State state;

public:
	Landing_GUI(Level *level_);
	~Landing_GUI();

	SelectButton *PlayButton;
	TextButton *ExitButton;
	TextButton *InstructionsButton;
	GuiImage *InstructionsImage;

	GuiImage * gameTitle;
	Label * pressStart;
	MultiLineLabel * detailsLabel;

	bool titleIsShown;

	LandingScreen * landingScreen;

	Menu_State GetState();
	void SetState(Menu_State);

	void Update(float timeStep_);
	void SetLevelName(std::string name_);
};