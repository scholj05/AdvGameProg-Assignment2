/*
*	This file and its contents were written by the author, unless stated otherwise.
*	The code is in no way guaranteed to be in working condition,
*	nor does the author claim any responsibility for negative outcomes of running
*	the contents of this file.
*  Any use of this file must be done with the permission of the author.
*
*	@author		Jesse Schollitt
*/

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

	void Pitch(float pitchRadians, bool time);

	void Yaw(float yawRadians, bool time);

	void Roll(float rollRadians, bool time);

	void Turn(float turnRadians);

	void Rotate(float angleRadians, const glm::vec3 & axis);

	void Rotate(const glm::quat & rotation);

	glm::vec3 GetForward() const;

	glm::vec3 GetLeft() const;

	glm::vec3 GetUp() const;

	void MoveForward(float movement, bool time);

	void MoveLeft(float movement, bool time);

	void MoveUp(float movement, bool time);

	void Tick();

	glm::mat4 GetViewMatrix() ;

	float * GetViewMatrixAsArray() ;

	float * GetInverseViewMatrix() ;

	glm::vec3 GetEulerAngles();

	float GetPitch();
	float GetYaw();
	float GetRoll();

	float GetAlt();



private:

	glm::vec3 mPosition;
	glm::quat mOrientation;

	sf::Clock tickTime;

	float pitch, yaw, roll;

	inline float QuatCamera::DegToRad(float degrees)
	{
		float pi = (float)PI;
		return degrees * (pi / 180);
	}

	inline float QuatCamera::RadToDeg(float rads)
	{
		float pi = (float)PI;
		return rads * (180 / pi);
	}
};

