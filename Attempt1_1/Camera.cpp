#include "Camera.h"


/// Set the initial position and direction of the camera
Camera::Camera(float x, float y, float z, float pitch, float yaw, float roll)
{
	this->position.x = 0;
	this->position.y = 0;
	this->position.z = 0;

	this->xRotation = 0.0f;
	this->yRotation = 0.0f;
	this->zRotation = 0.0f;

	this->xVector = glm::vec3(1.0f, 0.0f, 0.0f);
	this->yVector = glm::vec3(0.0f, 1.0f, 0.0f);
	this->zVector = glm::vec3(0.0f, 0.0f, 1.0f);

	/// initial fix to angle before applying adjustments
	Roll(270, false);

	///apply parameter movements via internal methods to the camera
	Strafe(x, false);
	Ascend(y, false);
	Advance(z, false);

	Pitch(pitch, false);
	Yaw(yaw, false);
	Roll(roll, false);
}


Camera::~Camera()
{
}


void Camera::Pitch(float angle, bool applyTime)
{
	if (applyTime)
		angle *= (1000 * (targetTickTime / tickTime.getElapsedTime().asMilliseconds()));
	this->xRotation += angle;

	this->zVector = glm::normalize(
		this->zVector * cosf(angle * piover180) +
		this->xVector * sinf(angle * piover180));

	this->xVector = glm::cross(
		this->zVector, this->yVector);

	this->xVector *= -1;
}


void Camera::Yaw(float angle, bool applyTime)
{
	if (applyTime)
		angle *= (1000 * (targetTickTime / tickTime.getElapsedTime().asMilliseconds()));

	this->yRotation += angle;

	this->zVector = glm::normalize(
		this->zVector * cosf(angle) -
		this->yVector * sinf(angle));

	this->yVector = glm::cross(
		this->zVector, this->xVector);
}


void Camera::Roll(float angle, bool applyTime)
{
	if (applyTime)
		angle *= (1000 * (targetTickTime / tickTime.getElapsedTime().asMilliseconds()));

	this->zRotation += angle;

	this->yVector = glm::normalize(
		this->yVector * cosf(angle * piover180) +
		this->xVector * sinf(angle * piover180));
	
	this->xVector = glm::cross(
		this->zVector, this->yVector);

	this->xVector *= -1;
}


void Camera::Strafe(float distance, bool applyTime)
{
	if (applyTime)
		distance *= (1000 * (targetTickTime / tickTime.getElapsedTime().asMilliseconds()));
	this->position += (this->yVector * (distance));
}


void Camera::Ascend(float distance, bool applyTime)
{
	if (applyTime)
		distance *= (1000 * (targetTickTime / tickTime.getElapsedTime().asMilliseconds()));
	this->position += (this->xVector * (distance));
}


void Camera::Advance(float distance, bool applyTime)
{
	if (applyTime)
		distance *= (1000 * (targetTickTime / tickTime.getElapsedTime().asMilliseconds()));
	this->position += (this->zVector * -(distance));
}


glm::mat3 Camera::Place()
{
	tickTime.restart();
	
	glm::vec3 viewDirection = this->position + this->zVector;

	return glm::mat3(
		position[0], position[1], position[2],
		viewDirection[0], viewDirection[1], viewDirection[2],
		xVector[0], xVector[1], xVector[2]);
}