#version 430 core

out vec4 fColor;
in vec2 texCoord;
in vec3 worldPosition;

uniform float lightIntensity; 
uniform sampler3D texture0;
uniform vec3 Offset;

uniform float TextureScale;
uniform vec4 AtmosphereColor;

void main() 
{
	vec4 noiseVec = texture(texture0, (worldPosition * TextureScale) + Offset);
	
	float intensity = abs(noiseVec[0] - 0.25) + abs(noiseVec[1] - 0.125) + abs(noiseVec[2] - 0.06) + abs(noiseVec[3] - 0.001);
	intensity = clamp(intensity * 4.0, 0.0, 1.0);

	fColor = AtmosphereColor * lightIntensity;
	fColor.w = pow(intensity, 3);
}