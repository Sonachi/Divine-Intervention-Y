//Author:	Mathieu Violette
//Date:		1/27/2016

#pragma once

#include <Button.h>

class CounterButton : public Button
{
public:
	CounterButton(int* counter_, Level* _level, Vec2 _screenPosition, std::string textureUnpressed_, std::string texturePressed_, std::string textureHover_, ScreenAnchor anchor_ = ScreenAnchor::BOTTOM_LEFT);
	~CounterButton();

	virtual void OnClick() override;
	virtual void OnHover() override;

	int* counter;
};