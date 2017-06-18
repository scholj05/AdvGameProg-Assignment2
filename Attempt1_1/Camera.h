#pragma once
#include <glm\glm.hpp>
#include "SFML\System.hpp"

#define piover180 double(0.0174532925f)
class Camera
{
public:
	Camera(float x, float y, float z, float pitch, float yaw, float roll);
	~Camera();

	void Camera::Pitch(float angle, bool applyTime);
	void Camera::Yaw(float angle, bool applyTime);
	void Camera::Roll(float angle, bool applyTime);

	void Camera::Advance(float distance, bool applyTime);

	void Ascend(float distance, bool applyTime);

	void Strafe(float distance, bool applyTime);

	glm::mat3 Camera::Place();

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
	float targetTickTime;
private:

};

