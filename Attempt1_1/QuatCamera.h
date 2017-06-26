#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "glm\gtc\quaternion.hpp"
#include "glm\gtx\quaternion.hpp"
#include "SFML/System.hpp"

#define PI 3.14159265359;
class QuatCamera
{
public:
	QuatCamera();

	QuatCamera(float positionX, float positionY, float positionZ, float rotationX, float rotationY, float rotationZ);

	void Pitch(float pitchRadians);

	void Yaw(float yawRadians);

	void Roll(float rollRadians);

	void Turn(float turnRadians);

	void Rotate(float angleRadians, const glm::vec3 & axis);

	void Rotate(const glm::quat & rotation);

	glm::vec3 GetForward() const;

	glm::vec3 GetLeft() const;

	glm::vec3 GetUp() const;

	void MoveForward(float movement);

	void MoveLeft(float movement);

	void MoveUp(float movement);

	glm::mat4 GetViewMatrix() const;

	float * GetViewMatrixAsArray() const;

	float * GetInverseViewMatrix() const;





private:

	glm::vec3 mPosition;
	glm::quat mOrientation;

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

