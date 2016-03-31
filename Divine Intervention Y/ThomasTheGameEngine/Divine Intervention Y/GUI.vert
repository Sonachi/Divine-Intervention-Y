#version 430 core

layout(location = 0) in vec4 vPosition;
in vec2 vTexCoord;
out vec2 texCoord;

uniform mat4 Transform; //model

void main()
{
	gl_Position = (Transform * vPosition);

	texCoord = vTexCoord;
}