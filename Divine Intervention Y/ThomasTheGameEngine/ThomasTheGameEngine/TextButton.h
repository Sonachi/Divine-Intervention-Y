//Author:	Mathieu Violette
//Date:		2/25/2015

#pragma once

#include "Button.h"
#include "Label.h"

class TextButton : public Button
{
protected:
	Label label;
	Colour unpressedTextColour, pressedTextColour;
public:
	TextButton(Level* level_, Vec2 screenPosition_, std::string labelText_, TTF_Font* font_, std::string textureUnpressed_, std::string textureHover_, ScreenAnchor anchor_ = ScreenAnchor::BOTTOM_LEFT, Colour unpressedTextColour_ = Colour::Black(), Colour pressedTextColour_ = Colour::Lime());
	~TextButton();

	virtual void Show() override;
	virtual void Hide() override;
	
	virtual void SelectSprite() override;

	//Sets text colour. If nullptr, it will not change that colour.
	void SetTextColour(Colour* unpressedTextColour_, Colour* pressedTextColour_);
};