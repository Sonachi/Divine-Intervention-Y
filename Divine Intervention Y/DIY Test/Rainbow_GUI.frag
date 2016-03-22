#version 430 core

out vec4 fColor;
uniform float drawPercent;
uniform sampler2D texture;
in vec2 texCoord;

void main()
{
	fColor = texture2D(texture, texCoord);

	if (texCoord.x > drawPercent)
		discard;

	//if Alpha == 0, don't draw
	if (fColor.w == 0)
		discard;

	float colour = abs(texCoord.x);		
	float MEDIUM_LOW_SPEED = 0.25f, 
			MEDIUM_HIGH_SPEED = 0.5f,
			HIGH_SPEED = 0.75f,
			MAX_SPEED = 1.0f;
    
	if (colour > 0)
	{
		float r = 0, g = 0, b = 0;

		colour = colour < 0 ? 0 : colour;
		colour = colour > MAX_SPEED ? MAX_SPEED : colour;

		//Low speed (blue @ 255; Green++)
		if (colour <= MEDIUM_LOW_SPEED) // > 0
		{
			r = 21;
			b = 255;
			g = 21 + (colour / (MEDIUM_LOW_SPEED - 0)) * 234;
		}
		//Medium-Low speed (Green @ 255; Blue--)
		else if (colour <= MEDIUM_HIGH_SPEED)
		{
			r = 21;
			b = 255 - ((colour - MEDIUM_LOW_SPEED) / (MEDIUM_HIGH_SPEED - MEDIUM_LOW_SPEED)) * 234;
			g = 255;
		}
		//Medium-High speed (Green @ 255; Red++)
		else if (colour <= HIGH_SPEED)
		{
			r = 21 + ((colour - MEDIUM_HIGH_SPEED) / (HIGH_SPEED - MEDIUM_HIGH_SPEED)) * 234;
			b = 21;
			g = 255;
		}
		//High speed (Red @ 255; Green--)
		else if (colour <= MAX_SPEED)
		{
			r = 255;
			b = 21;
			g = 255 - ((colour - HIGH_SPEED) / (MAX_SPEED - HIGH_SPEED)) * 234;
		}

		fColor = vec4(r/255.0, g/255.0, b/255.0, fColor.w);
	}
}