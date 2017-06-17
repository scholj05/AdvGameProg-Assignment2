#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\quaternion.hpp"
#include "glm\gtx\quaternion.hpp"
#include "SFML/System.hpp"

#define PI 3.14159265359;
class QuatCamera
{
public:

	QuatCamera(float x, float y, float z);

	~QuatCamera();

	void QuatCamera::Pitch(float angle);

	void QuatCamera::Yaw(float angle);

	void QuatCamera::Roll(float angle);

	void QuatCamera::Advance(float distance);

	void QuatCamera::Ascend(float distance);

	void QuatCamera::Strafe(float distance);

	glm::quat QuatCamera::Update();

private:

	glm::quat cameraQuat;

	float yaw, pitch, roll;

	glm::vec3 cameraPos;

	sf::Clock tickTime;

	inline float QuatCamera::DegToRad(float degrees)
	{
		float pi = PI;
		return degrees * (pi / 180);
	}

	inline float QuatCamera::RadToDeg(float rads)
	{
		float pi = PI;
		return rads * (180 / pi);
	}
};

