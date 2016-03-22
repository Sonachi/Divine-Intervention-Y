#version 430 core

layout(location = 0) in vec4 vPosition;
layout(location = 2) in vec4 Normal;
in vec2 vTexCoord;
out vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 Transform; //model
uniform mat4 Rotation; //for normals

out vec4 vNormal;
out vec4 worldPosition;

void main()
{
	//				Projection	View	Model
	gl_Position = projection * view * (Transform * vPosition);

	//World Position
	worldPosition = Transform * vPosition;

	//Need to rotate normals
	vNormal =  normalize(Rotation * Normal);

	texCoord = vTexCoord;
}