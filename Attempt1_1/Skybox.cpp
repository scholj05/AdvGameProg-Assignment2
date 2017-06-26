#include "Skybox.h"



Skybox::Skybox()
{
}


Skybox::~Skybox()
{
}

void Skybox::Setup()
{
	right.loadFromFile("../resources/skybox/right.png");
	left.loadFromFile("../resources/skybox/left.png");
	top.loadFromFile("../resources/skybox/top.png");
	bottom.loadFromFile("../resources/skybox/bottom.png");
	back.loadFromFile("../resources/skybox/back.png");
	front.loadFromFile("../resources/skybox/front.png");

	

	glGenTextures(1, &right_handle); // allocate a texture handle within OpenGL
	glGenTextures(1, &left_handle); // allocate a texture handle within OpenGL
	glGenTextures(1, &top_handle); // allocate a texture handle within OpenGL
	glGenTextures(1, &bottom_handle); // allocate a texture handle within OpenGL
	glGenTextures(1, &back_handle); // allocate a texture handle within OpenGL
	glGenTextures(1, &front_handle); // allocate a texture handle within OpenGL

	glBindTexture(GL_TEXTURE_2D, right_handle); // bind to the texture handle target
												// import texture into OpenGL
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		right.getSize().x, right.getSize().y,
		0,
		GL_RGBA, GL_UNSIGNED_BYTE, right.getPixelsPtr()
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, left_handle); // bind to the texture handle target
											   // import texture into OpenGL
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		left.getSize().x, left.getSize().y,
		0,
		GL_RGBA, GL_UNSIGNED_BYTE, left.getPixelsPtr()
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, top_handle); // bind to the texture handle target
											  // import texture into OpenGL
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		top.getSize().x, top.getSize().y,
		0,
		GL_RGBA, GL_UNSIGNED_BYTE, top.getPixelsPtr()
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, bottom_handle); // bind to the texture handle target
												 // import texture into OpenGL
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		bottom.getSize().x, bottom.getSize().y,
		0,
		GL_RGBA, GL_UNSIGNED_BYTE, bottom.getPixelsPtr()
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, back_handle); // bind to the texture handle target
											   // import texture into OpenGL
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		back.getSize().x, back.getSize().y,
		0,
		GL_RGBA, GL_UNSIGNED_BYTE, back.getPixelsPtr()
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, front_handle); // bind to the texture handle target
												// import texture into OpenGL
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		front.getSize().x, front.getSize().y,
		0,
		GL_RGBA, GL_UNSIGNED_BYTE, front.getPixelsPtr()
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glEnable(GL_TEXTURE_2D); // enable 2D textures
	glEnable(GL_DEPTH_TEST); // use the depth buffer to remove hidden surfaces to prevent their render + calculation
	glDepthMask(GL_TRUE); // enable program to write into the depth buffer


}

void Skybox::Render(float * matrix)
{
	
	glMultMatrixf(matrix);
	
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE); // enable back face culling so we don't see the hidden vertices / textures of the cube
	glCullFace(GL_FRONT);

	glBindTexture(GL_TEXTURE_2D, right_handle); // bind to the texture handle target
	glBegin(GL_QUADS);
	// Right face
	glNormal3f(-1.0f, 0.0f, 0.0f); // point left
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100000.0f, -100000.0f, -100000.0f);  // Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100000.0f, 100000.0f, -100000.0f);  // Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(100000.0f, 100000.0f, 100000.0f);  // Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(100000.0f, -100000.0f, 100000.0f);  // Bottom Left Of The Texture and Quad
	glEnd();

	glBindTexture(GL_TEXTURE_2D, left_handle); // bind to the texture handle target
	glBegin(GL_QUADS);
	// Left Face
	glNormal3f(1.0f, 0.0f, 0.0f); // point right
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100000.0f, -100000.0f, -100000.0f);  // Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-100000.0f, -100000.0f, 100000.0f);  // Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-100000.0f, 100000.0f, 100000.0f);  // Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-100000.0f, 100000.0f, -100000.0f);  // Top Left Of The Texture and Quad
	glEnd();

	glBindTexture(GL_TEXTURE_2D, top_handle); // bind to the texture handle target
	glBegin(GL_QUADS);
	// Top Face
	glNormal3f(0.0f, -1.0f, 0.0f); // point down
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-100000.0f, 100000.0f, -100000.0f);  // Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100000.0f, 100000.0f, 100000.0f);  // Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100000.0f, 100000.0f, 100000.0f);  // Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100000.0f, 100000.0f, -100000.0f);  // Top Right Of The Texture and Quad
	glEnd();

	glBindTexture(GL_TEXTURE_2D, bottom_handle); // bind to the texture handle target
	glBegin(GL_QUADS);
	// Bottom Face
	glNormal3f(0.0f, 1.0f, 0.0f); // point up
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-100000.0f, -100000.0f, -100000.0f);  // Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(100000.0f, -100000.0f, -100000.0f);  // Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(100000.0f, -100000.0f, 100000.0f);  // Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-100000.0f, -100000.0f, 100000.0f);  // Bottom Right Of The Texture and Quad
	glEnd();

	glBindTexture(GL_TEXTURE_2D, back_handle); // bind to the texture handle target
	glBegin(GL_QUADS);
	// Back Face
	glNormal3f(0.0f, 0.0f, -1.0f); // point out of the screen
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-100000.0f, -100000.0f, -100000.0f);  // Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-100000.0f, 100000.0f, -100000.0f);  // Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(100000.0f, 100000.0f, -100000.0f);  // Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(100000.0f, -100000.0f, -100000.0f);  // Bottom Left Of The Texture and Quad
	glEnd();

	glBindTexture(GL_TEXTURE_2D, front_handle); // bind to the texture handle target
	glBegin(GL_QUADS);
	// Front Face
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100000.0f, -100000.0f, 100000.0f);  // Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100000.0f, -100000.0f, 100000.0f);  // Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100000.0f, 100000.0f, 100000.0f);  // Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-100000.0f, 100000.0f, 100000.0f);  // Top Left Of The Texture and Quad
	glEnd();

	glEnable(GL_DEPTH_TEST);
}
