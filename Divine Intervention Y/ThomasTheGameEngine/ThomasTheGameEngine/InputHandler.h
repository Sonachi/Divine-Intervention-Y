#pragma once

#include <map>
#include <SDL.h>
#include "../Math/two_dimensions.hpp"

#define Input InputController::getInstance()

class InputController {
	std::map < SDL_Keycode, bool > curKeyboardMap;
	std::map < SDL_Keycode, bool > oldKeyboardMap;

	std::map < Uint8, bool > curMouseMap;
	std::map < Uint8, bool > oldMouseMap;

	Vec2 oldMousePos;
	Vec2 curMousePos;

	

	void hitKey(SDL_Keycode);
	void releaseKey(SDL_Keycode);
	void hitMouse(Uint8);
	void releaseMouse(Uint8);

	InputController();
public:
	~InputController();

	//singleton instance
	static InputController * instance;

	static InputController * getInstance()
	{
		if (!instance)
			instance = new InputController();

		return instance;
	}

	bool isKeyDown(SDL_Keycode);
	bool isKeyPressed(SDL_Keycode);
	bool isKeyReleased(SDL_Keycode);
	bool isMouseDown(Uint8);
	bool isMousePressed(Uint8);
	bool isMouseReleased(Uint8);
	bool isAnyKeyPressed();
	Vec2 deltaMouse();
	Vec2 mousePos();
	int mouseWheel;

	SDL_MouseMotionEvent motionEvent;

	//Just throw any input event here, will handle inside the input controller
	void takeEvent(SDL_Event event_);

	//Does not need a dt, simply moves current to previous
	void Update();

	//NOTES------------------------
	//To add game controller will need to add a new set of maps/functions because SDL has done it that way
	//There are 125 keys in the keyboard list, a standard keyboard has 104, there aer 12 extra F keys and I have no idea what else. enjoy
	//if there is a key missing from the list add it into the constructor at the bottom of the list
};