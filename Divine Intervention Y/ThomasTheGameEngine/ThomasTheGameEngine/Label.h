//Author:	Mathieu Violette
//Date:		12/01/2015

#pragma once

#include "GuiElement.h"
#include "FontManager.h"
#include "Colour.h"
#include <string>
#include "TextSprite.h"

class Level;

class Label : public GuiElement
{
protected:
	TextSprite* txtSprite;
	std::string text;
	TTF_Font* font;
	Colour textColour;
public:
	
	Label(Level* _level, std::string _text, TTF_Font* _font, Vec2 _screenPosition, ScreenAnchor anchor_ = ScreenAnchor::BOTTOM_LEFT, Colour _textColour = Colour::Black());
	~Label();
	virtual void Update(float _deltaTime) override;
	//virtual void Draw() override;

	virtual void OnClick();
	virtual void OnHover();

	void SetTextColor(Colour _colour);

	void SetText(std::string _text);
	std::string GetText() const;
};

class MultiLineLabel : public GuiElement
{
protected:
	MultiLineTextSprite* txtSprite;
	std::string text;
	TTF_Font* font;
	Colour textColour;
	Uint32 textWrapSize;

	void RecreateText(std::string _text, Uint32 textWrapSize_);
public:

	MultiLineLabel(Level* _level, std::string _text, Uint32 textWrapSize_, TTF_Font* _font, Vec2 _screenPosition, ScreenAnchor anchor_ = ScreenAnchor::BOTTOM_LEFT, Colour _textColour = Colour::Black());
	~MultiLineLabel();
	virtual void Update(float _deltaTime) override;
	//virtual void Draw() override;

	virtual void OnClick();
	virtual void OnHover();

	void SetTextColor(Colour _colour);
	void SetText(std::string _text);
	void SetText(std::string _text, Uint32 textWrapSize_);
	void SetWrapSize(Uint32 textWrapSize_);

	std::string GetText() const;
	Uint32 GetWrapSize() const;
};