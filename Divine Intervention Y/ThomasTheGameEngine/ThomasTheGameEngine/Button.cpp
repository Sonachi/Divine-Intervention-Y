#include "Button.h"
#include <SDL_image.h>
#include "ModelManager.h"
#include "Level.h"
#include "RenderableComponent.h"
#include "GameProperties.h"
#include "GuiHandler.h"

Button::Button(Level* _level, Vec2 _screenPosition, std::string textureUnpressed_, std::string texturePressed_, std::string textureHover_, ScreenAnchor anchor_)
: GuiElement(_level, _screenPosition, GuiType::BUTTON, anchor_)
{
	textureUnpressed = ModelManager::getInstance()->GetTextureID(textureUnpressed_);
	texturePressed = ModelManager::getInstance()->GetTextureID(texturePressed_);
	textureHover = ModelManager::getInstance()->GetTextureID(textureHover_);
	texture = textureUnpressed;

	renderable = new GUI_RenderableComponent(this, ModelManager::getInstance()->GetModelID("GUI"), texture);

	//Adjust scale so texture appears at 1:1 pixel size on the screen
	guiScale = Vec2(ModelManager::getInstance()->GetTextureWidth(texture) / (float)GameProperties::getInstance()->getVideoProperties()->screenWidth,
		ModelManager::getInstance()->GetTextureHeight(texture) / (float)GameProperties::getInstance()->getVideoProperties()->screenHeight);
	scale = Vec3(guiScale.x, guiScale.y, 1);

	isBeingHovered = false;
	buttonState = UNPRESSED;

	GuiHandler::getInstance()->SubscribeButton(*this);
}

Button::~Button()
{
	GuiHandler::getInstance()->RemoveButton(*this);
}

void Button::Update(float _deltaTime)
{
	SelectSprite();
}

void Button::OnClick()
{
	isClicked = true;
}

void Button::OnHover(){}

void Button::SetTexture(UINT32 texture_)
{
	//undo previous guiscale
	scale = Vec3(scale.x / guiScale.x, scale.y / guiScale.y, scale.z);

	texture = texture_;
	renderable->textureName[0] = texture;

	//Recalculate guiscale
	guiScale = Vec2(ModelManager::getInstance()->GetTextureWidth(texture) / (float)GameProperties::getInstance()->getVideoProperties()->screenWidth,
		ModelManager::getInstance()->GetTextureHeight(texture) / (float)GameProperties::getInstance()->getVideoProperties()->screenHeight);

	//apply new guiscale
	scale = Vec3(guiScale.x * scale.x, guiScale.y * scale.y, scale.z);
}

void Button::SelectSprite()
{
	if (buttonState == UNPRESSED)
	{
		if (isBeingHovered)
			SetTexture(textureHover);
		else
			SetTexture(textureUnpressed);
	}
	else if (buttonState == PRESSED)
	{
		if (isBeingHovered)
			SetTexture(texturePressed);
		else
			SetTexture(textureUnpressed);
	}
}

//Returns True if the Button has been pressed.
bool Button::HasBeenClicked()
{
	return isClicked;
}

void Button::CleanUp()
{
	isClicked = false;
}