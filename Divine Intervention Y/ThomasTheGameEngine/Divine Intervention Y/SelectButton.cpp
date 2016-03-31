#include "LevelSelectButton.h"
#include <ModelManager.h>

SelectButton::SelectButton(Level* level_, Vec2 screenPosition_, std::string labelText_, TTF_Font* font_, ScreenAnchor anchor_, Colour unpressedTextColour_, Colour pressedTextColour_, Colour disabledColour_)
: TextButton(level_, screenPosition_, labelText_, font_, Models->loadTexture("LevelSelectButtonTextureUnSelected", "Images/Level GUI/Buttons/Blank.png"),
Models->loadTexture("LevelSelectButtonTextureSelected", "Images/Level GUI/Buttons/BlankHovered.png"), anchor_, unpressedTextColour_, pressedTextColour_)
{
	isSelected = false;
	pressedTextColour = pressedTextColour_;
	unpressedTextColour = unpressedTextColour_;
	disabledTextColour = disabledColour_;
}

void SelectButton::SelectSprite()
{
	if (isSelected)
	{
		if (buttonState == UNPRESSED)
		{
			if (isBeingHovered)
				SetTexture(textureHover);
			else
				SetTexture(textureUnpressed);

			label.SetTextColor(unpressedTextColour);
		}
		else if (buttonState == PRESSED)
		{
			if (isBeingHovered)
			{
				SetTexture(texturePressed);
				label.SetTextColor(pressedTextColour);
			}
			else
			{
				SetTexture(textureUnpressed);
				label.SetTextColor(unpressedTextColour);
			}
		}
	}
	else
	{
		SetTexture(textureUnpressed);
		label.SetTextColor(disabledTextColour);
	}
}