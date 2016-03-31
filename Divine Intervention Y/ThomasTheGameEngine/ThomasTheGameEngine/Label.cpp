#include "Label.h"
#include <SDL_image.h>
#include "ModelManager.h"
#include "Level.h"
#include "RenderableComponent.h"
#include "GameProperties.h"

Label::Label(Level* _level, std::string _text, TTF_Font* _font, Vec2 _screenPosition, ScreenAnchor anchor_, Colour _textColour)
	: GuiElement(_level, _screenPosition, GuiType::LABEL, anchor_), font(_font), text(_text), textColour(_textColour)
{
	//create a texture based off text
	txtSprite = new TextSprite(_text, _font, _textColour);
	texture = txtSprite->mapAddress;

	//Adjust scale so texture appears at 1:1 pixel size on the screen
	guiScale = Vec2(ModelManager::getInstance()->GetTextureWidth(texture) / (float)GameProperties::getInstance()->getVideoProperties()->screenWidth,
		ModelManager::getInstance()->GetTextureHeight(texture) / (float)GameProperties::getInstance()->getVideoProperties()->screenHeight);
	scale = Vec3(guiScale.x, guiScale.y, 1);

	//create square model to draw on
	//ModelManager::getInstance()->CreatePlane("label", 0.01f, 0.01f);
	renderable = new GUI_RenderableComponent(this, ModelManager::getInstance()->GetModelID("GUI"), texture);
}

Label::~Label()
{
	std::cout << "   ?????  DELETING LABEL  ?????\n";
}

void Label::Update(float _deltaTime){}

//void Label::Draw()
//{
//	
//}

void Label::OnClick(){}

void Label::OnHover(){}

void Label::SetTextColor(Colour _color)
{
	textColour = _color;

	txtSprite->ReCreateTexture(text, font, textColour);
}

void Label::SetText(std::string _text)
{
	text = _text;

	txtSprite->ReCreateTexture(text, font, textColour);

	//undo previous guiscale
	scale = Vec3(scale.x / guiScale.x, scale.y / guiScale.y, scale.z);
	
	//Recalculate guiscale
	guiScale = Vec2(ModelManager::getInstance()->GetTextureWidth(texture) / (float)GameProperties::getInstance()->getVideoProperties()->screenWidth,
		ModelManager::getInstance()->GetTextureHeight(texture) / (float)GameProperties::getInstance()->getVideoProperties()->screenHeight);

	//apply new guiscale
	scale = Vec3(guiScale.x * scale.x, guiScale.y * scale.y, scale.z);

}
