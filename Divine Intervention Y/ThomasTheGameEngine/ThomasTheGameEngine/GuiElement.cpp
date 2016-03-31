#include "GuiElement.h"
#include "Level.h"
#include "RenderableComponent.h"
#include "ModelManager.h"
#include "GameProperties.h"
#include "Shader.h"

GuiElement::GuiElement(Level* _level, Vec2 _screenPosition, GuiType type_, ScreenAnchor anchor_) : GameObject(nullptr, Vec3(_screenPosition.x, _screenPosition.y, 0)),
anchorPosition(anchor_), type(type_)
{
	isVisible = true;

	//Register with level
	_level->guiElements.push_back(this);

	////For some reason Text is upside down...
	//Rotate(Quat(3.141592654f, Vec3::BasisZ()));
	//Rotate(Quat(3.141592654f, Vec3::BasisY()));

	ModelManager::getInstance()->CreatePlane("GUI", 2, 2);
};

GuiElement::~GuiElement()
{
}

void GuiElement::Draw()
{
	if (renderable && isVisible)
		renderable->Render();

	CleanUp();
}

void GuiElement::CleanUp(){}

bool GuiElement::IsVisible() { return isVisible; }

void GuiElement::Show() { isVisible = true; }

void GuiElement::Hide() { isVisible = false; }

bool GuiElement::CheckMouseCollision(int _x, int _y)
{
	//Flip mouse Y to correct position
	//SDL gives 0,0 in top left corner of the screen, 
	//	while this function acts as if it's in the lower left corner
	int halfScreenHeight = GameProperties::getInstance()->getVideoProperties()->screenHeight / 2.0f;
	_y = ((_y - halfScreenHeight) * -1) + halfScreenHeight;

	//Construct rectangle bounds of the button
	int rX, rY, rW, rH;
		
	rW = ModelManager::getInstance()->GetTextureWidth(texture) * scale.x / guiScale.x;
	rH = ModelManager::getInstance()->GetTextureHeight(texture) * scale.y / guiScale.y;

	rX = position.x - rW / 2.0f;
	rY = position.y - rH / 2.0f;

	//Adjust for anchor position
	switch (anchorPosition)
	{
	case ScreenAnchor::TOP_LEFT:
		rY += GameProperties::getInstance()->getVideoProperties()->screenHeight;
		break;
	case ScreenAnchor::TOP_CENTER:
		rX += GameProperties::getInstance()->getVideoProperties()->screenWidth / 2.0f;
		rY += GameProperties::getInstance()->getVideoProperties()->screenHeight;
		break;
	case ScreenAnchor::TOP_RIGHT:
		rX += GameProperties::getInstance()->getVideoProperties()->screenWidth;
		rY += GameProperties::getInstance()->getVideoProperties()->screenHeight;
		break;
	case ScreenAnchor::CENTER_LEFT:
		rY += GameProperties::getInstance()->getVideoProperties()->screenHeight / 2.0f;
		break;
	case ScreenAnchor::CENTER:
		rX += GameProperties::getInstance()->getVideoProperties()->screenWidth / 2.0f;
		rY += GameProperties::getInstance()->getVideoProperties()->screenHeight / 2.0f;
		break;
	case ScreenAnchor::CENTER_RIGHT:
		rX += GameProperties::getInstance()->getVideoProperties()->screenWidth;
		rY += GameProperties::getInstance()->getVideoProperties()->screenHeight / 2.0f;
		break;
	case ScreenAnchor::BOTTOM_LEFT:
		//Default, do nothing
		break;
	case ScreenAnchor::BOTTOM_CENTER:
		rX += GameProperties::getInstance()->getVideoProperties()->screenWidth / 2.0f;
		break;
	case ScreenAnchor::BOTTOM_RIGHT:
		rX += GameProperties::getInstance()->getVideoProperties()->screenWidth;
		break;
	}

	//Check if x & y are contained within bounds
	return (_x >= rX &&
			_x <= rX + rW &&
			_y >= rY &&
			_y <= rY + rH);
}

void GuiElement::Scale(Vec3 scale_)
{
	scale = Vec3(scale.x / guiScale.x, scale.y / guiScale.y, scale.z);

	scale = Vec3(guiScale.x * scale.x * scale_.x, guiScale.y * scale.y * scale_.y, scale.z * scale_.z);

	for (auto it = childObjects.begin(); it != childObjects.end(); it++){
		(*it)->Scale(scale_);
	}
}

void GuiElement::SetDrawPercent(float drawPercent_)
{
	((GUI_RenderableComponent*)renderable)->drawPercent = drawPercent_;
}

float GuiElement::GetDrawPercent() const
{
	return ((GUI_RenderableComponent*)renderable)->drawPercent;
}

void GuiElement::SetOpacity(float opacity_)
{
	((GUI_RenderableComponent*)renderable)->opacity = opacity_;
}

float GuiElement::GetOpacity() const
{
	return ((GUI_RenderableComponent*)renderable)->opacity;
}