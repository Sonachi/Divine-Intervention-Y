//Author:	Mathieu Violette
//Date:		1/26/2016

#include "GuiHandler.h"

GuiHandler * GuiHandler::instance;

GuiHandler::GuiHandler(){}

void GuiHandler::SubscribeButton(Button& btn)
{
	registeredBtns.push_back(&btn);
}

void GuiHandler::RemoveButton()
{
	registeredBtns.pop_back();
}

void GuiHandler::RemoveButton(Button& btn)
{
	for (int i = 0; i < registeredBtns.size(); i++)
	{
		if (&btn == registeredBtns[i])
		{
			registeredBtns.erase(registeredBtns.begin() + i);
			return;
		}
	}
}

void GuiHandler::RemoveAll()
{
	registeredBtns.clear();
}

bool GuiHandler::HandleEventMouseDown(int x, int y)
{
	for (std::vector<Button*>::iterator i = registeredBtns.begin(); i != registeredBtns.end(); ++i)
	{
		if ((*i)->IsVisible() && (*i)->CheckMouseCollision(x, y))
		{
			(*i)->buttonState = PRESSED;
			return true;
		}
	}
	return false;
}

bool GuiHandler::HandleEventMouseUp(int x, int y)
{
	bool succeess = false;
	std::vector<Button*>::iterator target = registeredBtns.begin();

	for (std::vector<Button*>::iterator i = registeredBtns.begin(); i != registeredBtns.end(); ++i)
	{
		if ((*i)->IsVisible() && (*i)->CheckMouseCollision(x, y) && (*i)->buttonState == PRESSED)
		{
			target = i;
			succeess = true;
		}

		(*i)->buttonState = UNPRESSED;
	}

	if (succeess)
		(*target)->OnClick();

	return succeess;
}

bool GuiHandler::HandleEventMouseHover(int x, int y)
{
	bool succeess = false;

	for (std::vector<Button*>::iterator i = registeredBtns.begin(); i != registeredBtns.end(); ++i)
	{
		if ((*i)->IsVisible() && (*i)->CheckMouseCollision(x, y))
		{
			(*i)->isBeingHovered = true;

			succeess = true;
		}
		else
		{
			(*i)->isBeingHovered = false;
		}
	}

	return succeess;
}