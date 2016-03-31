#version 430 core

layout(location = 0) in vec4 vPosition;
layout(location = 2) in vec4 Normal;
in vec2 vTexCoord;
out vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 Transform; //model
uniform mat4 Rotation; //for normals

out vec3 worldPosition;
out vec4 vNormal;

void main()
{
	//				Projection	View	Model
	gl_Position = projection * view * (Transform * vPosition);

	vec4 vertex = Transform * vPosition;
	worldPosition = vertex.xyz;

	texCoord = vTexCoord;

	//Need to rotate normals
	vNormal =  normalize(Rotation * Normal);
}