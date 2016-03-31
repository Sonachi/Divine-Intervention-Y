#version 430 core

layout(location = 0) in vec4 vPosition;

uniform float fColor;
uniform mat4 cameraMatrix;
uniform mat4 Transform;

layout(location = 1) out float vColor;

void main()
{
	gl_Position = cameraMatrix * (Transform * vPosition);
	vColor = fColor;
}