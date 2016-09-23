#version 430

in vec2 vUV;

layout(location = 0) uniform sampler2D colorMap;

out vec4 outColor;

void main()
{
	outColor = texture(colorMap, vUV);
}
