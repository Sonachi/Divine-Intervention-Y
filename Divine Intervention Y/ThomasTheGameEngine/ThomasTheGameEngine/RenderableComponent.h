#pragma once
#include "Component.h"
#include <string>
#include "Material.h"
#include <vector>
#include "Colour.h"

typedef unsigned int UINT32;
class Renderable;
class Shader;

/*
	A component that contains all the graphical information that can be
	associated with a gameObject. This component helps to ensure that
	models draw themselves and are being drawn based on the desired
	graphics engine.
*/
class RenderableComponent : public Component {

public:
	//An identifier string that will be used to find
	//the model information from the modelManager
	RenderableComponent(GameObject* _parent, Shader* shaderProgram_);
	~RenderableComponent();

	//Draw the model
	virtual void Render() = 0;

	static std::vector<RenderableComponent*> renderableComponents;
	static void DrawRenderables();

	UINT32 modelName;
	std::vector<UINT32> textureName;
protected:
	Shader* shaderProgram;

	Renderable* GetModel(std::string ID_);
	Renderable* GetModel(UINT32 ID_);
};

/**********************************************************************************/
/*                                CHILD CLASSES                                   */
/**********************************************************************************/

class Generic_RenderableComponent : public RenderableComponent
{
public:
	Material * mat;
	Vec3 isEffectedByLight;
	float opacity;

	Generic_RenderableComponent(GameObject* parent_, std::string modelID_, std::string textureID_, float opacity_ = 1, Material * mat_ = new Material(1.0f, 1.0f, 1.0f));

	~Generic_RenderableComponent();

	virtual void Render();

	void SetEffecctedByLight(bool _directional = true, bool _point = true, bool _spot = true);
};

class GUI_RenderableComponent : public RenderableComponent
{
public:
	float drawPercent;
	float opacity;

	GUI_RenderableComponent(GameObject* parent_, UINT32 modelID_, UINT32 textureID_, float opacity_ = 1);

	~GUI_RenderableComponent();

	virtual void Render();
};

class Rainbow_GUI_RenderableComponent : public RenderableComponent
{
public:
	float drawPercent;

	Rainbow_GUI_RenderableComponent(GameObject* parent_, UINT32 modelID_, UINT32 textureID_);

	~Rainbow_GUI_RenderableComponent();

	virtual void Render();
};

class Sun_RenderableComponent : public RenderableComponent
{
public:
	Sun_RenderableComponent(GameObject* parent_, std::string modelID_);

	~Sun_RenderableComponent();

	virtual void Render();

	Vec3 offset;
	float intensity;
	Colour coreColour, rippleColour;
};

class Atmosphere_RenderableComponent : public RenderableComponent
{
public:
	Atmosphere_RenderableComponent(GameObject* parent_, std::string modelID_);

	~Atmosphere_RenderableComponent();

	virtual void Render();

	Vec3 offset;
	float intensity;
	Colour atmosphereColour;
};

class Glow_RenderableComponent : public RenderableComponent
{
public:
	Glow_RenderableComponent(GameObject* parent_, std::string modelID_);

	~Glow_RenderableComponent();

	virtual void Render();

	Vec3 offset;
	Colour glowColour;
};