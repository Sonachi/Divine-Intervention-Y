#version 430 core

layout(location = 0) in vec4 vPosition;
in vec2 vTexCoord;
out vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 Transform; //model

out vec3  worldPosition;

void main()
{
	//				Projection	View	Model
	gl_Position = projection * view * (Transform * vPosition);

	vec4 vertex = Transform * vPosition;
	worldPosition = vertex.xyz;

	texCoord = vTexCoord;
}