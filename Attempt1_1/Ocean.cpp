/*
*	This file and its contents were written by the author, unless stated otherwise.
*	The code is in no way guaranteed to be in working condition,
*	nor does the author claim any responsibility for negative outcomes of running
*	the contents of this file.
*  Any use of this file must be done with the permission of the author.
*
*	@author		Jesse Schollitt
*/

#include "Ocean.h"



Ocean::Ocean()
{
}


Ocean::~Ocean()
{
}

void Ocean::Setup(float oceanSize, float y)
{
	size = oceanSize;
	height = y;


}

void Ocean::Render(float * matrix)
{
	glMultMatrixf(matrix);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(0.0f, 0.0f, 0.5f, 0.4f);
	glBegin(GL_QUADS);                      // Draw A Quad
	glVertex3f(-size, height, -size);              // Top Left
	glVertex3f(size, height, -size);              // Top Right
	glVertex3f(size, height, size);              // Bottom Right
	glVertex3f(-size, height, size);              // Bottom Left
	glEnd();
}
