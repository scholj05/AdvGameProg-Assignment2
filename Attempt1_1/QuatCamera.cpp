#include "QuatCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;

//default constructor
QuatCamera::QuatCamera()
{
}
//constructor
QuatCamera::QuatCamera(float positionX, float positionY, float positionZ, float rotationX, float rotationY, float rotationZ)
{
	glm::vec3 angles(rotationX, DegToRad(rotationY), DegToRad(rotationZ));
	mOrientation = glm::quat(angles);
	MoveLeft(positionX, false);
	MoveUp(positionY, false);
	MoveForward(positionZ, false);
}

void QuatCamera::Pitch(float pitchRadians, bool time) 
{
	if (time) 
		pitchRadians *= tickTime.getElapsedTime().asSeconds();
	Rotate(pitchRadians, glm::vec3(1.0f, 0.0f, 0.0f));
}

void QuatCamera::Yaw(float yawRadians, bool time) 
{
	if (time) 
		yawRadians *= tickTime.getElapsedTime().asSeconds();
	Rotate(yawRadians, glm::vec3(0.0f, 1.0f, 0.0f));
}

void QuatCamera::Roll(float rollRadians, bool time) 
{
	if (time) 
		rollRadians *= tickTime.getElapsedTime().asSeconds();
	Rotate(rollRadians, glm::vec3(0.0f, 0.0f, 1.0f));
}

void QuatCamera::Turn(float turnRadians) 
{
	glm::quat q = glm::angleAxis(turnRadians, mOrientation * glm::vec3(0.0f, 1.0f, 0.0f));
	Rotate(q);
}

void QuatCamera::Rotate(float angleRadians, const glm::vec3& axis) 
{
	glm::quat q = glm::angleAxis(angleRadians, axis);
	Rotate(q);
}

void QuatCamera::Rotate(const glm::quat& rotation) 
{
	mOrientation = rotation * mOrientation;
}

glm::vec3 QuatCamera::GetForward() const 
{
	return glm::conjugate(mOrientation) * glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::vec3 QuatCamera::GetLeft() const 
{
	return glm::conjugate(mOrientation) * glm::vec3(-1.0, 0.0f, 0.0f);
}

glm::vec3 QuatCamera::GetUp() const 
{
	return glm::conjugate(mOrientation) * glm::vec3(0.0f, 1.0f, 0.0f);
}

void QuatCamera::MoveForward(float movement, bool time) 
{
	if (time) 
		movement *= tickTime.getElapsedTime().asSeconds();
	mPosition += GetForward() * movement;
}

void QuatCamera::MoveLeft(float movement, bool time) 
{
	if (time) 
		movement *= tickTime.getElapsedTime().asSeconds();
	mPosition += GetLeft() * movement;
}

void QuatCamera::MoveUp(float movement, bool time) 
{
	if (time) 
		movement *= tickTime.getElapsedTime().asSeconds();
	mPosition += GetUp() * movement;
}

void QuatCamera::Tick()
{
	tickTime.restart();
}

glm::mat4 QuatCamera::GetViewMatrix()  
{
	Tick();
	glm::mat4 viewMatrix = glm::mat4_cast(mOrientation);
	viewMatrix = glm::translate(viewMatrix, -mPosition);
	return viewMatrix;
}

float* QuatCamera::GetViewMatrixAsArray() 
{
	glm::mat4 viewMatrix(GetViewMatrix());
	float *view = new float[16];
	const float *pSource = (const float*)glm::value_ptr(viewMatrix);
	for (int i = 0; i < 16; ++i)
		view[i] = pSource[i];
	return view;
}

float* QuatCamera::GetInverseViewMatrix() 
{
	glm::mat4 inverse(glm::inverseTranspose(GetViewMatrix()));
	float *view = new float[16];
	const float *pSource = (const float*)glm::value_ptr(inverse);
	for (int i = 0; i < 16; ++i)
		view[i] = pSource[i];
	return view;
}

