#version 430 core

layout(location = 0) in vec4 vPosition;
layout(location = 2) in vec4 Normal;

in vec2 vTexCoord;

uniform vec4 faceNormal;
uniform float uiDraw;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 Transform; //model
uniform mat4 Rotation; //for normals

out vec2 texCoord;
out vec4 vNormal;
out vec4 fNormal;
out vec4 fPosition;
out float fUiDraw;

void main()
{
	if (uiDraw > 0)
	{
		gl_Position = (Transform * vPosition);
	}
	else
	{
		//				Projection	View	Model
		gl_Position = projection * view * (Transform * vPosition);
	}

	//World Position
	fPosition = Transform * vPosition;

	/* Textures -- FLIP THEM TO FIX WIERDNESS */
	texCoord = vTexCoord * vec2(1.0, -1.0);

	//Need to rotate normals
	vNormal =  normalize(Rotation * Normal);
	fNormal = normalize(Rotation * faceNormal);

	fUiDraw = uiDraw;
}