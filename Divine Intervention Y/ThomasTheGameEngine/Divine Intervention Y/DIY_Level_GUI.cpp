#include "DIY_Level_GUI.h"
#include <ModelManager.h>
#include <Game.h>
#include "DIY_Level.h"
#include "LandingScreen.h"

std::string IntToString(int i_);
std::string IntToString(int i_, int digits_);

DIY_Level *level;

/***********************************************************/
/*						LEVEL GUI						   */
/***********************************************************/

DIY_Level_GUI::DIY_Level_GUI(Level *level_, int par_, int objectives_)
: par(par_), objectives(objectives_), strokes(0), shotPower(0)
{
	level = (DIY_Level*)level_;

	gameGUI = new DIY_Level_GameGUI(level_, par, objectives);
	pauseGUI = new DIY_Level_PauseGUI(level_);
	victoryGUI = new DIY_Level_VictoryGUI(level_, par);

	pauseGUI->Hide();
	victoryGUI->HideAll();
}

DIY_Level_GUI::~DIY_Level_GUI()
{
	delete victoryGUI;
	delete pauseGUI;
	delete gameGUI;
}

void DIY_Level_GUI::Update(float timeStep_)
{
	gameGUI->ShotPowerMeter->SetDrawPercent(shotPower);

	if (((DIY_Level*)GAME->currentLevel)->levelState == DIY_Level_State::PAUSED)
	{
		pauseGUI->Show();

		if (pauseGUI->ExitButton->HasBeenClicked())
		{
			GAME->LoadLevel(new LandingScreen());
		}

		if (pauseGUI->ResumeButton->HasBeenClicked())
		{
			((DIY_Level*)GAME->currentLevel)->levelState = DIY_Level_State::PLAYING;
		}
	}
	else
		pauseGUI->Hide();

	if (((DIY_Level*)GAME->currentLevel)->levelState == DIY_Level_State::VICTORY)
	{
		gameGUI->Hide();

		switch (((DIY_Level*)GAME->currentLevel)->victoryState)
		{
		case DIY_Level_Victory_State::REVIEW:
			victoryGUI->ShowReview();

			if (victoryGUI->exitButton->HasBeenClicked())
			{
				//Go back to landing screen
				GAME->LoadLevel(new LandingScreen());
			}
			else if (victoryGUI->retryButton->HasBeenClicked())
			{
				//Reload the level
				GAME->LoadLevel(new DIY_Level(level->GetLevelFileName()));
			}
			else if (victoryGUI->levelSelectButton->HasBeenClicked())
			{
				//Change VICTORY_STATE to level select
				((DIY_Level*)GAME->currentLevel)->victoryState = DIY_Level_Victory_State::LEVEL_SELECT;
			}
			break;
		case DIY_Level_Victory_State::LEVEL_SELECT:
			victoryGUI->ShowLevelSelect();

			if (victoryGUI->selectedLevel != "")
				victoryGUI->loadLevelButton->SetTextColour(&Colour::Yellow(), &Colour::Lime());
			else
				//Gray-out text when no level is selected
				victoryGUI->loadLevelButton->SetTextColour(&Colour::Colour(0.3f, 0.3f, 0.3f), &Colour::Colour(0.3f, 0.3f, 0.3f));

			if (victoryGUI->backButton->HasBeenClicked())
			{
				//Change VICTORY_STATE to Review
				((DIY_Level*)GAME->currentLevel)->victoryState = DIY_Level_Victory_State::REVIEW;
				victoryGUI->selectedLevel = "";
			}
			else if (victoryGUI->selectedLevel != "" && victoryGUI->loadLevelButton->HasBeenClicked())
			{
				GAME->LoadLevel(new DIY_Level(victoryGUI->selectedLevel));
			}
			else
			{
				for (auto it = victoryGUI->levelButtons.begin(); it != victoryGUI->levelButtons.end(); it++)
				{
					if (it->second->HasBeenClicked())
					{
						victoryGUI->selectedLevel = it->first;
						it->second->isSelected = true;
					}

					if (it->first != victoryGUI->selectedLevel)
						it->second->isSelected = false;
				}
			}
			break;
		}
	}
	else
	{
		gameGUI->Show();
		victoryGUI->HideAll();
	}

}

void DIY_Level_GUI::SetObjectivesRemaining(int objectives_)
{
	objectives = objectives_;

	//gameGUI->ObjectivesCountLabel->SetText(IntToString(objectives_));
}

void DIY_Level_GUI::SetLevelPar(int par_)
{
	par = par_;
	gameGUI->ParCountLabel->SetText(IntToString(par));
}

void DIY_Level_GUI::PlayerTookAStroke(int strokeIncrement)
{
	strokes += strokeIncrement;

	gameGUI->StrokeCountLabel->SetText(IntToString(strokes));
	victoryGUI->strokeCountLabel->SetText(IntToString(strokes));
}

std::string IntToString(int i_)
{
	char buffer[10];
	std::string text;

	_itoa_s(i_, buffer, 10);

	text = buffer;

	return text;
}

std::string IntToString(int i_, int digits_)
{
	char buffer[10];
	std::string text;

	_itoa_s(i_, buffer, 10);

	text = buffer;

	while (digits_ - text.length() > 0)
	{
		text = "0" + text;
	}

	return text;
}

/***********************************************************/
/*						GAME GUI						   */
/***********************************************************/

DIY_Level_GameGUI::DIY_Level_GameGUI(Level *level_, int par_, int objectives_)
{
	int strokes = 0;

	Vec2 ParLabelLocation = Vec2(150, -50);
	Vec2 StrokeLabelLocation = ParLabelLocation + Vec2(0, -80);
	Vec2 ObjectivesLabelLocation = StrokeLabelLocation + Vec2(0, -80);

	/* GUI IMAGES */
	{
		Vec2 ShotPowerMeterLocation = Vec2(-350, 65);

		Models->loadTexture("DIY_LEVEL_GUI_SHOT_METER", "Images/Level GUI/ShotPowerMeter.png");
		ShotPowerMeter = new GuiImage(level_, "DIY_LEVEL_GUI_SHOT_METER", ShotPowerMeterLocation + Vec2(2, 0), ScreenAnchor::BOTTOM_RIGHT, true);
		//ShotPowerMeter->drawStyle = DRAW_STYLE::RAINBOW_STYLE;

		Models->loadTexture("DIY_LEVEL_GUI_SHOT_FRAME", "Images/Level GUI/ShotPowerMeterFrame.png");
		ShotPowerMeterFrame = new GuiImage(level_, "DIY_LEVEL_GUI_SHOT_FRAME", ShotPowerMeterLocation, ScreenAnchor::BOTTOM_RIGHT);

		Models->loadTexture("DIY_LEVEL_GAMEGUI_BACKGROUND", "Images/Level GUI/GUI_Background.png");
		ParBackground = new GuiImage(level_, "DIY_LEVEL_GAMEGUI_BACKGROUND", ParLabelLocation + Vec2(35, 5), ScreenAnchor::TOP_LEFT);
		ParBackground->SetOpacity(0.5f);
		StrokeBackground = new GuiImage(level_, "DIY_LEVEL_GAMEGUI_BACKGROUND", StrokeLabelLocation + Vec2(35, 5), ScreenAnchor::TOP_LEFT);
		StrokeBackground->SetOpacity(0.5f);
		//ObjectivesBackground = new GuiImage(level_, "DIY_LEVEL_GAMEGUI_BACKGROUND", ObjectivesLabelLocation + Vec2(35, 5), ScreenAnchor::TOP_LEFT);
	}

	/* LABELS */
	{
		//Fonts
		FontManager::getInstance()->GenerateFont("DIY_LEVEL_GUI_TEXT", 40, "Font/Aaargh.ttf");
		FontManager::getInstance()->GenerateFont("DIY_LEVEL_GUI_COUNT", 50, "Font/ostrich-black.ttf");

		ParLabel = new Label(level_, "Par", FontManager::getInstance()->GetFont("DIY_LEVEL_GUI_TEXT"), ParLabelLocation, ScreenAnchor::TOP_LEFT, Colour::Yellow());
		StrokeLabel = new Label(level_, "Strokes", FontManager::getInstance()->GetFont("DIY_LEVEL_GUI_TEXT"), StrokeLabelLocation, ScreenAnchor::TOP_LEFT, Colour::Yellow());
		//ObjectivesLabel = new Label(level_, "Objectives Remaining", FontManager::getInstance()->GetFont("DIY_LEVEL_GUI_TEXT"), ObjectivesLabelLocation, ScreenAnchor::TOP_LEFT, Colour::Yellow());

		ParCountLabel = new Label(level_, IntToString(par_), FontManager::getInstance()->GetFont("DIY_LEVEL_GUI_COUNT"), ParLabelLocation + Vec2(175, 0), ScreenAnchor::TOP_LEFT, Colour::Yellow());
		StrokeCountLabel = new Label(level_, IntToString(strokes), FontManager::getInstance()->GetFont("DIY_LEVEL_GUI_COUNT"), StrokeLabelLocation + Vec2(175, 0), ScreenAnchor::TOP_LEFT, Colour::Yellow());
		//ObjectivesCountLabel = new Label(level_, IntToString(objectives_), FontManager::getInstance()->GetFont("DIY_LEVEL_GUI_COUNT"), ObjectivesLabelLocation + Vec2(175, 0), ScreenAnchor::TOP_LEFT, Colour::Yellow());
	}
}

DIY_Level_GameGUI::~DIY_Level_GameGUI(){}

void DIY_Level_GameGUI::Hide()
{
	ShotPowerMeter->Hide();
	ShotPowerMeterFrame->Hide();

	ParBackground->Hide();
	StrokeBackground->Hide();
	//ObjectivesBackground->Hide();

	ParLabel->Hide();
	StrokeLabel->Hide();
	//ObjectivesLabel->Hide();

	ParCountLabel->Hide();
	StrokeCountLabel->Hide();
	//ObjectivesCountLabel->Hide();
}

void DIY_Level_GameGUI::Show()
{
	ShotPowerMeter->Show();
	ShotPowerMeterFrame->Show();

	ParBackground->Show();
	StrokeBackground->Show();
	//ObjectivesBackground->Show();

	ParLabel->Show();
	StrokeLabel->Show();
	//ObjectivesLabel->Show();

	ParCountLabel->Show();
	StrokeCountLabel->Show();
	//ObjectivesCountLabel->Show();
}

/***********************************************************/
/*						PAUSE GUI						   */
/***********************************************************/

DIY_Level_PauseGUI::DIY_Level_PauseGUI(Level *level_)
{
	Vec2 ResumeButtonLocation = Vec2(0, 200);
	Vec2 ExitButtonLocation = ResumeButtonLocation + Vec2(0, -200);

	/* BACKGROUND */
	Models->loadTexture("DIY_LEVEL_PAUSEGUI_BACKGROUND", "Images/Level GUI/PauseBackground.png");
	Background = new GuiImage(level_, "DIY_LEVEL_PAUSEGUI_BACKGROUND", Vec2(), ScreenAnchor::CENTER);
	Background->SetOpacity(0.5f);

	/* BUTTONS */
	//Fonts
	FontManager::getInstance()->GenerateFont("DIY_LEVEL_GUI_BUTTON_LARGE", 60, "Font/ostrich-black.ttf");

	//Textures
	Models->loadTexture("LEVEL_GUI_BLANK", "Images/Level GUI/Buttons/Blank.png");
	Models->loadTexture("LEVEL_GUI_BLANK_HOVERED", "Images/Level GUI/Buttons/BlankHovered.png");

	/*Models->loadTexture("DIY_LEVEL_GUI_EXIT", "Images/Level GUI/Buttons/Exit.png");
	Models->loadTexture("DIY_LEVEL_GUI_EXIT_PRESSED", "Images/Level GUI/Buttons/ExitPressed.png");
	Models->loadTexture("DIY_LEVEL_GUI_EXIT_HOVERED", "Images/Level GUI/Buttons/ExitHovered.png");*/
	ExitButton = new TextButton(level_, ExitButtonLocation, "EXIT", FontManager::getInstance()->GetFont("DIY_LEVEL_GUI_BUTTON_LARGE"), "LEVEL_GUI_BLANK", "LEVEL_GUI_BLANK_HOVERED", ScreenAnchor::CENTER, Colour::Yellow());

	/*Models->loadTexture("DIY_LEVEL_GUI_RESUME", "Images/Level GUI/Buttons/Resume.png");
	Models->loadTexture("DIY_LEVEL_GUI_RESUME_PRESSED", "Images/Level GUI/Buttons/ResumePressed.png");
	Models->loadTexture("DIY_LEVEL_GUI_RESUME_HOVERED", "Images/Level GUI/Buttons/ResumeHovered.png");*/
	ResumeButton = new TextButton(level_, ResumeButtonLocation, "RESUME", FontManager::getInstance()->GetFont("DIY_LEVEL_GUI_BUTTON_LARGE"), "LEVEL_GUI_BLANK", "LEVEL_GUI_BLANK_HOVERED", ScreenAnchor::CENTER, Colour::Yellow());
}

DIY_Level_PauseGUI::~DIY_Level_PauseGUI(){}

void DIY_Level_PauseGUI::Hide()
{
	Background->Hide();
	ExitButton->Hide();
	ResumeButton->Hide();
}

void DIY_Level_PauseGUI::Show()
{
	Background->Show();
	ExitButton->Show();
	ResumeButton->Show();
}


/***********************************************************/
/*					  VICTORY GUI						   */
/***********************************************************/

DIY_Level_VictoryGUI::DIY_Level_VictoryGUI(Level *level_, int par_)
{
	int strokes = 0;

	/* BACKGROUND */
	Models->loadTexture("DIY_LEVEL_VICTORYGUI_BACKGROUND", "Images/Level GUI/Victory_Background.png");
	Background = new GuiImage(level_, "DIY_LEVEL_VICTORYGUI_BACKGROUND", Vec2(), ScreenAnchor::CENTER);
	Background->SetOpacity(0.5f);

	//Fonts
	FontManager::getInstance()->GenerateFont("DIY_LEVEL_GUI_TEXT_LARGE", 80, "Font/ostrich-black.ttf");
	FontManager::getInstance()->GenerateFont("DIY_LEVEL_GUI_LABEL_TEXT", 40, "Font/ostrich-black.ttf");
	FontManager::getInstance()->GenerateFont("DIY_LEVEL_GUI_COUNT", 65, "Font/ostrich-black.ttf");
	FontManager::getInstance()->GenerateFont("LEVEL_GUI_BUTTON_FONT", 45, "Font/Aaargh.ttf");

	//Textures
	Models->loadTexture("LEVEL_GUI_BLANK", "Images/Level GUI/Buttons/Blank.png");
	Models->loadTexture("LEVEL_GUI_BLANK_HOVERED", "Images/Level GUI/Buttons/BlankHovered.png");

	/***************************/
	/*		    REVIEW		   */
	/***************************/

	/* LABELS */
	Vec2 victoryLabelLocation = Vec2(0, 200);
	Vec2 strokeCountLabelLocation = victoryLabelLocation + Vec2(-150, -150);
	Vec2 strokeLabelLocation = strokeCountLabelLocation + Vec2(0, -100);
	Vec2 slashLabelLocation = victoryLabelLocation + Vec2(0, -150);
	Vec2 parCountLabelLocation = victoryLabelLocation + Vec2(150, -150);
	Vec2 parLabelLocation = parCountLabelLocation + Vec2(0, -100);

	victoryLabel = new Label(level_, "VICTORY!", FontManager::getInstance()->GetFont("DIY_LEVEL_GUI_TEXT_LARGE"), victoryLabelLocation, ScreenAnchor::CENTER, Colour::Yellow());
	strokeCountLabel = new Label(level_, IntToString(strokes), FontManager::getInstance()->GetFont("DIY_LEVEL_GUI_COUNT"), strokeCountLabelLocation, ScreenAnchor::CENTER, Colour::Yellow());
	strokeLabel = new Label(level_, "Strokes", FontManager::getInstance()->GetFont("DIY_LEVEL_GUI_LABEL_TEXT"), strokeLabelLocation, ScreenAnchor::CENTER, Colour::Yellow());
	slashLabel = new Label(level_, "/", FontManager::getInstance()->GetFont("DIY_LEVEL_GUI_TEXT_LARGE"), slashLabelLocation, ScreenAnchor::CENTER, Colour::Yellow());
	parCountLabel = new Label(level_, IntToString(par_), FontManager::getInstance()->GetFont("DIY_LEVEL_GUI_COUNT"), parCountLabelLocation, ScreenAnchor::CENTER, Colour::Yellow());
	parLabel = new Label(level_, "Par", FontManager::getInstance()->GetFont("DIY_LEVEL_GUI_LABEL_TEXT"), parLabelLocation, ScreenAnchor::CENTER, Colour::Yellow());

	/* BUTTONS */
	Vec2 levelSelectButtonLocation = Vec2(300, -200);
	Vec2 exitButtonLocation = Vec2(0, -200);
	Vec2 retryButtonLocation = Vec2(-300, -200);

	retryButton = new TextButton(level_, retryButtonLocation, "Retry", FontManager::getInstance()->GetFont("LEVEL_GUI_BUTTON_FONT"), "LEVEL_GUI_BLANK", "LEVEL_GUI_BLANK_HOVERED", ScreenAnchor::CENTER, Colour::Yellow());
	levelSelectButton = new TextButton(level_, levelSelectButtonLocation, "Level Select", FontManager::getInstance()->GetFont("LEVEL_GUI_BUTTON_FONT"), "LEVEL_GUI_BLANK", "LEVEL_GUI_BLANK_HOVERED", ScreenAnchor::CENTER, Colour::Yellow());
	exitButton = new TextButton(level_, exitButtonLocation, "Exit", FontManager::getInstance()->GetFont("LEVEL_GUI_BUTTON_FONT"), "LEVEL_GUI_BLANK", "LEVEL_GUI_BLANK_HOVERED", ScreenAnchor::CENTER, Colour::Yellow());


	/***************************/
	/*		Level Select	   */
	/***************************/

	selectedLevel = "";

	/* LABELS */
	Vec2 levelSelectLabelLocation = victoryLabelLocation;
	levelSelectLabel = new Label(level_, "Select a Level", FontManager::getInstance()->GetFont("DIY_LEVEL_GUI_TEXT_LARGE"), levelSelectLabelLocation, ScreenAnchor::CENTER, Colour::Yellow());

	/* BUTTONS */
	Vec2 backButtonLocation = Vec2(-300, -200);
	Vec2 loadLevelButtonLocation = Vec2(300, -200);

	backButton = new TextButton(level_, backButtonLocation, "Back", FontManager::getInstance()->GetFont("LEVEL_GUI_BUTTON_FONT"), "LEVEL_GUI_BLANK", "LEVEL_GUI_BLANK_HOVERED", ScreenAnchor::CENTER, Colour::Yellow());
	loadLevelButton = new TextButton(level_, loadLevelButtonLocation, "Load Level", FontManager::getInstance()->GetFont("LEVEL_GUI_BUTTON_FONT"), "LEVEL_GUI_BLANK", "LEVEL_GUI_BLANK_HOVERED", ScreenAnchor::CENTER, Colour::Yellow());

	/* LEVELS */
	Vec2 levelButtonsLocation = levelSelectLabelLocation + Vec2(-276, -130);
	Vec2 currentlevelButtonsLocation;

	int levelNumber = 1;
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			currentlevelButtonsLocation = levelButtonsLocation + Vec2(x * 276, -y * 77);
			levelButtons.insert(std::pair<std::string, LevelSelectButton*>("Level" + IntToString(levelNumber, 3) + ".xml", new LevelSelectButton(level_, currentlevelButtonsLocation, "LEVEL " + IntToString(levelNumber))));
			levelNumber++;
		}
	}

	//Need to get a list of xml files from database. For now, will manually insert.
	//levelButtons.insert(std::pair<std::string, LevelSelectButton*>("testLevel.xml", new LevelSelectButton(level_, levelButtonsLocation, "TEST LEVEL")));
	//levelButtonsLocation += Vec2(0, -70);
	//levelButtons.insert(std::pair<std::string, LevelSelectButton*>("broken.xml", new LevelSelectButton(level_, levelButtonsLocation, "Invalid LEVEL")));
}

DIY_Level_VictoryGUI::~DIY_Level_VictoryGUI(){}

void DIY_Level_VictoryGUI::HideAll()
{
	Background->Hide();
	HideReview();
	HideLevelSelect();
}

void DIY_Level_VictoryGUI::HideReview()
{
	victoryLabel->Hide();
	strokeCountLabel->Hide();
	strokeLabel->Hide();
	parCountLabel->Hide();
	parLabel->Hide();
	slashLabel->Hide();
	retryButton->Hide();
	levelSelectButton->Hide();
	exitButton->Hide();
}

void DIY_Level_VictoryGUI::ShowReview()
{
	Background->Show();

	victoryLabel->Show();
	strokeCountLabel->Show();
	strokeLabel->Show();
	parCountLabel->Show();
	parLabel->Show();
	slashLabel->Show();
	retryButton->Show();
	levelSelectButton->Show();
	exitButton->Show();

	HideLevelSelect();
}

void DIY_Level_VictoryGUI::HideLevelSelect()
{
	levelSelectLabel->Hide();
	backButton->Hide();
	loadLevelButton->Hide();

	for (auto it = levelButtons.begin(); it != levelButtons.end(); it++)
	{
		it->second->Hide();
	}
}

void DIY_Level_VictoryGUI::ShowLevelSelect()
{
	Background->Show();

	levelSelectLabel->Show();
	backButton->Show();
	loadLevelButton->Show();

	for (auto it = levelButtons.begin(); it != levelButtons.end(); it++)
	{
		it->second->Show();
	}

	HideReview();
}