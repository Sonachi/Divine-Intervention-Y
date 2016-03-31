#include "GuiImage.h"
#include "Level.h"
#include "ModelManager.h"
#include "RenderableComponent.h"
#include "GameProperties.h"

GuiImage::GuiImage(Level* _level, std::string _textureName, Vec2 _screenPosition, ScreenAnchor anchor_, bool isRainbow)
: GuiElement(_level, _screenPosition, GuiType::IMAGE, anchor_), type(isRainbow ? RAINBOW_GUI_SHADER : GUI_SHADER)
{
	texture = ModelManager::getInstance()->GetTextureID(_textureName);

	if (type == RAINBOW_GUI_SHADER)
		renderable = new Rainbow_GUI_RenderableComponent(this, ModelManager::getInstance()->GetModelID("GUI"), texture);
	else
		renderable = new GUI_RenderableComponent(this, ModelManager::getInstance()->GetModelID("GUI"), texture);

	//Adjust scale so image appears at 1:1 pixel size on the screen
	guiScale = Vec2(ModelManager::getInstance()->GetTextureWidth(texture) / (float)GameProperties::getInstance()->getVideoProperties()->screenWidth,
				 ModelManager::getInstance()->GetTextureHeight(texture) / (float)GameProperties::getInstance()->getVideoProperties()->screenHeight);
	scale = Vec3(guiScale.x, guiScale.y, 1);
}

GuiImage::~GuiImage()
{

}

void GuiImage::Update(float _deltaTime)
{

}

void GuiImage::OnClick()
{

}

void GuiImage::OnHover()
{

}