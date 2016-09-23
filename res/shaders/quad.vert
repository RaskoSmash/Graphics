#version 430

layout(location = 0) in vec4 position;
layout(location = 3) in vec2 texcoord;

out vec2 vUV;

layout(location = 1) uniform mat4 model;

void main()
{
	vUV = texcoord;
	gl_Position = model * position;
}