#include "TextButton.h"
#include <iostream>

TextButton::TextButton(Level* level_, Vec2 screenPosition_, std::string labelText_, TTF_Font* font_, std::string textureUnpressed_, std::string textureHover_, ScreenAnchor anchor_, Colour unpressedTextColour_, Colour pressedTextColour_)
: Button(level_, screenPosition_, textureUnpressed_, textureHover_, textureHover_, anchor_), 
label(level_, labelText_, font_, screenPosition_, anchor_, unpressedTextColour_), unpressedTextColour(unpressedTextColour_), pressedTextColour(pressedTextColour_)
{
}

TextButton::~TextButton()
{
	std::cout << "   ?????  DELETING TEXT BUTTON  ?????\n";
}

void TextButton::Show() 
{ 
	GuiElement::Show();
	label.Show();
}

void TextButton::Hide() 
{ 
	GuiElement::Hide();
	label.Hide();
}

void TextButton::SelectSprite()			 
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

void TextButton::SetTextColour(Colour* unpressedTextColour_, Colour* pressedTextColour_)
{
	if (unpressedTextColour_)
		unpressedTextColour = *unpressedTextColour_;

	if (pressedTextColour_)
		pressedTextColour = *pressedTextColour_;
}