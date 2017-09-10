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
#include "SFML\OpenGL.hpp"
class Ocean
{
public:
	Ocean();
	~Ocean();
	void Setup(float oceanSize, float y);
	void Ocean::Render(float * matrix);

private:
	float size;
	float height;

};

