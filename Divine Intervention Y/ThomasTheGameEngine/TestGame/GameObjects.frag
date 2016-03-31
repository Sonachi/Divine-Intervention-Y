#version 430 core

out vec4 fColor;
uniform sampler2D texture;

in vec2 texCoord;
in vec4 vNormal; //vertex normal
in vec4 fNormal; //face normal
in vec4 fPosition;
in float fUiDraw;

uniform vec3 CamPosition;

uniform vec4	AmbientColor;
uniform vec4[1] LightColor_Directional;
uniform vec4[1] LightDirection_Directional;
uniform vec4[2] LightColor_Point;
uniform vec4[2] LightPosition_Point;
uniform vec4[2]	LightColor_Spot;
uniform vec4[2] LightPosition_Spot;
uniform vec4[2] LightDirection_Spot;
uniform float[2] LightAngle_Spot;

uniform vec3 IsEffectedByLight;
uniform vec3 Material;

void main()
{

	//Normals Debug View
	bool normalsDebug = false;
	if (normalsDebug)
	{
		if (fUiDraw > 0)
			discard;
		fColor = 0.5 * vNormal + 0.5;
		//vec4 reflection = reflect(normalize(LightDirection_Spot[0]), vNormal);
		//fColor = reflection * max(dot(normalize(vec4(CamPosition, 0.0) - fPosition), reflection), 0.0);
	}
	else if (fUiDraw > 0)
		fColor = texture2D(texture, texCoord);
	else
	{
		/* AMBIENT LIGHTING */
		vec4 ambientLight = (Material.x * AmbientColor);

		if (length(IsEffectedByLight) == 0)
			ambientLight = vec4(1, 1, 1, 1);

		/* DIFFUSE LIGHTING */
		vec4 diffuse = vec4(0, 0, 0, 0);

		/* SPECULAR LIGHTING */
		vec4 specular = vec4(0, 0, 0, 0);


		vec4 toCameraVector = normalize(vec4(CamPosition, 0.0) - fPosition);

		if (IsEffectedByLight.x > 0)
		{
			/* DIRECTIONAL */
			for (int i = 0; i < LightDirection_Directional.length(); i++)
			{
				//vec4 directionalDiffuse = (LightColor_Directional[i] * Material.y * dot(LightDirection_Directional[i], vNormal));

				float brightness = dot(vNormal, -LightDirection_Directional[i]) / ( length(LightDirection_Directional[i]) * length(vNormal) );
				brightness = clamp(brightness, 0, 1);
				vec4 directionalDiffuse = brightness * LightColor_Directional[i] * Material.y;

				/*directionalDiffuse.x = directionalDiffuse.x < 0 ? 0 : directionalDiffuse.x;
				directionalDiffuse.y = directionalDiffuse.y < 0 ? 0 : directionalDiffuse.y;
				directionalDiffuse.z = directionalDiffuse.z < 0 ? 0 : directionalDiffuse.z;
				directionalDiffuse.w = directionalDiffuse.w < 0 ? 0 : directionalDiffuse.w;*/

				diffuse += directionalDiffuse;
			}
		}

		if (IsEffectedByLight.y > 0)
		{
			/* POINT */
			for (int i = 0; i < LightPosition_Point.length(); i++)
			{
				vec4 distanceToPoint =  fPosition - LightPosition_Point[i];
				vec4 toPointLight =  normalize(LightPosition_Point[i] - fPosition);

				float lambertian = max(dot(toPointLight, vNormal), 0.0);

				if(lambertian > 0.0)
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
					//specular += (pow(specAngle, 3.0) / (pow(length(distanceToPoint), 2) )) * Material.z * LightColor_Point[i];
					specular += pow(specAngle, 16.0) * Material.z * LightColor_Point[i];

					//vec4 lightDirection = normalize(fPosition - LightPosition_Point[i]);
					//if (dot(lightDirection, vNormal) < 0)
					//{
					//	vec4 reflection = normalize(reflect(lightDirection, vNormal));
					//	float sp_brightness =  max(dot(reflection, toCameraVector), 0) * max(dot(lightDirection, vNormal), 0);
					//	vec4 pointSpecular = (pow(sp_brightness, 1) / (length(distanceToPoint) * length(distanceToPoint))) * Material.z * LightColor_Point[i];
					//	specular += pointSpecular;
					//}
				}
			}
		}

		if (IsEffectedByLight.z > 0)
		{
			/* SPOT */
			for (int i = 0; i < LightPosition_Spot.length(); i++)
			{

				//vec3 lightDir = normalize(LightPosition_Spot[i] - fPosition);
				
				vec4 distanceToFrag =  fPosition - LightPosition_Spot[i];
				vec4 toSpotLight =  normalize(LightPosition_Spot[i] - fPosition);

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
							//specular += pow(specAngle, 16.0) * Material.z * LightColor_Spot[i];


							//crappy phong
							////if (dot(LightDirection_Spot[i], fNormal) < 0)
							//{
							//	vec4 reflection =  reflect(normalize(LightDirection_Spot[i]), vNormal);
							//	float sp_brightness = max(dot(toCameraVector, reflection), 0.0) ;
							//	vec4 spotSpecular = (pow(sp_brightness, 1) / (pow(length(distanceToFrag), 2) )) * Material.z * (distanceOfAngle / (LightAngle_Spot[i]/2.0f)) * LightColor_Spot[i];
							//	specular += spotSpecular;
							//}
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

		//vec3 colorGammaCorrected = pow(fColor, vec3(1.0/screenGamma));
	}


	//if Alpha == 0, don't draw
	if (fColor.w == 0)
		discard;

}