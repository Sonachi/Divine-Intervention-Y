//Author:	Mathieu Violette
//Date:		12/01/2015

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "GuiElement.h"
#include "FontManager.h"
#include "Colour.h"
#include <string>
#include "TextSprite.h"

class Level;

enum ButtonState{ UNPRESSED, PRESSED, HOVER };

class Button : public GuiElement
{
protected:
	UINT32 textureUnpressed, texturePressed, textureHover;
	void SetTexture(UINT32 texture_);

	bool isClicked;
public:

	Button(Level* _level, Vec2 _screenPosition, std::string textureUnpressed_, std::string texturePressed_, std::string textureHover_, ScreenAnchor anchor_ = ScreenAnchor::BOTTOM_LEFT);
	~Button();
	virtual void Update(float _deltaTime) override;
	virtual void CleanUp() override;

	ButtonState buttonState;
	bool isBeingHovered;

	virtual void OnClick();
	virtual void OnHover();

	virtual void SelectSprite();

	bool HasBeenClicked();
};

#endif