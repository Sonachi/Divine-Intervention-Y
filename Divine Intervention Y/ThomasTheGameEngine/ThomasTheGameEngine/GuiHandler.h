//Author:	Mathieu Violette
//Date:		1/26/2016

#pragma once

#include <vector>
#include "Button.h"

class GuiHandler
{
private:
	GuiHandler();
	std::vector<Button*> registeredBtns;
public:
	
	static GuiHandler* instance;
	static GuiHandler* getInstance()
	{
		if (!instance)instance = new GuiHandler();
		return instance;
	}
		
	~GuiHandler();

	bool HandleEventMouseDown(int x, int y);
	bool HandleEventMouseUp(int x, int y);
	bool HandleEventMouseHover(int x, int y);
	void SubscribeButton(Button& btn);
	void RemoveButton();
	void RemoveButton(Button& btn);
	void RemoveAll();
};