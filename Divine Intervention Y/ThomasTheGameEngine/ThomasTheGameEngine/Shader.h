#pragma once
//Author:	Mathieu Violette
//Date:		3/09/2016

#include <glew.h>
#include <vector>
#include "../Math/four_dimensions.hpp"

/*********************************************************/
/*                       BASE                            */
/*********************************************************/
enum Attribute_Type
{
	VERTEX_ATTRIBUTE, TEXTURE_ATTRIBUTE, NORMAL_ATTRIBUTE
};

class Shader
{
public:
	Shader(GLuint program_);
	virtual ~Shader() = 0;

	GLuint GetProgram() const;

	//static const GLuint NUMBER_OF_ATTRIBUTES = 3;
	//GLuint attributes[NUMBER_OF_ATTRIBUTES];
	
protected:
	GLuint program;
};

/*********************************************************/
/*                     CHILDREN                          */
/*********************************************************/

class Generic_Shader : public Shader
{
public:
	static Generic_Shader* instance;
	static Generic_Shader* _GetInstance();
	~Generic_Shader();

	GLuint view_Location, projection_Location, cameraPosition_Location;
	GLuint numberOfLights_Location, opacity_Location;
	GLuint transform_Location, rotate_Location;
	GLuint ambient_Location, lightColor_Directional_Location, lightDirection_Directional_Location;
	GLuint lightColor_Point_Location, lightPosition_Point_Location;
	GLuint lightColor_Spot_Location, lightPosition_Spot_Location, lightDirection_Spot_Location, lightAngle_Spot_Location;
	GLuint isEffectedByLight_Location, material_Location;
private:
	Generic_Shader();
};

class GUI_Shader : public Shader
{
public:
	static GUI_Shader* instance;
	static GUI_Shader* _GetInstance();
	~GUI_Shader();

	GLuint transform_Location, drawPercent_Location, opacity_Location;
private:
	GUI_Shader();
};

class Rainbow_GUI_Shader : public Shader
{
public:
	static Rainbow_GUI_Shader* instance;
	static Rainbow_GUI_Shader* _GetInstance();
	~Rainbow_GUI_Shader();

	GLuint transform_Location, drawPercent_Location;
private:
	Rainbow_GUI_Shader();
};

class Sun_Shader : public Shader
{
public:
	static Sun_Shader* instance;
	static Sun_Shader* _GetInstance();
	~Sun_Shader();

	GLuint view_Location, projection_Location, cameraPosition_Location;
	GLuint transform_Location, lightIntensity_Location, Offset_Location;
	GLuint rotate_Location;
	GLuint TextureScale_Location, CoreColor_Location, RippleColor_Location;

	Vec3 camPosition;
private:
	Sun_Shader();
};

class Atmosphere_Shader : public Shader
{
public:
	static Atmosphere_Shader* instance;
	static Atmosphere_Shader* _GetInstance();
	~Atmosphere_Shader();

	GLuint view_Location, projection_Location;
	GLuint transform_Location, lightIntensity_Location, Offset_Location;
	GLuint TextureScale_Location, AtmosphereColor_Location;
private:
	Atmosphere_Shader();
};

class Glow_Shader : public Shader
{
public:
	static Glow_Shader* instance;
	static Glow_Shader* _GetInstance();
	~Glow_Shader();

	GLuint view_Location, projection_Location, cameraPosition_Location;
	GLuint transform_Location, Offset_Location;
	GLuint rotate_Location;
	GLuint TextureScale_Location, GlowColor_Location;
private:
	Glow_Shader();
};
