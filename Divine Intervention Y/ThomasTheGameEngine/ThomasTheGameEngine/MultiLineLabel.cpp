#include "Label.h"
#include <SDL_image.h>
#include "ModelManager.h"
#include "Level.h"
#include "RenderableComponent.h"
#include "GameProperties.h"

MultiLineLabel::MultiLineLabel(Level* _level, std::string _text, Uint32 textWrapSize_, TTF_Font* _font, Vec2 _screenPosition, ScreenAnchor anchor_, Colour _textColour)
: GuiElement(_level, _screenPosition, GuiType::LABEL, anchor_), font(_font), text(_text), textColour(_textColour), textWrapSize(textWrapSize_)
{
	//create a texture based off text
	txtSprite = new MultiLineTextSprite(text, font, textColour, textWrapSize);
	texture = txtSprite->mapAddress;

	//Adjust scale so texture appears at 1:1 pixel size on the screen
	guiScale = Vec2(ModelManager::getInstance()->GetTextureWidth(texture) / (float)GameProperties::getInstance()->getVideoProperties()->screenWidth,
		ModelManager::getInstance()->GetTextureHeight(texture) / (float)GameProperties::getInstance()->getVideoProperties()->screenHeight);
	scale = Vec3(guiScale.x, guiScale.y, 1);

	//create square model to draw on
	//ModelManager::getInstance()->CreatePlane("label", 0.01f, 0.01f);
	renderable = new GUI_RenderableComponent(this, ModelManager::getInstance()->GetModelID("GUI"), texture);
}

void MultiLineLabel::RecreateText(std::string _text, Uint32 textWrapSize_)
{
	txtSprite->ReCreateTexture(text, font, textColour, textWrapSize);

	//undo previous guiscale
	scale = Vec3(scale.x / guiScale.x, scale.y / guiScale.y, scale.z);

	//Recalculate guiscale
	guiScale = Vec2(ModelManager::getInstance()->GetTextureWidth(texture) / (float)GameProperties::getInstance()->getVideoProperties()->screenWidth,
		ModelManager::getInstance()->GetTextureHeight(texture) / (float)GameProperties::getInstance()->getVideoProperties()->screenHeight);

	//apply new guiscale
	scale = Vec3(guiScale.x * scale.x, guiScale.y * scale.y, scale.z);
}

MultiLineLabel::~MultiLineLabel()
{
	std::cout << "   ?????  DELETING LABEL  ?????\n";
}

void MultiLineLabel::Update(float _deltaTime){}


void MultiLineLabel::OnClick(){}

void MultiLineLabel::OnHover(){}

void MultiLineLabel::SetTextColor(Colour _color)
{
	textColour = _color;

	txtSprite->ReCreateTexture(text, font, textColour, textWrapSize);
}

void MultiLineLabel::SetText(std::string _text)
{
	text = _text;

	RecreateText(text, textWrapSize);
}

Uint32 MultiLineLabel::GetWrapSize() const
{
	return textWrapSize;
}

void MultiLineLabel::SetText(std::string _text, Uint32 textWrapSize_)
{
	text = _text;
	textWrapSize = textWrapSize_;

	RecreateText(text, textWrapSize);
}

void MultiLineLabel::SetWrapSize(Uint32 textWrapSize_)
{
	textWrapSize = textWrapSize_;

	RecreateText(text, textWrapSize);
}