//Author:	Mathieu Violette
//Date:		10/12/2015

#ifndef _OPENGL_UTILITIES_
#define _OPENGL_UTILITIES_

#include <iostream>
#include <glew.h>
#include "LoadShaders.h"

#define GLU OpenGLUtilities

/*
	A utility class to simplify the use of complex openGl methods.
*/

class OpenGLUtilities
{
public:

	static GLint CreateShaders(char* vertexShader, char* fragmentShader)
	{
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, vertexShader },
			{ GL_FRAGMENT_SHADER, fragmentShader },
			{ GL_NONE, NULL }
		};

		GLuint program = LoadShaders(shaders);
		//glUseProgram(program);

		return program;
	}

	static void OutputOpenGLVersion()
	{
		std::cout << glGetString(GL_VERSION) << std::endl;
	}
};

#endif