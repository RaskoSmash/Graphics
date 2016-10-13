#version 430

in vec2 vUV;

layout(location = 0) uniform sampler2D glowMap;
layout(location = 1) uniform sampler2D colorMap;

out vec4 outColor;

vec4 glow(in vec4 color, in sampler2D colorTexture, in vec2 UV, in int offset);

void main()
{
	vec4 glowSpot = texture(glowMap, vUV);
	vec4 color = texture(colorMap, vUV);
	if(glowSpot.r == 1)
	{
		outColor = glow(color, colorMap, vUV, 25);
	}
	else
	{
		outColor = color;
	}
}

vec4 glow(in vec4 color, in sampler2D colorTexture, in vec2 UV, in int offset)
{
	vec4 sum = vec4(0,0,0,0);
	int i, j;

		for(i = -offset; i <= offset; ++i)
		{
			for(j = -offset; j <= offset; ++j)
			{
				vec2 newOffset = vec2(i,j) * 0.001;
				sum += texture(colorTexture, UV + newOffset);
			} 
		}

	int sumDiv = (offset * 2) + 1;
	return (sum / (sumDiv * sumDiv)) + color;
}