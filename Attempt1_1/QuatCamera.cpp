/*
*	This file and its contents were written by the author, unless stated otherwise.
*	The code is in no way guaranteed to be in working condition,
*	nor does the author claim any responsibility for negative outcomes of running
*	the contents of this file.
*  Any use of this file must be done with the permission of the author.
*
*	@author		Jesse Schollitt
*/

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
	pitch = yaw = roll = 0;
}

void QuatCamera::Pitch(float pitchRadians, bool time) 
{
	if (time) 
		pitchRadians *= tickTime.getElapsedTime().asSeconds();
	Rotate(pitchRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	pitch += RadToDeg(pitchRadians);
}

void QuatCamera::Yaw(float yawRadians, bool time) 
{
	if (time) 
		yawRadians *= tickTime.getElapsedTime().asSeconds();
	Rotate(yawRadians, glm::vec3(0.0f, 1.0f, 0.0f));
	yaw += RadToDeg(yawRadians);
}

void QuatCamera::Roll(float rollRadians, bool time) 
{
	if (time) 
		rollRadians *= tickTime.getElapsedTime().asSeconds();
	Rotate(rollRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	roll += RadToDeg(rollRadians);
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

glm::vec3 QuatCamera::GetEulerAngles()
{
	float * mat4 = GetViewMatrixAsArray();

	if (mat4[0] == 1.0f)
	{
		yaw = atan2f(mat4[2], mat4[11]);
		pitch = 0;
		roll = 0;

	}
	else if (mat4[0] == -1.0f)
	{
		yaw = atan2f(mat4[2], mat4[11]);
		pitch = 0;
		roll = 0;
	}
	else
	{

		yaw = atan2(-mat4[8], mat4[0]);
		pitch = asin(mat4[4]);
		roll = atan2(-mat4[6], mat4[5]);
	}
	return glm::vec3(RadToDeg(pitch), RadToDeg(yaw), RadToDeg(roll));
}

float QuatCamera::GetPitch()
{
	return (fmod(pitch, 360));
}

float QuatCamera::GetYaw()
{
	return (fmod(yaw, 360));
}

float QuatCamera::GetRoll()
{
	return (fmod(roll, 360));
}

float QuatCamera::GetAlt()
{
	return mPosition.y;
}
