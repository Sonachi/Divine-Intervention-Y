#version 430 core

out vec4 fColor;
in vec2 texCoord;
in vec3  worldPosition;

uniform float lightIntensity; 
uniform sampler3D texture0;
uniform vec3 Offset;

void main() 
{

	vec4 noiseVec = texture(texture0, (worldPosition * 0.025) + Offset);
    vec4 color2 = vec4(1.5, 1.5, 0.0,0.0);
	vec4 color1 = vec4(8.0, 0.4, 0.0,0.0);

	//float intensity = abs(noiseVec[0] - 0.25) + abs(noiseVec[1] - 0.125) + abs(noiseVec[2] - 0.0625) + abs(noiseVec[3] - 0.03125);	
	float intensity = abs(noiseVec[0] - 0.25) + abs(noiseVec[1] - 0.125) + abs(noiseVec[2] - 0.06) + abs(noiseVec[3] - 0.001);
	
	intensity = clamp(intensity * 4.0, 0.0, 1.0);
	
	fColor = mix(color1, color2, intensity) * lightIntensity;

	fColor.w = 1;

}