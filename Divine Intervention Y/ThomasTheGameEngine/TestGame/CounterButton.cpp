#include "CounterButton.h"

CounterButton::CounterButton(int* counter_, Level* _level, Vec2 _screenPosition, std::string textureUnpressed_, std::string texturePressed_, std::string textureHover_, ScreenAnchor anchor_)
: Button(_level, _screenPosition, textureUnpressed_, texturePressed_, textureHover_, anchor_)
{
	counter = counter_;
}


CounterButton::~CounterButton()
{

}

void CounterButton::OnClick()
{
	(*counter)++;
}

void CounterButton::OnHover()
{

}