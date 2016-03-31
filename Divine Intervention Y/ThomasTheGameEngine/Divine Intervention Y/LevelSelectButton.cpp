#include "LevelSelectButton.h"
#include <ModelManager.h>

LevelSelectButton::LevelSelectButton(Level* level_, Vec2 screenPosition_, std::string labelText_, ScreenAnchor anchor_, Colour unpressedTextColour_, Colour pressedTextColour_ )
: TextButton(level_, screenPosition_, labelText_, FontManager::getInstance()->GenerateFont("LevelSelectButtonFont", 42, "Font/ostrich-regular.ttf"), Models->loadTexture("LevelSelectButtonTextureUnSelected", "Images/Level GUI/Buttons/Blank.png"), 
	Models->loadTexture("LevelSelectButtonTextureSelected", "Images/Level GUI/Buttons/BlankHovered.png"), anchor_, unpressedTextColour_, pressedTextColour_)
{
	isSelected = false;
}

void LevelSelectButton::SelectSprite()
{
	if (isSelected)
		SetTexture(textureHover);
	else
		SetTexture(textureUnpressed);

	if (buttonState == UNPRESSED)
	{
		label.SetTextColor(unpressedTextColour);
	}
	else if (buttonState == PRESSED)
	{
		if (isBeingHovered)
		{
			label.SetTextColor(pressedTextColour);
		}
		else
		{
			label.SetTextColor(unpressedTextColour);
		}
	}
}