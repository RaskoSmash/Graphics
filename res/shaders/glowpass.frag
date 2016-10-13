#version 430

in vec2 vUV;

layout(location = 0) uniform sampler2D colorMap;
layout(location = 1) uniform float rThres;
layout(location = 2) uniform float gThres;
layout(location = 3) uniform float bThres;

layout(location = 0) out vec4 glowMap;

void main()
{
	vec4 color = texture(colorMap, vUV);
	vec4 vColor = (color * color);
	if(vColor.r > rThres || vColor.g > gThres || vColor.b > bThres)
	{
		glowMap = vec4(1,1,1,1);
	}
	else
	{
		glowMap = vec4(0,0,0,0);
	}
	//glowMap = vec4(1,0,1,1);
}