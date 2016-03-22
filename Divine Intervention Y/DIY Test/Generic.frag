#version 430 core

out vec4 fColor;
uniform sampler2D texture;
in vec2 texCoord;

in vec4 vNormal; //vertex normal
in vec4 worldPosition;

uniform vec3 CamPosition;
uniform vec4 AmbientColor;
uniform vec3 NumberOfLights;

uniform vec4[4] LightColor_Directional;
uniform vec4[4] LightDirection_Directional;

uniform vec4[10] LightColor_Point;
uniform vec4[10] LightPosition_Point;

uniform vec4[10] LightColor_Spot;
uniform vec4[10] LightPosition_Spot;
uniform vec4[10] LightDirection_Spot;
uniform float[10] LightAngle_Spot;

uniform vec3 IsEffectedByLight;
uniform vec3 Material;

uniform float Opacity;

void main()
{
	//if Alpha == 0, don't draw
	if (texture2D(texture, texCoord).w == 0)
		discard;

	/* AMBIENT LIGHTING */
	vec4 ambientLight = (Material.x * AmbientColor);

	if (length(IsEffectedByLight) == 0)
		ambientLight = vec4(1, 1, 1, 1);

	/* DIFFUSE LIGHTING */
	vec4 diffuse = vec4(0, 0, 0, 0);

	/* SPECULAR LIGHTING */
	vec4 specular = vec4(0, 0, 0, 0);


	vec4 toCameraVector = normalize(vec4(CamPosition, 0.0) - worldPosition);

	if (IsEffectedByLight.x > 0)
	{
		int directionalLength = int(NumberOfLights.x);
		if (NumberOfLights.x > LightDirection_Directional.length())
			directionalLength = int(LightDirection_Directional.length());

		/* DIRECTIONAL */
		for (int i = 0; i < directionalLength; i++)
		{
			float brightness = dot(vNormal, -LightDirection_Directional[i]) / ( length(LightDirection_Directional[i]) * length(vNormal) );
			brightness = clamp(brightness, 0, 1);
			vec4 directionalDiffuse = brightness * LightColor_Directional[i] * Material.y;


			diffuse += directionalDiffuse;
		}
	}

	if (IsEffectedByLight.y > 0)
	{
		int pointLength = int(NumberOfLights.y);
		if (NumberOfLights.y > LightPosition_Point.length())
			pointLength = int(LightPosition_Point.length());

		/* POINT */
		for (int i = 0; i < pointLength; i++)
		{
			vec4 distanceToPoint =  worldPosition - LightPosition_Point[i];
			vec4 toPointLight =  normalize(LightPosition_Point[i] - worldPosition);

			float lambertian = max(dot(toPointLight, vNormal), 0.0);
			//float lambertian = dot(toPointLight, vNormal);

			//if(lambertian > -0.3)
			{
				{
					float brightness = dot(vNormal, toPointLight);
					brightness = clamp(brightness, 0.0f, 1.0f) / ( length(toPointLight) * length(vNormal) * pow(length(distanceToPoint), 2) );
					vec4 pointDiffuse = brightness * LightColor_Point[i] * Material.y;
					diffuse += pointDiffuse;
				}

				vec3 viewDir = normalize(toCameraVector.xyz);
				vec3 halfDir = normalize(toPointLight.xyz + viewDir);
				float specAngle = max(dot(halfDir, vNormal.xyz), 0.0);
				//specular += pow(specAngle, 16.0) * Material.z * LightColor_Point[i];
				specular += pow(specAngle, 15.5) * Material.z * LightColor_Point[i];
			}
		}
	}

	if (IsEffectedByLight.z > 0)
	{
		int spotLength = int(NumberOfLights.z);
		if (NumberOfLights.z > LightPosition_Spot.length())
			spotLength = int(LightPosition_Spot.length());

		/* SPOT */
		for (int i = 0; i < spotLength; i++)
		{				
			vec4 distanceToFrag =  worldPosition - LightPosition_Spot[i];
			vec4 toSpotLight =  normalize(LightPosition_Spot[i] - worldPosition);

			{
				//Check if within cone
				if ( dot(distanceToFrag, LightDirection_Spot[i]) / (length(distanceToFrag) * length(LightDirection_Spot[i])) > cos(LightAngle_Spot[i]/2.0f) )
				{
					float lambertian = max(dot(toSpotLight, vNormal), 0.0);

					if(lambertian > 0.0)
					{
						float distanceOfAngle = (LightAngle_Spot[i]/2.0f) - acos(dot(distanceToFrag, LightDirection_Spot[i]) / (length(distanceToFrag) * length(LightDirection_Spot[i])));

						float brightness = dot(vNormal, toSpotLight) / (pow(length(distanceToFrag),2) * length(toSpotLight) * length(vNormal)) * (distanceOfAngle / (LightAngle_Spot[i]/2.0f));
						brightness = clamp(brightness, 0, 1);
						vec4 spotDiffuse = brightness * LightColor_Spot[i] * Material.y;
						diffuse += lambertian * spotDiffuse;

							
						// this is blinn phong
						vec3 viewDir = normalize(toCameraVector.xyz);
						vec3 halfDir = normalize(toSpotLight.xyz + viewDir);
						float specAngle = max(dot(halfDir, vNormal.xyz), 0.0);
						specular += (pow(specAngle, 4.0) / (pow(length(distanceToFrag), 2) )) * Material.z * (distanceOfAngle / (LightAngle_Spot[i]/2.0f)) * LightColor_Spot[i];
					}
				}
			}
		}
	}

	diffuse.x = diffuse.x > 1 ? 1 : diffuse.x;
	diffuse.y = diffuse.y > 1 ? 1 : diffuse.y;
	diffuse.z = diffuse.z > 1 ? 1 : diffuse.z;
	diffuse.w = diffuse.w > 1 ? 1 : diffuse.w;

	specular.x = specular.x > 1 ? 1 : specular.x;
	specular.y = specular.y > 1 ? 1 : specular.y;
	specular.z = specular.z > 1 ? 1 : specular.z;
	specular.w = specular.w > 0 ? 0 : specular.w;

	fColor = specular + texture2D(texture, texCoord) * (ambientLight + diffuse);
	fColor.w = fColor.w * Opacity;
}