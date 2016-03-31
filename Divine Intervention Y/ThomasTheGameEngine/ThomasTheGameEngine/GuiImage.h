//Author:	Mathieu Violette
//Date:		12/03/2015

#ifndef GUI_IMAGE_H
#define GUI_IMAGE_H

#include "GuiElement.h"
#include <string>
#include "Texture.h"

class Level;
enum Shader_Program_Type;

class GuiImage : public GuiElement
{
protected:
public:

	GuiImage(Level* _level, std::string _textureName, Vec2 _screenPosition, ScreenAnchor anchor_ = ScreenAnchor::BOTTOM_LEFT, bool isRainbow = false);
	
	~GuiImage();
	virtual void Update(float _deltaTime) override;

	virtual void OnClick();
	virtual void OnHover();

	Shader_Program_Type type;
};

#endif