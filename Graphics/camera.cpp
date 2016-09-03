#include "input.h"
#include "timer.h"
#include "camera.h"

void FlyCamera::update(const Input &in, const Timer &t, const float speed)
{
	glm::vec3 moveDir = glm::vec3(0,0,0);
	glm::vec3 right = glm::cross(direction, glm::vec3(0, 1, 0));
	glm::vec3 up = glm::cross(direction, right);
	
	yaw += in.getMouseAxisHorizontal() * t.getDeltaTime();// *150;
	pitch += in.getMouseAxisVertical() * t.getDeltaTime();// *150;
	
	direction = glm::vec3(glm::rotate(yaw, glm::vec3(0, 1, 0))[2]);
	right = glm::cross(direction, glm::vec3(0, 1, 0));
	direction = glm::mat3(glm::rotate(pitch, right)) * direction;
	
	direction.x = sin(yaw);
	direction.y = -(sinf(yaw) * cosf(pitch));
	direction.z = -(cosf(yaw) * sinf(pitch));
	
	if (in.getKeyState('W') == Input::DOWN) { moveDir += direction; }
	if (in.getKeyState('S') == Input::DOWN) { moveDir -= direction; }
	if (in.getKeyState('A') == Input::DOWN) { moveDir -= right; }
	if (in.getKeyState('D') == Input::DOWN) { moveDir += right; }
	if (in.getKeyState('E') == Input::DOWN) { moveDir -= up; }
	if (in.getKeyState('Q') == Input::DOWN) { moveDir += up; }
	
	if (glm::length(moveDir) >= .7f)
	{
		moveDir = glm::normalize(moveDir);
		position += moveDir * t.getDeltaTime() * speed;
	}
}

void FlyCamera::lookAt(const glm::vec3 & target)
{
	direction = glm::normalize(target - position);
}

/*
void FlyCamera::update(const Input &in, const Timer &t, const float speed)
{

//glm::vec3 moveDir = glm::vec3(0,0,0);
//glm::vec3 right = glm::cross(direction, glm::vec3(0, 1, 0));
//glm::vec3 up = glm::cross(direction, right);
//
//yaw += in.getMouseAxisHorizontal() * t.getDeltaTime();// *150;
//pitch += in.getMouseAxisVertical() * t.getDeltaTime();// *150;
//
//direction = glm::vec3(glm::rotate(yaw, glm::vec3(0, 1, 0))[2]);
//right = glm::cross(direction, glm::vec3(0, 1, 0));
//direction = glm::mat3(glm::rotate(pitch, right)) * direction;
//
//direction.x = sin(yaw);
//direction.y = -(sinf(yaw) * cosf(pitch));
//direction.z = -(cosf(yaw) * sinf(pitch));
//
//if (in.getKeyState('W') == Input::DOWN) { moveDir += direction; }
//if (in.getKeyState('S') == Input::DOWN) { moveDir -= direction; }
//if (in.getKeyState('A') == Input::DOWN) { moveDir -= right; }
//if (in.getKeyState('D') == Input::DOWN) { moveDir += right; }
//if (in.getKeyState('E') == Input::DOWN) { moveDir -= up; }
//if (in.getKeyState('Q') == Input::DOWN) { moveDir += up; }
//
//if (glm::length(moveDir) >= .7f)
//{
//moveDir = glm::normalize(moveDir);
//position += moveDir * t.getDeltaTime() * speed;
//}


	yaw -= in.getMouseAxisHorizontal() / 200.f;
	pitch -= in.getMouseAxisVertical() / 200.f;

	pitch = glm::clamp<float>(pitch, -glm::pi<float>() / 2 + 0.0001f,
		glm::pi<float>() / 2 - 0.0001f);

	glm::mat4 &view = transform;
	view = glm::rotate(yaw, glm::vec3(0, 1, 0));
	view = glm::rotate(pitch, glm::vec3(view[0])) * view;
	view = glm::rotate(roll, glm::vec3(view[2])) * view;

	view = glm::eulerAngleYXZ(yaw, pitch, roll);
	view = glm::translate(-position) * view;

	glm::vec3 moveDir = glm::vec3(0, 0, 0);
	if (in.getKeyState('W') == Input::DOWN) moveDir += glm::vec3(view[2]);
	if (in.getKeyState('S') == Input::DOWN) moveDir -= glm::vec3(view[2]);
	if (in.getKeyState('A') == Input::DOWN) moveDir += glm::vec3(view[0]);
	if (in.getKeyState('D') == Input::DOWN) moveDir -= glm::vec3(view[0]);
	if (in.getKeyState('E') == Input::DOWN) moveDir -= glm::vec3(view[1]);
	if (in.getKeyState('Q') == Input::DOWN) moveDir += glm::vec3(view[1]);

	if (glm::length(moveDir) >= .7f)
	{
		moveDir = glm::normalize(moveDir);
		position += moveDir * t.getDeltaTime() * speed;
	}
}*/