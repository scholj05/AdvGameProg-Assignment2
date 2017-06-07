#include "Camera.h"


/// Set the initial position and direction of the camera
Camera::Camera(float x, float y, float z, float direction)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;

	this->xRotation = 0.0f;
	this->yRotation = 0.0f;
	this->zRotation += 0.0f;//direction;

	this->xVector = glm::vec3(1.0f, 0.0f, 0.0f);
	this->yVector = glm::vec3(0.0f, 1.0f, 0.0f);
	this->zVector = glm::vec3(0.0f, 0.0f, 1.0f);
	Roll(270);
}


Camera::~Camera()
{
}


void Camera::Pitch(float angle)
{
	this->xRotation += angle;

	this->zVector = glm::normalize(
		this->zVector * cosf(angle * piover180) +
		this->xVector * sinf(angle * piover180));

	this->xVector = glm::cross(
		this->zVector, this->yVector);

	this->xVector *= -1;
}


void Camera::Yaw(float angle)
{
	this->yRotation += angle;

	this->zVector = glm::normalize(
		this->zVector * cosf(angle * piover180) -
		this->yVector * sinf(angle* piover180));

	this->yVector = glm::cross(
		this->zVector, this->xVector);
}


void Camera::Roll(float angle)
{
	this->zRotation += angle;

	this->yVector = glm::normalize(
		this->yVector * cosf(angle * piover180) +
		this->xVector * sinf(angle * piover180));
	

	this->xVector = glm::cross(
		this->zVector, this->yVector);

	this->xVector *= -1;

}

void Camera::Advance(float distance)
{
	this->position += (this->zVector * -distance);
}


glm::mat3 Camera::Place()
{
	glm::vec3 viewDirection = this->position + this->zVector;

	return glm::mat3(
		position[0], position[1], position[2],
		viewDirection[0], viewDirection[1], viewDirection[2],
		xVector[0], xVector[1], xVector[2]);
}
