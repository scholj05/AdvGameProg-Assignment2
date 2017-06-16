#pragma once
#include <glm\glm.hpp>
#include "SFML\System.hpp"
class Camera
{
public:
	Camera(float x, float y, float z, float pitch, float yaw, float roll);
	~Camera();

	void Camera::Pitch(float angle);
	void Camera::Yaw(float angle);
	void Camera::Roll(float angle);

	void Camera::Advance(float distance);

	void Ascend(float distance);

	void Strafe(float distance);

	glm::mat3 Camera::Place();

private:
	const float piover180 = 0.0174532925f;

	/// keep track of total rotation around each axis
	float xRotation;	// pitch angle
	float yRotation;	// yaw angle
	float zRotation;	// roll angle

	/// Vector for each axis of the camera
	glm::vec3 xVector;
	glm::vec3 yVector;
	glm::vec3 zVector;

	/// Vector for the global position of the camera in the game world
	glm::vec3 position;

	sf::Clock tickTime;
};

