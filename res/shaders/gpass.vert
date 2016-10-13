#version 430

layout(location = 0) in vec4 position;
layout(location = 2) in vec4 normal;
layout(location = 3) in vec2 texcoord;

out vec2 vUV;
out vec3 vNormal;
out vec3 vPosition;

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 proj;
layout(location = 6) uniform bool isHeightMap;
layout(location = 7) uniform sampler2D heightMap;
layout(location = 8) uniform float mountScale;		//changing the geometry in real-time

void main()
{
	vNormal   = (view * model * normal).xyz;
	vPosition = (view * model * position).xyz;

	vUV = texcoord;

	if(isHeightMap)
	{
		vec4 pos = position;
		pos.y = texture(heightMap, vUV).r * mountScale; // used to change the height based on the r value
		gl_Position = proj * view * model * pos;
	}
	else gl_Position = proj * view * model * position;
}