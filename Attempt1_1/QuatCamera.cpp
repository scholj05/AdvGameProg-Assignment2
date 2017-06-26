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





















/* graveyard */
/*
QuatCamera::QuatCamera(float x, float y, float z)
{
cameraPos = glm::vec3(x, y, z);
glm::vec3 cameraPosAsRad = glm::vec3(DegToRad(cameraPos.x), DegToRad(cameraPos.y), DegToRad(cameraPos.z));
cameraQuat = glm::quat(cameraPosAsRad);
}


QuatCamera::~QuatCamera()
{
}

double QuatCamera::Length(glm::quat)
{
return double(0.0000);
}

glm::quat QuatCamera::Normalise(glm::quat quat)
{
return glm::normalize(quat);
}


glm::quat QuatCamera::Update()
{
glm::fquat pitchQuat(cos(TO_RADIANS(pitch / 2.0f)), cross(m_direction, vec3(0, 1, 0) * (float)sin(TO_RADIANS(pitch / 2.0f)));
glm::fquat yawQuat(cos(TO_RADIANS(yaw / 2.0f)), vec3(0, 1, 0) * (float)sin(TO_RADIANS(yaw / 2.0f)));
glm::fquat rollQuat(cos(TO_RADIANS(roll / 2.0f)), m_direction * (float)sin(TO_RADIANS(roll / 2.0f)));
m_rotation = yawQuat * pitchQuat * rollQuat;

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
cameraQuat = Normalise(cameraQuat);
glm::mat4 rotate = glm::mat4_cast(cameraQuat);

glm::mat4 translate = glm::mat4(1.0f);
translate = glm::translate(translate, -cameraPos);

return (rotate * translate);



// for use in third person camera
glm::quat QuatCamera::RotateTowards(glm::quat q1, glm::quat q2, float maxAngle) {

	if (maxAngle < 0.001f) {
		// No rotation allowed. Prevent dividing by 0 later.
		return q1;
	}

	float cosTheta = dot(q1, q2);

	// q1 and q2 are already equal.
	// Force q2 just to be sure
	if (cosTheta > 0.9999f) {
		return q2;
	}

	// Avoid taking the long path around the sphere
	if (cosTheta < 0) {
		q1 = q1*-1.0f;
		cosTheta *= -1.0f;
	}

	float angle = acos(cosTheta);

	// If there is only a 2&deg; difference, and we are allowed 5&deg;,
	// then we arrived.
	if (angle < maxAngle) {
		return q2;
	}

	float fT = maxAngle / angle;
	angle = maxAngle;

	glm::quat res = (sin((1.0f - fT) * angle) * q1 + sin(fT * angle) * q2) / sin(angle);
	res = glm::normalize(res);
	return res;

}

void QuatCamera::Rotate(float x, float y, float z)
{
	float scaleX = (x / 2);
	float scaleY = (y / 2);
	float scaleZ = (z / 2);

	glm::quat quatX(cos(DegToRad(x) / 2), sin(scaleX), 0, 0);
	glm::quat quatY(cos(DegToRad(y) / 2), 0, sin(scaleY), 0);
	glm::quat quatZ(cos(DegToRad(z) / 2), 0, 0, sin(scaleZ));

	//quatX = glm::normalize(quatX);
	//quatY = glm::normalize(quatY);
	//quatZ = glm::normalize(quatZ);

	glm::quat totalRotation(quatZ.operator*=(quatY.operator*=(quatX)));
}
}*/
