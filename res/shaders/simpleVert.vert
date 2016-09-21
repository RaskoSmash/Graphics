#version 430

layout(location = 0) in vec4 position;
layout(location = 3) in vec2 texCoord;

out vec4 vPosition;
out vec2 vUV;


layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 proj;


layout(location = 3) uniform sampler2D diffuseMap;
layout(location = 4) uniform sampler2D normalMap;
layout(location = 5) uniform sampler2D specularMap;

void main()
{
	vPosition = position;
	vUV = texCoord;
	gl_Position = proj * view * model * position;
}