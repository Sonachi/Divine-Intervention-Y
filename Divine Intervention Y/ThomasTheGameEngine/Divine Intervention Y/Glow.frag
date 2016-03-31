#version 430 core

out vec4 fColor;
in vec2 texCoord;
in vec3 worldPosition;

uniform sampler3D texture0;
uniform vec3 Offset;
uniform float TextureScale;

uniform vec4 GlowColor;

uniform vec3 CamPosition;
in vec4 vNormal; //vertex normal

void main() 
{
	vec4 noiseVec = texture(texture0, (worldPosition * TextureScale) + Offset);	
	float intensity = abs(noiseVec[0] - 0.25) + abs(noiseVec[1] - 0.125) + abs(noiseVec[2] - 0.06) + abs(noiseVec[3] - 0.001);
	intensity = clamp(intensity * 4.0, 0.0, 1.0);

	fColor = GlowColor * pow(clamp(dot(normalize(CamPosition - worldPosition), normalize(vNormal.xyz)), 0.0, 1.0), 1.0/3.0);
	fColor.w = pow(clamp(dot(normalize(CamPosition - worldPosition), normalize(vNormal.xyz)) - 0.1 , 0.0, 0.6), 4) * 7;
}