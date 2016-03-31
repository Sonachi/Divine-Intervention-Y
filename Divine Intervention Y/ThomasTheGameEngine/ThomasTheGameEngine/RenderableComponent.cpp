#include "RenderableComponent.h"
#include "ModelManager.h"
#include "Renderable.h"
#include "GameObject.h"
#include "GuiElement.h"
#include "GameProperties.h"
#include "Shader.h"
#include "Texture.h"
#include "Noise.h"

/**********************************************************************************/
/*                          Renderable Component                                  */
/**********************************************************************************/

std::vector<RenderableComponent*> RenderableComponent::renderableComponents;

RenderableComponent::RenderableComponent(GameObject* _parent, Shader* shaderProgram_) : Component(_parent, Component::ComponentType::Renderable_Component), shaderProgram(shaderProgram_)
{
	isEnabled = true;
}
RenderableComponent::~RenderableComponent(){}

void RenderableComponent::DrawRenderables()
{
	for (int i = 0; i < renderableComponents.size(); i++)
	{
		if (renderableComponents[i]->isEnabled)
		{
			renderableComponents[i]->Render();
		}
	}
}

Renderable* RenderableComponent::GetModel(std::string ID_)
{
	return ModelManager::getInstance()->getModel(ID_);
}

Renderable* RenderableComponent::GetModel(UINT32 ID_)
{
	return ModelManager::getInstance()->getModel(ID_);
}

/**********************************************************************************/
/*                                 GENERIC                                        */
/**********************************************************************************/

void Generic_RenderableComponent::SetEffecctedByLight(bool _directional, bool _point, bool _spot)
{
	isEffectedByLight = Vec3(_directional, _point, _spot);
}

Generic_RenderableComponent::Generic_RenderableComponent(GameObject* parent_, std::string modelID_, std::string textureID_, float opacity_, Material * mat_) : RenderableComponent(parent_, Generic_Shader::_GetInstance())
{
	opacity = opacity_;
	modelName = ModelManager::getInstance()->GetModelID(modelID_);
	textureName.push_back(ModelManager::getInstance()->GetTextureID(textureID_));
	mat = mat_;
	isEffectedByLight = Vec3::One();
	RenderableComponent::renderableComponents.push_back(this);
}

Generic_RenderableComponent::~Generic_RenderableComponent(){}

void Generic_RenderableComponent::Render()
{
	Renderable* model = GetModel(modelName);

	if (!model || !shaderProgram)
		return;

	//Get textures
	std::vector<Texture*> _textures;
	for (int i = 0; i < textureName.size(); i++)
		_textures.push_back(ModelManager::getInstance()->getTexture(textureName[i]));

	Generic_Shader* shader_ = (Generic_Shader*)shaderProgram;
	glUseProgram(shader_->GetProgram());

	//Switch Face Culling Mode
	switch (model->GetDrawMode())
	{
	case Draw_Mode::CCW:
		glFrontFace(GL_CCW);
		break;
	case Draw_Mode::CW:
		glFrontFace(GL_CW);
		break;
	}
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	//Effected by light?
	float effected[3] = { isEffectedByLight.x, isEffectedByLight.y, isEffectedByLight.z };
	glProgramUniform3fv(shader_->GetProgram(), shader_->isEffectedByLight_Location, 1, effected);

	glProgramUniform1f(shader_->GetProgram(), shader_->opacity_Location, opacity);

	//Get Transform Stuff
	glProgramUniformMatrix4fv(shader_->GetProgram(), shader_->transform_Location, 1, GL_FALSE, parentObject->toMat4().transpose().values);
	glProgramUniformMatrix4fv(shader_->GetProgram(), shader_->rotate_Location, 1, GL_FALSE, Matrix4(parentObject->rotation).transpose().values);

	//MATERIAL UNIFORM JUNK
	float mat_[] {mat->ambient, mat->diffuse, mat->specular};
	glProgramUniform3fv(shader_->GetProgram(), shader_->material_Location, 1, mat_);

	for (int m = 0; m < model->meshes.size(); m++)
	{
		//Get Texture
		if (m < _textures.size() && _textures[m])
		{
			glBindTexture(GL_TEXTURE_2D, _textures[m]->address);
		}
		else if (_textures[0])
			glBindTexture(GL_TEXTURE_2D, _textures[0]->address);

		glDrawArrays(GL_TRIANGLES, model->meshes[m].edge[0], model->meshes[m].vertex.size());
	}
}

/**********************************************************************************/
/*                                   GUI                                          */
/**********************************************************************************/

GUI_RenderableComponent::GUI_RenderableComponent(GameObject* parent_, UINT32 modelID_, UINT32 textureID_, float opacity_) : RenderableComponent(parent_, GUI_Shader::_GetInstance()), drawPercent(1), opacity(opacity_)
{
	modelName = modelID_;
	textureName.push_back(textureID_);
}

GUI_RenderableComponent::~GUI_RenderableComponent(){}

void GUI_RenderableComponent::Render()
{
	Renderable* model = GetModel(modelName);

	if (!model || !shaderProgram)
		return;

	//Get textures
	std::vector<Texture*> _textures;
	for (int i = 0; i < textureName.size(); i++)
		_textures.push_back(ModelManager::getInstance()->getTexture(textureName[i]));

	GUI_Shader* shader_ = (GUI_Shader*)shaderProgram;
	glUseProgram(shader_->GetProgram());

	//Switch Face Culling Mode
	switch (model->GetDrawMode())
	{
	case Draw_Mode::CCW:
		glFrontFace(GL_CCW);
		break;
	case Draw_Mode::CW:
		glFrontFace(GL_CW);
		break;
	}
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	float _UIdrawPercent[1] = { drawPercent };
	glProgramUniform1fv(shader_->GetProgram(), shader_->drawPercent_Location, 1, _UIdrawPercent);

	glProgramUniform1f(shader_->GetProgram(), shader_->opacity_Location, opacity);

	//Get Transform Stuff
	//Convert from screen pixels to screen percent
	Matrix4 GUITransform = parentObject->toMat4();

	//Adjust Position based on anchor location
	switch (((GuiElement*)parentObject)->anchorPosition)
	{
	case ScreenAnchor::TOP_LEFT:
		GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight;
		break;
	case ScreenAnchor::TOP_CENTER:
		GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth / 2.0f;
		GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight;
		break;
	case ScreenAnchor::TOP_RIGHT:
		GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth;
		GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight;
		break;
	case ScreenAnchor::CENTER_LEFT:
		GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight / 2.0f;
		break;
	case ScreenAnchor::CENTER:
		GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth / 2.0f;
		GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight / 2.0f;
		break;
	case ScreenAnchor::CENTER_RIGHT:
		GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth;
		GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight / 2.0f;
		break;
	case ScreenAnchor::BOTTOM_LEFT:
		//Default, do nothing
		break;
	case ScreenAnchor::BOTTOM_CENTER:
		GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth / 2.0f;
		break;
	case ScreenAnchor::BOTTOM_RIGHT:
		GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth;
		break;
	}

	//X position
	GUITransform.values[3] = (GUITransform.values[3] / (float)GameProperties::getInstance()->getVideoProperties()->screenWidth) * 2 - 1;

	//Y Position
	GUITransform.values[7] = (GUITransform.values[7] / (float)GameProperties::getInstance()->getVideoProperties()->screenHeight) * 2 - 1;

	glProgramUniformMatrix4fv(shader_->GetProgram(), shader_->transform_Location, 1, GL_FALSE, GUITransform.transpose().values);

	for (int m = 0; m < model->meshes.size(); m++)
	{
		//Get Texture
		if (m < _textures.size() && _textures[m])
		{
			glBindTexture(GL_TEXTURE_2D, _textures[m]->address);
		}
		else if (_textures[0])
			glBindTexture(GL_TEXTURE_2D, _textures[0]->address);

		glDrawArrays(GL_TRIANGLES, model->meshes[m].edge[0], model->meshes[m].vertex.size());
	}
}

/**********************************************************************************/
/*                               RAINBOW GUI                                      */
/**********************************************************************************/

Rainbow_GUI_RenderableComponent::Rainbow_GUI_RenderableComponent(GameObject* parent_, UINT32 modelID_, UINT32 textureID_) : RenderableComponent(parent_, Rainbow_GUI_Shader::_GetInstance()), drawPercent(1)
{
	modelName = modelID_;
	textureName.push_back(textureID_);
}

Rainbow_GUI_RenderableComponent::~Rainbow_GUI_RenderableComponent(){}

void Rainbow_GUI_RenderableComponent::Render()
{
	Renderable* model = GetModel(modelName);

	if (!model || !shaderProgram)
		return;

	//Get textures
	std::vector<Texture*> _textures;
	for (int i = 0; i < textureName.size(); i++)
		_textures.push_back(ModelManager::getInstance()->getTexture(textureName[i]));

	Rainbow_GUI_Shader* shader_ = (Rainbow_GUI_Shader*)shaderProgram;
	glUseProgram(shader_->GetProgram());

	//Switch Face Culling Mode
	switch (model->GetDrawMode())
	{
	case Draw_Mode::CCW:
		glFrontFace(GL_CCW);
		break;
	case Draw_Mode::CW:
		glFrontFace(GL_CW);
		break;
	}
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	float _UIdrawPercent[1] = { drawPercent };
	glProgramUniform1fv(shader_->GetProgram(), shader_->drawPercent_Location, 1, _UIdrawPercent);

	//Get Transform Stuff
	//Convert from screen pixels to screen percent
	Matrix4 GUITransform = parentObject->toMat4();

	//Adjust Position based on anchor location
	switch (((GuiElement*)parentObject)->anchorPosition)
	{
	case ScreenAnchor::TOP_LEFT:
		GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight;
		break;
	case ScreenAnchor::TOP_CENTER:
		GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth / 2.0f;
		GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight;
		break;
	case ScreenAnchor::TOP_RIGHT:
		GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth;
		GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight;
		break;
	case ScreenAnchor::CENTER_LEFT:
		GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight / 2.0f;
		break;
	case ScreenAnchor::CENTER:
		GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth / 2.0f;
		GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight / 2.0f;
		break;
	case ScreenAnchor::CENTER_RIGHT:
		GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth;
		GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight / 2.0f;
		break;
	case ScreenAnchor::BOTTOM_LEFT:
		//Default, do nothing
		break;
	case ScreenAnchor::BOTTOM_CENTER:
		GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth / 2.0f;
		break;
	case ScreenAnchor::BOTTOM_RIGHT:
		GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth;
		break;
	}

	//X position
	GUITransform.values[3] = (GUITransform.values[3] / (float)GameProperties::getInstance()->getVideoProperties()->screenWidth) * 2 - 1;

	//Y Position
	GUITransform.values[7] = (GUITransform.values[7] / (float)GameProperties::getInstance()->getVideoProperties()->screenHeight) * 2 - 1;

	glProgramUniformMatrix4fv(shader_->GetProgram(), shader_->transform_Location, 1, GL_FALSE, GUITransform.transpose().values);

	for (int m = 0; m < model->meshes.size(); m++)
	{
		//Get Texture
		if (m < _textures.size() && _textures[m])
		{
			glBindTexture(GL_TEXTURE_2D, _textures[m]->address);
		}
		else if (_textures[0])
			glBindTexture(GL_TEXTURE_2D, _textures[0]->address);

		glDrawArrays(GL_TRIANGLES, model->meshes[m].edge[0], model->meshes[m].vertex.size());
	}
}

/**********************************************************************************/
/*                                   SUN                                          */
/**********************************************************************************/

Sun_RenderableComponent::Sun_RenderableComponent(GameObject* parent_, std::string modelID_) : RenderableComponent(parent_, Sun_Shader::_GetInstance())
{
	modelName = ModelManager::getInstance()->GetModelID(modelID_);
	intensity = 1;
	offset = Vec3();

	//create 3d texture
	textureName.push_back(CreateNoise3D());

	RenderableComponent::renderableComponents.push_back(this);

	coreColour = Colour::Yellow();
	rippleColour = Colour::Orange();
}

Sun_RenderableComponent::~Sun_RenderableComponent(){}

void Sun_RenderableComponent::Render()
{
	Renderable* model = GetModel(modelName);
	Renderable* guiModel = GetModel("GUI");

	if (!model || !guiModel || !shaderProgram)
		return;

	//Get textures
	std::vector<Texture*> _textures;
	for (int i = 0; i < textureName.size(); i++)
		_textures.push_back(ModelManager::getInstance()->getTexture(textureName[i]));

	Sun_Shader* shader_ = (Sun_Shader*)shaderProgram;

	//Render to scene
	glUseProgram(shader_->GetProgram());

	//Switch Face Culling Mode
	switch (model->GetDrawMode())
	{
	case Draw_Mode::CCW:
		glFrontFace(GL_CCW);
		break;
	case Draw_Mode::CW:
		glFrontFace(GL_CW);
		break;
	}
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	//Get Transform Stuff
	glProgramUniformMatrix4fv(shader_->GetProgram(), shader_->transform_Location, 1, GL_FALSE, parentObject->toMat4().transpose().values);
	glProgramUniformMatrix4fv(shader_->GetProgram(), shader_->rotate_Location, 1, GL_FALSE, Matrix4(parentObject->rotation).transpose().values);

	//Light Intensity
	glProgramUniform1f(shader_->GetProgram(), shader_->lightIntensity_Location, intensity);

	//Offset
	glProgramUniform3f(shader_->GetProgram(), shader_->Offset_Location, offset.x, offset.y, offset.z);

	//Texture Scale
	glProgramUniform1f(shader_->GetProgram(), shader_->TextureScale_Location, parentObject->scale.length() / 50.0f);

	//Colours
	glProgramUniform4f(shader_->GetProgram(), shader_->CoreColor_Location, coreColour.r, coreColour.g, coreColour.b, 1);
	glProgramUniform4f(shader_->GetProgram(), shader_->RippleColor_Location, rippleColour.r, rippleColour.g, rippleColour.b, 1);

	for (int m = 0; m < model->meshes.size(); m++)
	{
		if (m < _textures.size() && _textures[m])
		{
			glBindTexture(GL_TEXTURE_3D, _textures[m]->address);
		}
		else if (_textures[0])
			glBindTexture(GL_TEXTURE_3D, _textures[0]->address);

		glDrawArrays(GL_TRIANGLES, model->meshes[m].edge[0], model->meshes[m].vertex.size());
	}
}

/**********************************************************************************/
/*                                ATMOSPHERE                                      */
/**********************************************************************************/

Atmosphere_RenderableComponent::Atmosphere_RenderableComponent(GameObject* parent_, std::string modelID_) : RenderableComponent(parent_, Atmosphere_Shader::_GetInstance())
{
	modelName = ModelManager::getInstance()->GetModelID(modelID_);
	intensity = 1;
	offset = Vec3();

	//create 3d texture
	textureName.push_back(CreateNoise3D());

	RenderableComponent::renderableComponents.push_back(this);

	atmosphereColour = Colour::White();
}

Atmosphere_RenderableComponent::~Atmosphere_RenderableComponent(){}

void Atmosphere_RenderableComponent::Render()
{
	Renderable* model = GetModel(modelName);

	if (!model || !shaderProgram)
		return;

	//Get textures
	std::vector<Texture*> _textures;
	for (int i = 0; i < textureName.size(); i++)
		_textures.push_back(ModelManager::getInstance()->getTexture(textureName[i]));

	Atmosphere_Shader* shader_ = (Atmosphere_Shader*)shaderProgram;

	//Render to scene
	glUseProgram(shader_->GetProgram());

	//Switch Face Culling Mode
	switch (model->GetDrawMode())
	{
	case Draw_Mode::CCW:
		glFrontFace(GL_CCW);
		break;
	case Draw_Mode::CW:
		glFrontFace(GL_CW);
		break;
	}
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	//Get Transform Stuff
	glProgramUniformMatrix4fv(shader_->GetProgram(), shader_->transform_Location, 1, GL_FALSE, parentObject->toMat4().transpose().values);

	//Light Intensity
	glProgramUniform1f(shader_->GetProgram(), shader_->lightIntensity_Location, intensity);

	//Offset
	glProgramUniform3f(shader_->GetProgram(), shader_->Offset_Location, offset.x, offset.y, offset.z);

	//Texture Scale
	glProgramUniform1f(shader_->GetProgram(), shader_->TextureScale_Location, parentObject->scale.length() / 10.0f);

	//Colours
	glProgramUniform4f(shader_->GetProgram(), shader_->AtmosphereColor_Location, atmosphereColour.r, atmosphereColour.g, atmosphereColour.b, 1);

	for (int m = 0; m < model->meshes.size(); m++)
	{
		if (m < _textures.size() && _textures[m])
		{
			glBindTexture(GL_TEXTURE_3D, _textures[m]->address);
		}
		else if (_textures[0])
			glBindTexture(GL_TEXTURE_3D, _textures[0]->address);

		glDrawArrays(GL_TRIANGLES, model->meshes[m].edge[0], model->meshes[m].vertex.size());
	}
}

/**********************************************************************************/
/*                                   GLOW                                         */
/**********************************************************************************/

Glow_RenderableComponent::Glow_RenderableComponent(GameObject* parent_, std::string modelID_) : RenderableComponent(parent_, Glow_Shader::_GetInstance())
{
	modelName = ModelManager::getInstance()->GetModelID(modelID_);
	offset = Vec3();

	//create 3d texture
	textureName.push_back(CreateNoise3D());

	RenderableComponent::renderableComponents.push_back(this);

	glowColour = Colour::White();
}

Glow_RenderableComponent::~Glow_RenderableComponent(){}

void Glow_RenderableComponent::Render()
{
	Renderable* model = GetModel(modelName);

	if (!model || !shaderProgram)
		return;

	//Get textures
	std::vector<Texture*> _textures;
	for (int i = 0; i < textureName.size(); i++)
		_textures.push_back(ModelManager::getInstance()->getTexture(textureName[i]));

	Glow_Shader* shader_ = (Glow_Shader*)shaderProgram;

	//Render to scene
	glUseProgram(shader_->GetProgram());

	//Switch Face Culling Mode
	switch (model->GetDrawMode())
	{
	case Draw_Mode::CCW:
		glFrontFace(GL_CCW);
		break;
	case Draw_Mode::CW:
		glFrontFace(GL_CW);
		break;
	}
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	//Get Transform Stuff
	glProgramUniformMatrix4fv(shader_->GetProgram(), shader_->transform_Location, 1, GL_FALSE, parentObject->toMat4().transpose().values);
	glProgramUniformMatrix4fv(shader_->GetProgram(), shader_->rotate_Location, 1, GL_FALSE, Matrix4(parentObject->rotation).transpose().values);

	//Offset
	glProgramUniform3f(shader_->GetProgram(), shader_->Offset_Location, offset.x, offset.y, offset.z);

	//Texture Scale
	glProgramUniform1f(shader_->GetProgram(), shader_->TextureScale_Location, parentObject->scale.length() / 50.0f);

	//Colours
	glProgramUniform4f(shader_->GetProgram(), shader_->GlowColor_Location, glowColour.r, glowColour.g, glowColour.b, 1);

	for (int m = 0; m < model->meshes.size(); m++)
	{
		if (m < _textures.size() && _textures[m])
		{
			glBindTexture(GL_TEXTURE_3D, _textures[m]->address);
		}
		else if (_textures[0])
			glBindTexture(GL_TEXTURE_3D, _textures[0]->address);

		glDrawArrays(GL_TRIANGLES, model->meshes[m].edge[0], model->meshes[m].vertex.size());
	}
}