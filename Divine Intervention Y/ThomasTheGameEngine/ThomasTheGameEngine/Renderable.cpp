#include "Renderable.h"
#include <glew.h>
#include "GameObject.h"
#include "ModelManager.h"
#include "Texture.h"
#include "Material.h"
#include "GameProperties.h"
#include "GuiElement.h"
#include "RenderableComponent.h"

Draw_Mode Renderable::GetDrawMode() const
{
	return drawMode;
}

//void OpenGL_Renderable::DrawUI(GameObject& parentTransform, Material * _mat, std::vector<UINT32> _textureNames, ScreenAnchor anchor_)
//{
//	//Get textures
//	std::vector<Texture*> _textures;
//	for (int i = 0; i < _textureNames.size(); i++)
//		_textures.push_back(ModelManager::getInstance()->getTexture(_textureNames[i]));
//
//	//Switch Face Culling Mode
//	switch (drawMode)
//	{
//	case ModelManager::Draw_Mode::CCW:
//		glFrontFace(GL_CCW);
//		break;
//	case ModelManager::Draw_Mode::CW:
//		glFrontFace(GL_CW);
//		break;
//	}
//	
//	//IS UI
//	float _ui[1] = { ((GuiElement*)&parentTransform)->drawType };
//	glUniform1fv(ModelManager::getInstance()->UI_DRAW_Location, 1, _ui);
//	float _UIdrawPercent[1] = { ((GuiElement*)&parentTransform)->drawPercent };
//	glUniform1fv(ModelManager::getInstance()->UI_DrawPercent_Location, 1, _UIdrawPercent);
//
//	float _drawStyle[1] = { parentTransform.drawStyle };
//	glUniform1fv(ModelManager::getInstance()->DrawStyle_Location, 1, _drawStyle);
//	float _rRand[1] = { parentTransform.rainbowRand };
//	glUniform1fv(ModelManager::getInstance()->RainbowRand_Location, 1, _rRand);
//
//	glDisable(GL_CULL_FACE);
//	glDisable(GL_DEPTH_TEST);
//
//	//Effected by light?
//	float effected[3] = { isEffectedByLight.x, isEffectedByLight.y, isEffectedByLight.z };
//	glUniform3fv(ModelManager::getInstance()->isEffectedByLight_Location, 1, effected);
//
//	//Get Transform Stuff
//
//	//Convert from screen pixels to screen percent
//	Matrix4 GUITransform = parentTransform.toMat4();
//
//	//Adjust Position based on anchor location
//	switch (anchor_)
//	{
//		case ScreenAnchor::TOP_LEFT:
//			GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight;
//			break;
//		case ScreenAnchor::TOP_CENTER:
//			GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth / 2.0f;
//			GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight;
//			break;
//		case ScreenAnchor::TOP_RIGHT:
//			GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth;
//			GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight;
//			break;
//		case ScreenAnchor::CENTER_LEFT:
//			GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight / 2.0f;
//			break;
//		case ScreenAnchor::CENTER:
//			GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth / 2.0f;
//			GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight / 2.0f;
//			break;
//		case ScreenAnchor::CENTER_RIGHT:
//			GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth;
//			GUITransform.values[7] += GameProperties::getInstance()->getVideoProperties()->screenHeight / 2.0f;
//			break;
//		case ScreenAnchor::BOTTOM_LEFT:
//			//Default, do nothing
//			break;
//		case ScreenAnchor::BOTTOM_CENTER:
//			GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth / 2.0f;
//			break;
//		case ScreenAnchor::BOTTOM_RIGHT:
//			GUITransform.values[3] += GameProperties::getInstance()->getVideoProperties()->screenWidth;
//			break;
//	}
//
//	//X position
//	GUITransform.values[3] = (GUITransform.values[3] / (float)GameProperties::getInstance()->getVideoProperties()->screenWidth) * 2 - 1;
//	
//	//Y Position
//	GUITransform.values[7] = (GUITransform.values[7] / (float)GameProperties::getInstance()->getVideoProperties()->screenHeight) * 2 - 1;
//
//	glUniformMatrix4fv(ModelManager::getInstance()->transformLocation, 1, GL_FALSE, GUITransform.transpose().values);
//	glUniformMatrix4fv(ModelManager::getInstance()->rotateLocation, 1, GL_FALSE, Matrix4(parentTransform.rotation).transpose().values);
//
//	//MATERIAL UNIFORM JUNK
//	float mat[] {_mat->ambient, _mat->diffuse, _mat->specular};
//	glUniform3fv(ModelManager::getInstance()->materialLocation, 1, mat);
//
//	for (int m = 0; m < meshes.size(); m++)
//	{
//		//Get Texture
//		if (m < _textures.size() && _textures[m])
//		{
//			glBindTexture(GL_TEXTURE_2D, _textures[m]->address);
//		}
//		else if (_textures[0])
//			glBindTexture(GL_TEXTURE_2D, _textures[0]->address);
//
//		glDrawArrays(GL_TRIANGLES, meshes[m].edge[0], meshes[m].vertex.size());
//	}
//}