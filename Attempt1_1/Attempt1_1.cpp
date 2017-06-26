#include "GL\glew.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/OpenGL.hpp"
#include "Camera.h"
#include "QuatCamera.h"
#include "HeightMap.h"
#include "Overlay.h"
#include "Menu.h"
#include "Vehicle.h"
#include "Skybox.h"
#include <glm\glm.hpp>
#include <iostream>
#include <cmath>
#include <vector>

void HandleInput()
{

}

int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;          // Request a 24-bit depth buffer
	settings.stencilBits = 8;           // Request a 8 bits stencil buffer
	settings.antialiasingLevel = 2; // Request 2 levels of antialiasing

	// Use SFML to handle the window for us
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Height Map Flight Sim", sf::Style::Close, settings);

	// create vars for setting values from the menu
	int gridSize = 256;
	int gridWidth = 250;
	float min_height = -30.0f;
	float max_height = 1000.0f;
	HeightMap::RandomNumber random_number = HeightMap::RandomNumber::normalDistribution;
	float random_min;
	float random_max;
	HeightMap::Smoother smoother = HeightMap::Smoother::normalSmoothing;
	int smoothCount = 0;
	float offset = 1.2;

	Menu menu(window);
	if (!menu.Run())
		window.close();
	bool drawMenu = false;

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // nicest perspective correction calculations

	//prepare OpenGL surface for HSR
	//glClearDepth(1.f); // clear the z-buffer/depth buffer completely +1.0f is the furtherest away
	glClearColor(0.3f, 0.3f, 0.3f, 0.f); // set the background colour for when we clear the screen RGBA values in the 0.0 to 1.0 range. This gives us a nice grey background.

	// Setup a perspective projection & Camera position

	// GL_PROJECTION what we actually see
	glMatrixMode(GL_PROJECTION); // Select the builtin projection matrix
	glLoadIdentity();  // reset the projection matrix by loading the projection identity matrix

	GLdouble fovY = 90;
	GLdouble aspect = float(float(window.getSize().x) / float(window.getSize().y)); // 16:9 ascpect ratio = 1.77
	GLdouble zNear = 1.0f;
	GLdouble zFar = 1000000.0f;

	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;

	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;
	

	// define a perspective projection
	glFrustum(-fW, fW, -fH, fH, zNear, zFar); // multiply the set matrix; by a perspective matrix

	Skybox skybox;
	skybox.Setup();

	// ((pow(2, 8) + 1), 300, -50.0f, 3000.0f, HeightMap::Smoother::normalSmoothing, 3, HeightMap::RandomNumber::logNormalDistribution, 1.0f, 3.5f, 1.2f);
	HeightMap heightmap(
		menu.finalGridSize + 1, 
		menu.finalGridWidth, 
		menu.finalMinHeight, 
		menu.finalMaxHeight, 
		menu.finalSmoother, 
		menu.finalSmoothCount, 
		menu.finalNumberGen, 
		menu.finalNumberGenFirst, 
		menu.finalNumberGenSecond,
		menu.finalOffset
	);
	heightmap.GenerateHeightMap();

	Vehicle plane;
	plane.LoadObjectFile("../resources/PaperAirplane.obj");

	Overlay gameUI;
	gameUI.Setup(window);

	//Camera quatCamera(0.0f, 2000.0f, 5000.0f, 0.0f, 0.0f, 0.0f);

	QuatCamera quatCamera(0.0f, 500.0f, 0.0f, 0.01f, 0.01f, 0.01f);
	float flightSpeed = 15.0f;

	// bool for debugging. if false, the call to keep moving forward will not happen.
	sf::Clock keyTimeout;
	bool moveForward = false;//determines if movement is on or off at start
	bool firstLoop = true;

	// Start game loop
	while (window.isOpen())
	{
	
		if (window.hasFocus())
		{
			/// do input checks outside of sf event to avoid 'first key stutter'
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W)))
			{
				if (flightSpeed < 50.0f)
					flightSpeed += 0.01f;

			}

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
			{
				if (flightSpeed > 1.001f)
					flightSpeed -= 0.01f;
			}

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
			{
				quatCamera.Yaw(-0.01f);// , true);
			}

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
			{
				quatCamera.Yaw(0.01f);//, true);
			}

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
			{
				quatCamera.Pitch(0.01f);//, true);
			}

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
			{
				quatCamera.Pitch(-0.01f);//, true);
			}

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))
			{
				quatCamera.Roll(-0.01f);//, true);
			}

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
			{
				quatCamera.Roll(0.01f);//, true);
			}

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)))
			{
				if (keyTimeout.getElapsedTime().asSeconds() > 0.5)
				{
					if (moveForward)
						moveForward = false;
					else
						moveForward = true;
					keyTimeout.restart();
				}
			}

			//processing continual movement
			if (moveForward && !firstLoop)
				quatCamera.MoveForward(flightSpeed);//, true);
		}
		
		firstLoop = false;

		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//Prepare for drawing
		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear z-buffer and set previously selected colour
		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity(); // reset

		glMultMatrixf(quatCamera.GetViewMatrixAsArray());

		glewInit();

		window.pushGLStates();
		skybox.Render(quatCamera.GetViewMatrixAsArray());
		window.popGLStates();

		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xff);
		heightmap.Render();

		//draw the airplane
		window.pushGLStates();
		plane.Render();
		window.popGLStates();

		//update game UI values
		float mat[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, mat);
		gameUI.Update(mat[12], mat[13], mat[14], flightSpeed);
	
		//draw the game UI
		window.pushGLStates();
		gameUI.Draw(window);
		window.popGLStates();

		window.display();


	}

	return EXIT_SUCCESS;
};