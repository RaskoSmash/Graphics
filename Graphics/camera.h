#pragma once
#define GLM_SWIZZLE
#include "GLM\glm.hpp"
#include "GLM\ext.hpp"
class FlyCamera
{
	glm::vec3 position, direction;

	//near has to be > 0
	float aspect, fov, near, far;

	glm::mat4 transform;

	//yaw rotation along Y, pitch rotation along X
	float pitch, yaw, roll;
public:
	FlyCamera(float  a_aspect  = 16.f/9.f, float a_fov = 45.f, float near = 1.f,float far = 100.f)
		: aspect(a_aspect), fov(a_fov), near(near), far(far) , pitch(0), roll(0), yaw(0), direction(0,0,1){}

	void update(const class Input &in, const class Timer &t, const float speed);

	void jumpTo(const glm::vec3 &jump)
	{
		position = jump;
	}
	void lookAt(const glm::vec3 &target);

	glm::mat4 getView() const
	{
		return glm::lookAt(position, position + direction, glm::vec3(0, 1, 0));
	}
	glm::mat4 getProjection() const
	{
		return glm::perspective(fov, aspect, near, far);
	}
};