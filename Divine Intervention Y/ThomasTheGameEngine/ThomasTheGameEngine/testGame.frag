#version 430 core

layout(location = 1) in float colour;

out vec4 fColor;

void main()
{	
	if (colour > 0)
	{
		float avgV = abs(colour);

		float MEDIUM_LOW_SPEED = 250, 
			MEDIUM_HIGH_SPEED = 500,
            HIGH_SPEED = 750,
            MAX_SPEED = 1000;

		float r = 0, g = 0, b = 0;

		avgV = avgV < 0 ? 0 : avgV;
		avgV = avgV > MAX_SPEED ? MAX_SPEED : avgV;

		//Low speed (blue @ 255; Green++)
		if (avgV <= MEDIUM_LOW_SPEED) // > 0
		{
			r = 21;
			b = 255;
			g = 21 + (avgV / (MEDIUM_LOW_SPEED - 0)) * 234;
		}
		//Medium-Low speed (Green @ 255; Blue--)
		else if (avgV <= MEDIUM_HIGH_SPEED)
		{
			r = 21;
			b = 255 - ((avgV - MEDIUM_LOW_SPEED) / (MEDIUM_HIGH_SPEED - MEDIUM_LOW_SPEED)) * 234;
			g = 255;
		}
		//Medium-High speed (Green @ 255; Red++)
		else if (avgV <= HIGH_SPEED)
		{
			r = 21 + ((avgV - MEDIUM_HIGH_SPEED) / (HIGH_SPEED - MEDIUM_HIGH_SPEED)) * 234;
			b = 21;
			g = 255;
		}
		//High speed (Red @ 255; Green--)
		else if (avgV <= MAX_SPEED)
		{
			r = 255;
			b = 21;
			g = 255 - ((avgV - HIGH_SPEED) / (MAX_SPEED - HIGH_SPEED)) * 234;
		}

		//r = MathHelper.Clamp(r, 21, 255);
		//b = MathHelper.Clamp(b, 21, 255);
		//g = MathHelper.Clamp(g, 21, 255);

		fColor = vec4(r/255.0, g/255.0, b/255.0, 1.0);
	}
	else
	{
	fColor = vec4(0.0, 0.0, 0.0, 1.0);
	}

}