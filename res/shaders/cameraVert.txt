#version 430

// location is based on the attributes
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out vec4 vColor;
out vec4 vPosition;

// location is based on the glUniform calls
layout(location = 0) uniform mat4 Projection;	
layout(location = 1) uniform mat4 View;		
layout(location = 2) uniform mat4 Model;	


void main()
{
	vColor = color;
	vPosition = position; // Model Space

	gl_Position = Projection * View * Model * position;
}