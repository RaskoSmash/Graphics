#pragma once

struct Vertex
{
	float position[4];
	float color[4];
	//float normal[4];
	//float tangent[4];
	//float binormal[4];
	//float texCord[4];
	enum
	{
		POSITION = 0,
		COLOR = 16
	};
};