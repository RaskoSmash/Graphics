#pragma once
#include "GLM\glm.hpp"
struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec4 normal;
	//float tangent[4];
	//float binormal[4];
	glm::vec2 texcoord;
	enum
	{
		POSITION = 0,
		COLOR = 16,
		NORMAL = 32,
		TEXCOORD = 48
	};
};