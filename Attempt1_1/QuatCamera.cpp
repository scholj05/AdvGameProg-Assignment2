#include "QuatCamera.h"
#include <glm/gtc/matrix_transform.hpp>



QuatCamera::QuatCamera(float x, float y, float z)
{
	cameraPos = glm::vec3(x, y, z);
	glm::vec3 cameraPosAsRad = glm::vec3(DegToRad(cameraPos.x), DegToRad(cameraPos.y), DegToRad(cameraPos.z));
	cameraQuat = glm::quat(cameraPosAsRad);
}


QuatCamera::~QuatCamera()
{
}

void QuatCamera::Pitch(float angle)
{
	pitch += angle;
}

void QuatCamera::Yaw(float angle)
{
	yaw += angle;
}

void QuatCamera::Roll(float angle)
{
	roll += angle;
}

void QuatCamera::Advance(float distance)
{
	cameraPos.z += distance;
}

void QuatCamera::Ascend(float distance)
{
	cameraPos.y += distance;
}

void QuatCamera::Strafe(float distance)
{
	cameraPos.x += distance;
}

glm::quat QuatCamera::Update()
{
	glm::quat key_quat = glm::quat
	(glm::vec3(
		pitch * tickTime.getElapsedTime().asMilliseconds(), 
		yaw * tickTime.getElapsedTime().asMilliseconds(), 
		roll * tickTime.getElapsedTime().asMilliseconds()
	));
	//reset values
	pitch = yaw = roll = 0;
	tickTime.restart();

	//order matters,update camera_quat
	cameraQuat = key_quat * cameraQuat;
	cameraQuat = glm::normalize(cameraQuat);
	glm::mat4 rotate = glm::mat4_cast(cameraQuat);

	glm::mat4 translate = glm::mat4(1.0f);
	translate = glm::translate(translate, -cameraPos);

	return (rotate * translate);

	
}
