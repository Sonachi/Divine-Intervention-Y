#include "InputHandler.h"
#include "GuiHandler.h"

#include <iostream>

InputController * InputController::instance;

InputController::InputController(){
	//Populate the maps

	//Mouse buttons
	curMouseMap.insert(std::pair<Uint8,bool>(SDL_BUTTON_LEFT,false));
	curMouseMap.insert(std::pair<Uint8, bool>(SDL_BUTTON_MIDDLE, false));
	curMouseMap.insert(std::pair<Uint8, bool>(SDL_BUTTON_RIGHT, false));
	curMouseMap.insert(std::pair<Uint8, bool>(SDL_BUTTON_X1, false));
	curMouseMap.insert(std::pair<Uint8, bool>(SDL_BUTTON_X2, false));
	oldMouseMap = curMouseMap; //Everything should be false so this just makes sure nothing goes wrong in frame 1

	//MousePosition
	curMousePos = Vec2::Zero();
	oldMousePos = Vec2::Zero();
	motionEvent = SDL_MouseMotionEvent();

	//Keyboard buttons
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_0, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_1, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_2, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_3, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_4, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_5, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_6, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_7, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_8, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_9, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_a, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_b, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_c, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_d, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_e, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_f, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_g, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_h, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_i, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_j, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_k, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_l, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_m, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_n, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_o, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_p, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_q, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_r, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_s, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_t, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_u, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_v, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_w, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_x, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_y, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_z, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_BACKSPACE, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_CANCEL, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_CAPSLOCK, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_COMMA, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_DELETE, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_DOWN, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_END, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_EQUALS, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_ESCAPE, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F1, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F2, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F3, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F4, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F5, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F6, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F7, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F8, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F9, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F10, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F11, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F12, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F13, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F14, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F15, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F16, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F17, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F18, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F19, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F20, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F21, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F22, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F23, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_F24, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_BACKQUOTE, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_HOME, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_INSERT, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_0, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_1, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_2, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_3, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_4, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_5, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_6, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_7, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_8, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_9, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_DECIMAL, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_DIVIDE, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_ENTER, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_PLUS, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_MINUS, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_KP_MULTIPLY, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_LALT, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_LCTRL, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_LEFT, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_LEFTBRACKET, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_LSHIFT, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_MINUS, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_NUMLOCKCLEAR, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_PAGEDOWN, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_PAGEUP, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_PERIOD, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_PRINTSCREEN, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_RALT, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_RCTRL, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_RIGHT, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_LSHIFT, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_RIGHTBRACKET, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_SCROLLLOCK, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_SEMICOLON, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_SLASH, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_SPACE, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_TAB, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_UP, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_PAUSE, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_MENU, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_BACKSLASH, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_RETURN, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_LGUI, false));
	curKeyboardMap.insert(std::pair<SDL_Keycode, bool>(SDLK_RGUI, false));
	oldKeyboardMap = curKeyboardMap; //see comment from mouse map

	//Game controller buttons will go here.
}

bool InputController::isKeyDown(SDL_Keycode key_){
	return (curKeyboardMap[key_]);
}

bool InputController::isKeyPressed(SDL_Keycode key_){
	return (!oldKeyboardMap[key_] && curKeyboardMap[key_]);
}

bool InputController::isKeyReleased(SDL_Keycode key_){
	return (oldKeyboardMap[key_] && !curKeyboardMap[key_]);
}


bool InputController::isMouseDown(Uint8 button_){
	return curMouseMap[button_];
}

bool InputController::isMousePressed(Uint8 button_){
	return (!oldMouseMap[button_] && curMouseMap[button_]);
}

bool InputController::isMouseReleased(Uint8 button_){
	return (oldMouseMap[button_] && !curMouseMap[button_]);
}

bool InputController::isAnyKeyPressed()
{
	for (auto it = curKeyboardMap.begin(); it != curKeyboardMap.end(); it++)
	{
		if (it->second)
			return true;
	}

	for (auto it = curMouseMap.begin(); it != curMouseMap.end(); it++)
	{
		if (it->second)
			return true;
	}

	return false;
}

Vec2 InputController::mousePos(){
	return curMousePos;
}

Vec2 InputController::deltaMouse(){
	return Vec2(motionEvent.xrel, motionEvent.yrel);
}

void InputController::hitKey(SDL_Keycode key_){
	curKeyboardMap[key_] = true;
}

void InputController::releaseKey(SDL_Keycode key_){
	curKeyboardMap[key_] = false;
}

void InputController::hitMouse(Uint8 key_){
	curMouseMap[key_] = true;
}

void InputController::releaseMouse(Uint8 key_){
	curMouseMap[key_] = false;
}

void InputController::Update(){
	oldMousePos = curMousePos;
	oldKeyboardMap = curKeyboardMap;
	oldMouseMap = curMouseMap;
	mouseWheel = 0;

	int x, y;
	SDL_GetMouseState(&x, &y);
	curMousePos.x = x;
	curMousePos.y = y;
}

void InputController::takeEvent(SDL_Event e_){

	if (e_.type == SDL_KEYDOWN)
		hitKey(e_.key.keysym.sym);
	else if (e_.type == SDL_KEYUP)
		releaseKey(e_.key.keysym.sym);
	else if (e_.type == SDL_MOUSEBUTTONDOWN)
	{
		if (e_.button.button == SDL_BUTTON_LEFT)
			GuiHandler::getInstance()->HandleEventMouseDown(e_.motion.x, e_.motion.y);

		hitMouse(e_.button.button);
	}
	else if (e_.type == SDL_MOUSEBUTTONUP)
	{
		if (e_.button.button == SDL_BUTTON_LEFT)
			GuiHandler::getInstance()->HandleEventMouseUp(e_.motion.x, e_.motion.y);
		
		releaseMouse(e_.button.button);
	}
	else if (e_.type == SDL_MOUSEMOTION)
	{
		GuiHandler::getInstance()->HandleEventMouseHover(curMousePos.x, curMousePos.y);
		motionEvent = e_.motion;
	}
	else if (e_.type == SDL_MOUSEWHEEL)
	{
		mouseWheel = e_.wheel.y;
	}

	//Gui events may need to be re-checked
}