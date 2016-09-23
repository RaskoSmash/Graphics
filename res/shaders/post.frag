#version 430

out vec4 outColor;

in vec2 vUV;

layout(location = 0) uniform sampler2D map;

vec4 sobel(in sampler2D vMap, in vec2 UV);
vec4 crossblur(in sampler2D vMap, in vec2 UV);

void main()
{
	outColor = crossblur(map, vUV) + sobel(map,vUV);
}

vec4 crossblur(in sampler2D vMap, in vec2 UV)
{
	vec2 sDim = textureSize(vMap,0).xy;
	vec4 retval = vec4(0,0,0,0);
	for(int i = -9; i <= 9; ++i)
	{
		retval += texture(vMap, UV + vec2( i, 0)/sDim);
		retval += texture(vMap, UV + vec2( 0, i)/sDim);
	}
	return retval / 36;
}

vec4 sobel(in sampler2D vMap, in vec2 UV)
{
	vec2 sDim = textureSize(vMap,0).xy;
	vec4 yColor = 
	texture(vMap,UV + vec2( 1, -1)/sDim)   +
	texture(vMap,UV + vec2( 0, -1)/sDim)*2 +
	texture(vMap,UV + vec2(-1, -1)/sDim)   -
	texture(vMap,UV + vec2( 1,  1)/sDim)    -
	texture(vMap,UV + vec2( 0,  1)/sDim)*2  -
	texture(vMap,UV + vec2(-1,  1)/sDim);

	vec4 xColor = 
	texture(vMap,UV + vec2(-1, -1)/sDim)   +
	texture(vMap,UV + vec2(-1,  0)/sDim)*2 +
	texture(vMap,UV + vec2(-1,  1)/sDim)   -
	texture(vMap,UV + vec2( 1, -1)/sDim)   -
	texture(vMap,UV + vec2( 1,  0)/sDim)*2 -
	texture(vMap,UV + vec2( 1,  1)/sDim);

	return sqrt(yColor * yColor + xColor * xColor);
}