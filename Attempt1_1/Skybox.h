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
#include "GL\glew.h"
#include "SFML\OpenGL.hpp"
#include "SFML\Graphics.hpp"
#include "glm\common.hpp"
#include <string>

class Skybox
{
public:
	Skybox();

	~Skybox();
	
	void Skybox::Setup(float boxSize);

	void Skybox::Render( float * matrix);
	float GetSize();
private:
	//allocate handles
	GLuint right_handle;
	GLuint left_handle;
	GLuint top_handle;
	GLuint bottom_handle;
	GLuint back_handle;
	GLuint front_handle;

	std::vector<GLfloat> vectorBuffer;
	std::vector<GLint> facesIndex;
	std::vector<GLfloat> textureBuffer;

	//image holders
	sf::Image top, bottom, left, right, front, back;

	float boxSize;
};

